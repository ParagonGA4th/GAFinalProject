#include "Test_CSync_MoveToNewDir.h"
#include "BTHelper.h"

namespace Pg::Engine::BTree::Node
{
	using Pg::Engine::BTree::BTHelper;

	BT::NodeStatus Test_CSync_MoveToNewDir::tick()
	{
		Pg::Math::PGFLOAT3 LOOK;
		BTHelper::Validate(getInput<Pg::Math::PGFLOAT3>("_moveDir", LOOK));

		//현재 방향을 받았으니, 해당 방향으로 이동.
		Pg::Math::PGFLOAT3 POS = GetObject()->_transform._position;
		float DISTANCE = 0.1f; // 임시로 거리를 설정함 (distance * camera speed * deltaTime)

		LOOK = { LOOK.x * DISTANCE, LOOK.y * DISTANCE, LOOK.z * DISTANCE };
		POS += LOOK;

		GetObject()->_transform._position = POS;
	}
}
