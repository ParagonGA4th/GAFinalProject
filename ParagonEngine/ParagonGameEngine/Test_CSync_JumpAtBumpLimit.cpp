#include "Test_CSync_JumpAtBumpLimit.h"
#include "../ParagonUtil/Log.h"
namespace Pg::Engine::BTree::Node
{
	void Test_CSync_JumpAtBumpLimit::InitCustom()
	{
		//_collider = GetObject()->GetComponent<Pg::Data::DynamicCollider>();
		//assert(_collider != nullptr);
	}

	BT::NodeStatus Test_CSync_JumpAtBumpLimit::tick()
	{
		//PG_TRACE(typeid(this).name());

		int tVal;
		getInput<int>("_jumpCount", tVal);

		if (tVal == 5)
		{
			GetGameObject()->_transform._position.y += 1.0f;
		}

		return BT::NodeStatus::SUCCESS;
	}
}