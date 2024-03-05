#pragma once
#include "../ParagonMath/PgMath.h"
#include <string>

/// <summary>
/// BlackBoard Instance를 공유하는 것 말고도 Blackboard끼리 정보를 공유하기 위해.
/// </summary>

namespace Pg::Data::BTree
{
	struct BTreeShareData
	{

		//이런 식으로 들어간다. Ex.
		Pg::Math::PGFLOAT3 _playerPosition;
	};
}
