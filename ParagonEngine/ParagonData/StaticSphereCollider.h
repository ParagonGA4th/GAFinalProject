#pragma once
#include "StaticCollider.h"
#include "DebugData.h"
#include "../ParagonMath/PgMath.h"

#include <visit_struct/visit_struct.hpp>
namespace Pg::Data
{
	class GameObject;

	class StaticSphereCollider : public StaticCollider
	{
	public:
		StaticSphereCollider(GameObject* owner);

		virtual void OnSerialize(SerializeVector& sv);
		virtual void OnDeserialize(SerializeVector& sv);

	public:
		virtual void Update() override;

		virtual float GetWidth() const;
		virtual float GetHeight() const;
		virtual float GetDepth() const;

	public:
		float GetRadius() const;
		void SetRadius(float rad);

		Pg::Data::SphereInfo _sphereInfo;

	public:
		BEGIN_VISITABLES(StaticSphereCollider);
		VISITABLE(float, _rad);
		END_VISITABLES;
	};
}

