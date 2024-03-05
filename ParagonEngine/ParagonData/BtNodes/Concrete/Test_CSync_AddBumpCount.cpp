#include "Test_CSync_AddBumpCount.h"
#include "../ParagonUtil/Log.h"
#include "../BTHelper.h"
namespace Pg::Data::BTree::Node
{
	BT::NodeStatus Test_CSync_AddBumpCount::tick()
	{
		//PG_TRACE(typeid(this).name());

		//초기값은 0이어야 한다.
		auto tVal = getInput<int>("_jumpCount");
		BT_VALIDATE(tVal);

		int tValue = tVal.value() + 1;

		//5 이상이면 0으로 리셋.
		if (tVal.value() > 5)
		{
			tValue = 0;
		}

		setOutput<int>("_jumpCount", tValue);

		return BT::NodeStatus::SUCCESS;
	}
}