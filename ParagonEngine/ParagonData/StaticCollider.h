#pragma once
#include "Collider.h"

/// <summary>
/// Rigid가 적용되지 않는 Collider.
/// 아마 Box와 Plane정도만 구현하지 않을까....
/// 2023.12.11
/// </summary>
namespace Pg::Data
{
	class GameObject;

	class StaticCollider : public Collider
	{
	public:
		StaticCollider(GameObject* owner);

		virtual void UpdateTransform() override;

	private:
		bool _isCollide;
	};
}

