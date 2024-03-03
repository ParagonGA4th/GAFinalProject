#include "Test_CSync_ChooseNewDir.h"
#include "BTHelper.h"
#include <cstdlib>

namespace Pg::Engine::BTree::Node
{
	using Pg::Engine::BTree::BTHelper;

	BT::NodeStatus Test_CSync_ChooseNewDir::tick()
	{
		//0-RAND_MAX까지의 하드한 랜덤 값을 받는다.
		int val1 = rand();
		int val2 = rand();

		val1 %= 360;
		val2 %= 360;

		//-180~180까지 만듬.
		val1 -= 180;
		val2 -= 180;

		//랜덤한 값을 가진 값 배출.
		Pg::Math::PGFLOAT3 tOut = { Pg::Math::PGConvertToRadians((float)val1), 
									0.f, 
									Pg::Math::PGConvertToRadians((float)val2)};

		//방향으로 쓰일 것이기에, 정규화.
		tOut = Pg::Math::PGFloat3Normalize(tOut);

		BTHelper::Validate(setOutput<Pg::Math::PGFLOAT3>("_moveDir", tOut));
	}
}