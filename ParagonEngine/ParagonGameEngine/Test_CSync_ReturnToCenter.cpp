#include "Test_CSync_ReturnToCenter.h"
#include "BTHelper.h"
#include "../ParagonData/Transform.h"
#include "../ParagonUtil/Log.h"

namespace Pg::Engine::BTree::Node
{
	BT::NodeStatus Test_CSync_ReturnToCenter::tick()
	{
		//PG_TRACE(typeid(this).name());

		//탑 다운 관점. X / Z 의미.
		//Pg::Math::PGFLOAT2 tLT;
		//Pg::Math::PGFLOAT2 tRB;
		auto tLT = getInput<Pg::Math::PGFLOAT2>("_LT");
		auto tRB = getInput<Pg::Math::PGFLOAT2>("_RB");
		BT_VALIDATE(tLT);
		BT_VALIDATE(tRB);

		float tMidX = (tLT.value().x + tRB.value().x) / 2.0f;
		float tMidZ = (tLT.value().y + tRB.value().y) / 2.0f;

		GetGameObject()->_transform._position = { tMidX, GetGameObject()->_transform._position.y, tMidZ };

		//항상 성공할 수밖에 없는 위치.
		return BT::NodeStatus::SUCCESS;
	}
}
