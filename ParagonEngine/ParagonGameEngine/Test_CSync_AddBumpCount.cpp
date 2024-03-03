#include "Test_CSync_AddBumpCount.h"

namespace Pg::Engine::BTree::Node
{
	BT::NodeStatus Test_CSync_AddBumpCount::tick()
	{
		unsigned int tVal;
		//초기값은 0이어야 한다.
		getInput<unsigned int>("_jumpCount", tVal);
		tVal++;

		//5 이상이면 0으로 리셋.
		if (tVal > 5)
		{
			tVal = 0;
		}

		setOutput<unsigned int>("_jumpCount", tVal);
	}
}