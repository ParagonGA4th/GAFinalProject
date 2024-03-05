#include "Test_CSync_JumpAtBumpLimit.h"
#include "../ParagonUtil/Log.h"
#include "BTHelper.h"

namespace Pg::Engine::BTree::Node
{
	void Test_CSync_JumpAtBumpLimit::InitCustom()
	{
		//_collider = GetObject()->GetComponent<Pg::Data::DynamicCollider>();
		//assert(_collider != nullptr);

		//{}¸¦ ¶¼Áà¾ß ÇÑ´Ù.
		config().blackboard->set<int>("JUMPCOUNT", 0);
		//°³¾ß¸Å Initialize.
	}

	BT::NodeStatus Test_CSync_JumpAtBumpLimit::tick()
	{
		//PG_TRACE(typeid(this).name());

		auto tVal = getInput<int>("_jumpCount");
		BT_VALIDATE(tVal);
	
		if (tVal.value() == 5)
		{
			if (tToggle)
			{
				GetGameObject()->_transform._position.y = 0.f;
			}
			else
			{
				GetGameObject()->_transform._position.y = -4.f;
			}
			
			tToggle = !tToggle;
		}

		return BT::NodeStatus::SUCCESS;
	}
}