#include "Test_CSync_AddBumpCount.h"

namespace Pg::Engine::BTree::Node
{
	BT::NodeStatus Test_CSync_AddBumpCount::tick()
	{
		int tVal;
		//초기값은 0이어야 한다.
		getInput<int>("_jumpCount", tVal);
		tVal++;

		//5 이상이면 0으로 리셋.
		if (tVal > 5)
		{
			tVal = 0;
		}

		setOutput<int>("_jumpCount", tVal);

		return BT::NodeStatus::SUCCESS;
	}
}