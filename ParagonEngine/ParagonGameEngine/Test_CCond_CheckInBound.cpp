#include "Test_CCond_CheckInBound.h"
#include "BTHelper.h"

namespace Pg::Engine::BTree::Node
{
	using Pg::Engine::BTree::BTHelper;

	BT::NodeStatus Test_CCond_CheckInBound::tick()
	{
		//탑 다운 관점. X / Z 의미.
		Pg::Math::PGFLOAT2 tLT;
		Pg::Math::PGFLOAT2 tRB;
		BTHelper::Validate(getInput<Pg::Math::PGFLOAT2>("_LT", tLT));
		BTHelper::Validate(getInput<Pg::Math::PGFLOAT2>("_RB", tRB));

		//단순히 Position이 위치 안에 있는지 검사.
		Pg::Math::PGFLOAT3 tPos;
		BTHelper::Validate(getInput<Pg::Math::PGFLOAT3>("_currentPos", tPos));

		bool tIn = false;
		if (tPos.x > tLT.x && tPos.z > tLT.y &&
			tPos.x < tRB.x && tPos.z < tRB.y)
		{
			tIn = true;
		}

		BTHelper::Validate(setOutput<bool>("_isInBound", tIn));

	}

}