#include "Test_CSync_ReturnToCenter.h"
#include "BTHelper.h"
#include "../ParagonData/Transform.h"

namespace Pg::Engine::BTree::Node
{
	BT::NodeStatus Test_CSync_ReturnToCenter::tick()
	{
		//탑 다운 관점. X / Z 의미.
		Pg::Math::PGFLOAT2 tLT;
		Pg::Math::PGFLOAT2 tRB;
		BTHelper::Validate(getInput<Pg::Math::PGFLOAT2>("_LT", tLT));
		BTHelper::Validate(getInput<Pg::Math::PGFLOAT2>("_RB", tRB));

		float tMidX = (tLT.x + tRB.x) / 2.0f;
		float tMidZ = (tLT.y + tRB.y) / 2.0f;

		GetObject()->_transform._position = { tMidX, 0.f, tMidZ };

		//항상 성공할 수밖에 없는 위치.
		return BT::NodeStatus::SUCCESS;
	}
}
