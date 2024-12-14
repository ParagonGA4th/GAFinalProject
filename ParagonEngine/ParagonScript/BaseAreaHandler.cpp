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
		//Transform ���� ��� �����ֱ�.
		for (auto& [bUINT, bMap] : _managedMovingObjectList)
		{
			for (auto& [bObjName, bAgg] : bMap)
			{
				//������� Transform �ٲ��ֱ�.
				bAgg._transStorage.ToTransform(&(bAgg._moveObj->_transform));

				//�ٽ� ������� ������ ���� ��������.
				bAgg._moveBehav->ResetAll();
			}
		}

		//IConfinedArea ���� ������ ó��.
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
		//������ ���� �ۿ� �ε����� ���� �� ���� �ϱ� ����.
		unsigned int tAreaIndex = std::clamp<unsigned int>(index, 0, _entireAreaCount - 1);

		auto& bIndexMap = _managedMovingObjectList.at(tAreaIndex);
		for (auto& [bObjName, bAgg] : bIndexMap)
		{
			bAgg._moveBehav->ResetAll();
		}

		//IConfinedArea ���� ������ ó��.
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
		//�÷��̾� ���̰� 
		//SceneReset, �ϴ��� ��ü��.
		//���� �Ĵ��� NO. (�׽�Ʈ ���� ����)

		// �ϳ���ŭ Player�� ������ ���.
		// Clamp�ϱ� ������, Ȯ�������� ���̴� ���̴�.
		CombatSystem::GetInstance(nullptr)->ChangePlayerHealth(-1.0f);
	}

	void BaseAreaHandler::GrabOrganizeAllAreas(Pg::Data::GameObject* obj)
	{
		// Project Load ��.
		// ��Ƽ��Ʈ ���� + ���� ������ ���⿡�� �����ؾ� �Ѵ�. 
		// ������ ������ ���� �������� ���ΰ�,
		// ���� �޾ƿ;� �� ��.
		// ��·�� ��� ������ �� ��, ��� ������Ʈ�� �޾ƿ;� �Ѵ�.
		// ���߿� �ٸ� �����̳� �׷� �������� ������ ������Ʈ / ����.

		// �� ���(����, ���� ��)���� ��� ������ ���� �ƴϴ�.
		// 4�� ���̾��� ���, �����;� �� ���̴�.
		// �̷���  �⺻ �������̽� ���̽� Ŭ���� �ʿ�.

		// ������ Object�δ� �� �� ����. ���� Scene���� �ʿ� ������Ʈ�� �����´�. 
		// Collider�� �ִ� ģ����. LAYER_MOVABLE_OBJECTS �˻�. 
		// ��� ������ ���� Renderer�� �پ��־�� �Ѵ�.
		// ��� ������ IMovableObject�� ��ӹ޴� ������Ʈ�� �����;� �Ѵ�. 
		// Renderer == Collider�̱� ������, �������� XML ������ �ʿ��� ��.

		//�� �ȿ� ��ü Area Index��ŭ Map�� ä���ֱ�. 
		//�� Handler���� �޶�� �Ѵ�.
		//Ex.

		Pg::Data::GameObject* _object = obj;
		for (int i = 0; i < _entireAreaCount; i++)
		{
			//.at�� out of index�� ���� �ʰ� �ϱ� ���ؼ�.
			_managedMovingObjectList.insert(std::make_pair(i, std::unordered_map<std::string,
				MovingObjectAggregate>()));

			//��������. 
			_confinedAreaList.push_back(std::make_unique<std::vector<IConfinedArea*>>());
		}

		for (auto& bObj : _object->GetScene()->GetObjectList())
		{
			// Layer �˻�� ���� ���� ���� : 
			// LAYER_MOVABLE_OBJECTS�� w/o ����.
			IMovableObject* tMo = bObj->GetComponent<IMovableObject>();

			if (tMo != nullptr)
			{
				// Collider�� �ִٴ� ��.
				// ���� ���� ������ �������� �ʰ�, ������ ������ �� �ִ� �ָ�.
				// Renderer�� ���� ������Ʈ ���ο� ���� ���̴�.
				//assert((tMo != nullptr)
				//	&& "Layer�� ������ MOVABLE_OBJECTS��, IMoveableObject ��ӹ��� ������Ʈ�� ���������� ������ �־�� �Ѵ�.");

				//��ü���� �޶�� �Ѵ�.
				unsigned int tDesignatedIndex = tMo->GetDesignatedAreaIndex();
				auto& bIndexedMap = _managedMovingObjectList.at(tDesignatedIndex);

				//������ �غ�.
				MovingObjectAggregate tToInsert;
				tToInsert._moveBehav = tMo;
				tToInsert._moveObj = bObj;
				tToInsert._transStorage = TransformSimpleStorage(&(bObj->_transform));

				bIndexedMap.insert(std::make_pair(bObj->GetName(), tToInsert));

				continue;
			}

			//BattleArea / TrapArea �� IConfinedArea�� �޾ƾ� �Ѵ�.
			IConfinedArea* tConfinedArea = bObj->GetComponent<IConfinedArea>();
			if (tConfinedArea != nullptr)
			{
				unsigned int tDesignatedIndex = tConfinedArea->GetDesignatedAreaIndex();
				auto& bVecPtr = _confinedAreaList.at(tDesignatedIndex);
				bVecPtr->push_back(tConfinedArea);

				continue;
			}

			//������ DeathPlane / AreaPassingTrigger �˻�.
			//��ü ���� + �ڽ��� ��ü �ּ� ����.
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
