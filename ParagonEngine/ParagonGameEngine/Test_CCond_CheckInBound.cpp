#include "Test_CCond_CheckInBound.h"
#include "BTHelper.h"

namespace Pg::Engine::BTree::Node
{
	BT::NodeStatus Test_CCond_CheckInBound::tick()
	{
		//탑 다운 관점. X / Z 의미.
		Pg::Math::PGFLOAT2 tRB = { 30.f, 30.f };
		Pg::Math::PGFLOAT2 tLT = { 50.f, 50.f };

		//단순히 Position이 위치 안에 있는지 검사.
		Pg::Math::PGFLOAT3 tPos;

		BTHelper::Validate(getInput<Pg::Math::PGFLOAT3>("_currentPos", tPos));

		bool tIn = false;
		if (tPos.x > tRB.x && tPos.z > tRB.y &&
			tPos.x < tLT.x && tPos.z < tLT.y)
		{
			tIn = true;
		}

		BTHelper::Validate(setOutput<bool>("_isInBound", tIn));

	}

}