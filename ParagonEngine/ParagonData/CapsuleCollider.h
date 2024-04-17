#pragma once
#include "DynamicCollider.h"
#include "../ParagonMath/PgMath.h"
#include "../ParagonData/DebugData.h"

#include <visit_struct/visit_struct.hpp>

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

		virtual void OnSerialize(SerializeVector& sv);
		virtual void OnDeserialize(SerializeVector& sv);

	protected:
		virtual void Update() override;

	public:
		virtual float GetWidth() const override;
		virtual float GetHeight() const override;
		virtual float GetDepth() const override;

		void SetRadius(float r);
		void SetHalfHeight(float h);
		float GetRadius() const;
		float GetHalfHeight() const;

		void SetCapsuleInfo(float rad, float halfHeight);

	public:
		Pg::Data::CapsuleInfo _capsuleInfo;

	public:
		BEGIN_VISITABLES(CapsuleCollider);
		VISITABLE(float, _radius);
		VISITABLE(float, _halfHeight);
		END_VISITABLES;
	};
}

