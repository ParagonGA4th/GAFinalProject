#include "BaseAreaHandler.h"
#include "HandleBundle3D.h"

#include "../ParagonData/Collider.h"
#include "../ParagonData/Scene.h"
#include "../ParagonData/LayerMask.h"

#include "DeathPlane.h"
#include "AreaPassingTrigger.h"

#include "InGameManager.h"
#include "TotalGameManager.h"
#include "PlayerHandler.h"
#include "CombatSystem.h"

#include "../ParagonUtil/CustomAssert.h"

#include <algorithm>

namespace Pg::DataScript
{
	BaseAreaHandler::BaseAreaHandler(unsigned int entireAreaCount) : _entireAreaCount(entireAreaCount)
	{

	}

	void BaseAreaHandler::ResetToInitialState()
	{
		//Transform 빼고 모두 돌려주기.
		for (auto& [bUINT, bMap] : _managedMovingObjectList)
		{
			for (auto& [bObjName, bAgg] : bMap)
			{
				//원래대로 Transform 바꿔주기.
				bAgg._transStorage.ToTransform(&(bAgg._moveObj->_transform));

				//다시 원래대로 본인의 정보 돌려놓기.
				bAgg._moveBehav->ResetAll();
			}
		}

		//IConfinedArea 역시 동일한 처리.
		for (auto& bConfinedIndexVec : _confinedAreaList)
		{
			for (auto& it : *bConfinedIndexVec)
			{
				it->ResetAll();
			}
		}
	}

	void BaseAreaHandler::ResetAreaWithIndex(unsigned int index)
	{
		//무조건 영역 밖에 인덱스가 들어올 수 없게 하기 위해.
		unsigned int tAreaIndex = std::clamp<unsigned int>(index, 0, _entireAreaCount - 1);

		auto& bIndexMap = _managedMovingObjectList.at(tAreaIndex);
		for (auto& [bObjName, bAgg] : bIndexMap)
		{
			bAgg._moveBehav->ResetAll();
		}

		//IConfinedArea 역시 동일한 처리.
		auto& bConfinedIndexVec = _confinedAreaList.at(tAreaIndex);
		for (auto& it : *bConfinedIndexVec)
		{
			it->ResetAll();
		}
	}

	void BaseAreaHandler::SetCurrentAreaIndex(unsigned int index)
	{
		_currentAreaIndex = index;
	}

	void BaseAreaHandler::OnPlayerHitDeathPlane()
	{
		//플레이어 죽이고 
		//SceneReset, 일단은 전체로.
		//영역 냐누기 NO. (테스트 빌드 기준)

		// 하나만큼 Player의 라이프 깎기.
		// Clamp하기 때문에, 확정적으로 죽이는 것이다.
		CombatSystem::GetInstance(nullptr)->ChangePlayerHealth(-1.0f);
	}

	void BaseAreaHandler::GrabOrganizeAllAreas(Pg::Data::GameObject* obj)
	{
		// Project Load 시.
		// 아티팩트 관리 + 프랍 관리를 여기에서 진행해야 한다. 
		// 움직일 이유가 없는 옵젝들은 놔두고,
		// 값을 받아와야 할 것.
		// 어쨌든 얘는 리셋을 할 때, 모든 오브젝트를 받아와야 한다.
		// 나중에 다른 로직이나 그런 곳에서는 독자적 업데이트 / 관리.

		// 맵 기믹(함정, 전투 등)들이 모두 별개의 것이 아니다.
		// 4번 레이어일 경우, 가져와야 할 것이다.
		// 이러면  기본 인터페이스 베이스 클래스 필요.

		// 별개로 Object로는 뺄 수 없다. 따로 Scene에서 필요 오브젝트를 가져온다. 
		// Collider가 있는 친구들. LAYER_MOVABLE_OBJECTS 검사. 
		// 얘는 무조건 같이 Renderer가 붙어있어야 한다.
		// 얘는 무조건 IMovableObject를 상속받는 컴포넌트를 가져와야 한다. 
		// Renderer == Collider이기 때문에, 개별적인 XML 편집이 필요할 것.

		//씬 안에 전체 Area Index만큼 Map을 채워넣기. 
		//각 Handler마다 달라야 한다.
		//Ex.

		Pg::Data::GameObject* _object = obj;
		for (int i = 0; i < _entireAreaCount; i++)
		{
			//.at이 out of index가 뜨지 않게 하기 위해서.
			_managedMovingObjectList.insert(std::make_pair(i, std::unordered_map<std::string,
				MovingObjectAggregate>()));

			//마찬가지. 
			_confinedAreaList.push_back(std::make_unique<std::vector<IConfinedArea*>>());
		}

		for (auto& bObj : _object->GetScene()->GetObjectList())
		{
			// Layer 검사는 따로 하지 않음 : 
			// LAYER_MOVABLE_OBJECTS는 w/o 가능.
			IMovableObject* tMo = bObj->GetComponent<IMovableObject>();

			if (tMo != nullptr)
			{
				// Collider가 있다는 것.
				// 따로 기존 로직에 관여하지 않고, 옵젝중 움직일 수 있는 애만.
				// Renderer는 동일 오브젝트 내부에 있을 것이다.
				//assert((tMo != nullptr)
				//	&& "Layer가 무조건 MOVABLE_OBJECTS면, IMoveableObject 상속받은 오브젝트를 내부적으로 가지고 있어야 한다.");

				//객체별로 달라야 한다.
				unsigned int tDesignatedIndex = tMo->GetDesignatedAreaIndex();
				auto& bIndexedMap = _managedMovingObjectList.at(tDesignatedIndex);

				//저장할 준비.
				MovingObjectAggregate tToInsert;
				tToInsert._moveBehav = tMo;
				tToInsert._moveObj = bObj;
				tToInsert._transStorage = TransformSimpleStorage(&(bObj->_transform));

				bIndexedMap.insert(std::make_pair(bObj->GetName(), tToInsert));

				continue;
			}

			//BattleArea / TrapArea 등 IConfinedArea를 받아야 한다.
			IConfinedArea* tConfinedArea = bObj->GetComponent<IConfinedArea>();
			if (tConfinedArea != nullptr)
			{
				unsigned int tDesignatedIndex = tConfinedArea->GetDesignatedAreaIndex();
				auto& bVecPtr = _confinedAreaList.at(tDesignatedIndex);
				bVecPtr->push_back(tConfinedArea);

				continue;
			}

			//이제는 DeathPlane / AreaPassingTrigger 검사.
			//자체 저장 + 자신의 객체 주소 전달.
			DeathPlane* tDeathPlane = bObj->GetComponent<DeathPlane>();
			if (tDeathPlane != nullptr)
			{
				tDeathPlane->StoreDesignatedAreaHandler(this);
				_deathPlaneList.push_back(tDeathPlane);

				continue;
			}

			AreaPassingTrigger* tAreaTrigger = bObj->GetComponent<AreaPassingTrigger>();
			if (tAreaTrigger != nullptr)
			{
				tAreaTrigger->StoreDesignatedAreaHandler(this);
				_areaTriggerMap.insert(std::make_pair(tAreaTrigger,
					(unsigned int)(tAreaTrigger->_areaIndex)));

				continue;
			}
		}
	}

	void BaseAreaHandler::SetActivateConfinedAreaIndex(unsigned int index, bool val)
	{
		auto& bConfinedIndexVec = _confinedAreaList.at(index);
		for (auto& it : *bConfinedIndexVec)
		{
			it->SetActivate(val);
		}
	}

}
