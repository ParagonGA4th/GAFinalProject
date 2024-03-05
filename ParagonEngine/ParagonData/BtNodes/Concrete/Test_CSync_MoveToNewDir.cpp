#include "Test_CSync_MoveToNewDir.h"
#include "../BTHelper.h"
#include "../ParagonUtil/Log.h"
#include <cassert>

namespace Pg::Data::BTree::Node
{
	using Pg::Data::BTree::BTHelper;

	void Test_CSync_MoveToNewDir::InitCustom()
	{
		config().blackboard->set<Pg::Math::PGFLOAT3>("MOVEDIR", {0.f,0.f,1.f});
	}

	BT::NodeStatus Test_CSync_MoveToNewDir::tick()
	{
		//PG_TRACE(typeid(this).name());

		auto tMoveDir = getInput<Pg::Math::PGFLOAT3>("_moveDir");
		BT_VALIDATE(tMoveDir);

		//현재 방향을 받았으니, 해당 방향으로 이동.
		Pg::Math::PGFLOAT3 POS = GetGameObject()->_transform._position;
		float DISTANCE = 0.1f;
		//float DISTANCE = 0.1f; // 임시로 거리를 설정함 (distance * camera speed * deltaTime)

		Pg::Math::PGFLOAT3 LOOK = { tMoveDir->x * DISTANCE, tMoveDir->y * DISTANCE, tMoveDir->z * DISTANCE };
		POS += LOOK;

		GetGameObject()->_transform._position = POS;

		return BT::NodeStatus::SUCCESS;
	}
}
