#include "Test_CCond_CheckInBound.h"
#include "../BTHelper.h"
#include "../ParagonUtil/Log.h"
namespace Pg::Data::BTree::Node
{
	using Pg::Data::BTree::BTHelper;

	BT::NodeStatus Test_CCond_CheckInBound::tick()
	{
		//PG_TRACE(typeid(this).name());

		//탑 다운 관점. X / Z 의미.
		auto tLT = getInput<Pg::Math::PGFLOAT2>("_LT");
		auto tRB = getInput<Pg::Math::PGFLOAT2>("_RB");
		BT_VALIDATE(tLT);
		BT_VALIDATE(tRB);

		//단순히 Position이 위치 안에 있는지 검사.
		auto tPos = getInput<Pg::Math::PGFLOAT3>("_currentPos");
		BT_VALIDATE(tPos);

		//Fallback Node에 의해 제어받을 것. 일단 Failure
		BT::NodeStatus tStat = BT::NodeStatus::FAILURE;

		if (tPos.value().x > tLT.value().x && tPos.value().z > tLT.value().y &&
			tPos.value().x < tRB.value().x && tPos.value().z < tRB.value().y)
		{
			//InBound
			tStat = BT::NodeStatus::SUCCESS;
		}

		return tStat;
	}

}


//BT_REGISTER_FACTORY();