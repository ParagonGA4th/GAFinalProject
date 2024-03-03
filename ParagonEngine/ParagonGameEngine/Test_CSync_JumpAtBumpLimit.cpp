#include "Test_CSync_JumpAtBumpLimit.h"
#include "../ParagonData/DynamicCollider.h"

namespace Pg::Engine::BTree::Node
{
	void Test_CSync_JumpAtBumpLimit::InitCustom()
	{
		_collider = GetObject()->GetComponent<Pg::Data::DynamicCollider>();
		assert(_collider != nullptr);
	}

	BT::NodeStatus Test_CSync_JumpAtBumpLimit::tick()
	{
		unsigned int tVal;
		getInput<unsigned int>("_jumpCount", tVal);

		if (tVal == 5)
		{
			_collider->AddForce({ 0.0f, 10.0f ,0.0f }, Pg::Data::ForceMode::eIMPULSE);
		}
	}
}