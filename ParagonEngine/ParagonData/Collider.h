#pragma once
#include "Component.h"
#include "GameObject.h"

/// <summary>
/// 변지상의 Collider 컴포넌트
/// 2D와 3D에 플래그 적용하여 오브젝트에 맞는 Collider 적용할 예정.
/// 근데 일단 PhysX부터 어찌.....
/// 2023.10.17
/// </summary>

namespace PG::Data
{
	class GameObject;
}

namespace Pg::Data
{
	class Collider : public Component
	{
	public:
		Collider();

	private:
	};
}

