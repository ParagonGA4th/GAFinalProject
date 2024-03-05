#include "Test_CSync_RecordCurrentPos.h"
#include "BTHelper.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/Transform.h"
#include "../ParagonUtil/Log.h"
namespace Pg::Engine::BTree::Node
{
	using Pg::Engine::BTree::BTHelper;

	BT::NodeStatus Test_CSync_RecordCurrentPos::tick()
	{
		//PG_TRACE(typeid(this).name());

		//현재 자신이 속한 오브젝트를 받아왔다. (InputPort)
		//이후, 어떤 포지션을 가지고 있는지 OutputPort로 기록한다.)
		BTHelper::Validate(setOutput<Pg::Math::PGFLOAT3>("_currentPos", GetGameObject()->_transform._position));
		//또한, Tree당 하나의 Blackboard를 가지고 있으므로, 동일 Node 사이에서 공유된다.
		//걱정할 내용 X.

		//여기까지 오면 해당 작용에는 성공했으므로, SUCCESS 반환. 
		return BT::NodeStatus::SUCCESS;
	}

}