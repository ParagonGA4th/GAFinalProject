#include "Stage1AreaHandler.h"

#include "../ParagonData/Collider.h"
#include "../ParagonData/Scene.h"
#include "../ParagonData/LayerMask.h"

namespace Pg::DataScript
{
	Stage1AreaHandler::Stage1AreaHandler(Pg::Data::GameObject* obj) : ScriptInterface(obj)
	{

	}

	void Stage1AreaHandler::GrabManagedObjects()
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
		// 얘는 무조건 IMovableObject를 상속받는 컴포넌트를 가져와야 한다. 
		// Renderer != Collider이기 때문에, 개별적인 XML 편집이 필요할 것.
		for (auto& it : _object->GetScene()->GetObjectList())
		{
			Pg::Data::Collider* tCol = it->GetComponent<Pg::Data::Collider>();

			if (tCol != nullptr)
			{
				// Collider가 있다는 것.
				// 따로 기존 로직에 관여하지 않고, 옵젝중 움직일 수 있는 애만.
				// Renderer 매칭 : 결과적으로 
				if (tCol->GetLayer() == Pg::Data::Enums::eLayerMask::LAYER_MOVABLE_OBJECTS)
				{

				}
			}
		}



	}

	void Stage1AreaHandler::Start()
	{

	}

	void Stage1AreaHandler::Update()
	{

	}

	void Stage1AreaHandler::ResetToInitialState()
	{

	}

}