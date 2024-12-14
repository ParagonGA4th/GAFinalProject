#pragma once
#include <functional>
#include <string>
#include <variant>
#include "IEvent.h"

#include "../ParagonData/Transform.h"
#include "../ParagonData/GameObject.h"

/// <summary>
/// Event를 사용하는 used type을 더 쉽게 활용할 수 있기 위해.
/// 크게 Transform까지 활용할 수 있게 만들었다.
/// </summary>

using UsedVariant = std::variant<int, float, bool, std::string, Pg::Data::Transform*, Pg::Data::GameObject*>;
using SlotType = std::function<void(const Pg::DataScript::IEvent&, UsedVariant, UsedVariant)>;


