#pragma once
#include "DynamicCollider.h"

namespace Pg::Data
{
	class GameObject;

	class SphereCollider : public DynamicCollider
	{
	public:
		SphereCollider(GameObject* owner);

	public:
		virtual float GetWidth() const override;
		virtual float GetHeight() const override;
		virtual float GetDepth() const override;

	public:
		float GetRadius() const;
		void SetRadius(float rad);
	};
}

