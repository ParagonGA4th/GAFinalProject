#include "Test_CCond_CheckInBound.h"
#include "BTHelper.h"
#include "../ParagonUtil/Log.h"
namespace Pg::Engine::BTree::Node
{
	using Pg::Engine::BTree::BTHelper;

	BT::NodeStatus Test_CCond_CheckInBound::tick()
	{
		//PG_TRACE(typeid(this).name());

		//탑 다운 관점. X / Z 의미.
		Pg::Math::PGFLOAT2 tLT;
		Pg::Math::PGFLOAT2 tRB;
		BTHelper::Validate(getInput<Pg::Math::PGFLOAT2>("_LT", tLT));
		BTHelper::Validate(getInput<Pg::Math::PGFLOAT2>("_RB", tRB));

		//단순히 Position이 위치 안에 있는지 검사.
		Pg::Math::PGFLOAT3 tPos;
		BTHelper::Validate(getInput<Pg::Math::PGFLOAT3>("_currentPos", tPos));

		//Fallback Node에 의해 제어받을 것. 일단 Failure
		BT::NodeStatus tStat = BT::NodeStatus::FAILURE;

		if (tPos.x > tLT.x && tPos.z > tLT.y &&
			tPos.x < tRB.x && tPos.z < tRB.y)
		{
			//InBound
			tStat = BT::NodeStatus::SUCCESS;
		}

		return tStat;
	}

}


//BT_REGISTER_FACTORY();