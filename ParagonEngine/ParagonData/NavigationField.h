#pragma once
#include "Component.h"

/// <summary>
/// Recast Navigation이 적용될 필드.
/// 아마 PlaneCollider 객체에만 들어갈 것 같다.
/// </summary>
namespace Pg::Data
{
	class GameObject;

	class NavigationField : public Component
	{
	public:
		NavigationField(GameObject* owner);

		virtual void Start() override;
		virtual void Update() override;
	};
}

