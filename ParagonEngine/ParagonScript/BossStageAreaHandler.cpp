#include "BossStageAreaHandler.h"

#include "../ParagonData/Collider.h"
#include "../ParagonData/Scene.h"
#include "../ParagonData/LayerMask.h"

#include "../ParagonUtil/CustomAssert.h"

namespace Pg::DataScript
{
	BossStageAreaHandler::BossStageAreaHandler(Pg::Data::GameObject* obj) : ScriptInterface(obj)
	{

	}

	void BossStageAreaHandler::GrabManagedObjects()
	{
		//Project Load 시.
		// 아티팩트 관리 + 프랍 관리를 여기에서 진행해야 한다.
		// 움직일 이유가 없는 옵젝들은 놔두고,
		// 값을 받아와야 할 것.
		// 어쨌든 얘는 리셋을 할 때, 모든 오브젝트를 받아와야 한다.
		// 나중에 다른 로직이나 그런 곳에서는 독자적 업데이트 / 관리.

		// 맵 기믹(함정, 전투 등)들이 모두 별개의 것이 아니다.
		// 4번 레이어일 경우, 가져와야 할 것이다.
		// 이러면  기본 인터페이스 베이스 클래스 필요.

		std::vector<Pg::Data::GameObject*> tColliderEntireList;

		// 별개로 Object로는 뺄 수 없다. 따로 Scene에서 필요 오브젝트를 가져온다. 
		// Collider가 있는 친구들. LAYER_MOVABLE_OBJECTS 검사. 
		// 얘는 무조건 같이 Renderer가 붙어있어야 한다.
		// 얘는 무조건 IMovableObject를 상속받는 컴포넌트를 가져와야 한다. 
		// Renderer == Collider이기 때문에, 개별적인 XML 편집이 필요할 것.
		for (auto& bObj : _object->GetScene()->GetObjectList())
		{
			Pg::Data::Collider* tCol = bObj->GetComponent<Pg::Data::Collider>();

			if (tCol != nullptr)
			{
				// Collider가 있다는 것.
				// 따로 기존 로직에 관여하지 않고, 옵젝중 움직일 수 있는 애만.
				// Renderer는 동일 오브젝트 내부에 있을 것이다.
				if (tCol->GetLayer() == Pg::Data::Enums::eLayerMask::LAYER_MOVABLE_OBJECTS)
				{
					IMovableObject* tMo = bObj->GetComponent<IMovableObject>();
					assert((tMo != nullptr)
						&& "Layer가 무조건 MOVABLE_OBJECTS면, IMoveableObject 상속받은 오브젝트를 내부적으로 가지고 있어야 한다.");

					//저장할 준비.
					MovingObjectAggregate tToInsert;
					tToInsert._moveBehav = tMo;
					tToInsert._moveObj = bObj;
					tToInsert._transStorage = TransformSimpleStorage(&(bObj->_transform));

					_managedMovingObjectList.insert(std::make_pair(bObj->GetName(), tToInsert));
				}
			}
		}
	}

	void BossStageAreaHandler::Start()
	{

	}

	void BossStageAreaHandler::Update()
	{

	}

	void BossStageAreaHandler::ResetToInitialState()
	{
		//Transform 빼고 모두 돌려주기.
		for (auto& [bObjName, bAgg] : _managedMovingObjectList)
		{
			//원래대로 Transform 바꿔주기.
			bAgg._transStorage.ToTransform(&(bAgg._moveObj->_transform));

			//다시 원래대로 본인의 정보 돌려놓기.
			bAgg._moveBehav->ResetAll();
		}
	}

	void BossStageAreaHandler::ResetAreaWithIndex(unsigned int index)
	{

	}

	void BossStageAreaHandler::SetCurrentAreaIndex(unsigned int index)
	{

	}

	void BossStageAreaHandler::OnPlayerHitDeathPlane()
	{

	}

}