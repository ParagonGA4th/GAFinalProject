#pragma once
#include "../ParagonData/GameObject.h"
#include "IMovableObject.h"
#include "TransformSimpleStorage.h"

#include <string>
#include <array>

namespace Pg::DataScript
{
	struct MovingObjectAggregate
	{
		IMovableObject* _moveBehav = nullptr;
		Pg::Data::GameObject* _moveObj = nullptr;
		TransformSimpleStorage _transStorage;
	};
}
