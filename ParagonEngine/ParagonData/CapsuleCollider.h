#pragma once
#include "DynamicCollider.h"
#include "../ParagonMath/PgMath.h"

namespace Pg::Data
{
	class GameObject;
}

namespace Pg::Data
{
	class CapsuleCollider : public DynamicCollider
	{
	public:
		CapsuleCollider(GameObject* owner);

	protected:
		virtual void Update() override;

	public:
		virtual float GetWidth() const override;
		virtual float GetHeight() const override;
		virtual float GetDepth() const override;

		float GetRadius() const;
		float GetHalfHeight() const;
	private:
		float _radius;
		float _halfHeight;
	};
}

