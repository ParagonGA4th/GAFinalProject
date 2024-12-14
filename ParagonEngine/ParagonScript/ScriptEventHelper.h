#pragma once
#include <functional>
#include <string>
#include <variant>
#include "IEvent.h"

#include "../ParagonData/Transform.h"
#include "../ParagonData/GameObject.h"

/// <summary>
/// Event�� ����ϴ� used type�� �� ���� Ȱ���� �� �ֱ� ����.
/// ũ�� Transform���� Ȱ���� �� �ְ� �������.
/// </summary>

using UsedVariant = std::variant<int, float, bool, std::string, Pg::Data::Transform*, Pg::Data::GameObject*>;
using SlotType = std::function<void(const Pg::DataScript::IEvent&, UsedVariant, UsedVariant)>;


