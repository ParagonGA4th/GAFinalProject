#pragma once
#include "DynamicCollider.h"
#include "../ParagonData/DebugData.h"
#include <visit_struct/visit_struct.hpp>

namespace Pg::Data
{
	class GameObject;

	class SphereCollider : public DynamicCollider
	{
	public:
		SphereCollider(GameObject* owner);
		
		virtual void OnSerialize(SerializeVector& sv);
		virtual void OnDeserialize(SerializeVector& sv);

	public:
		virtual void Update() override;
		
		virtual float GetWidth() const override;
		virtual float GetHeight() const override;
		virtual float GetDepth() const override;

	public:
		float GetRadius() const;
		void SetRadius(float rad);

		Pg::Data::SphereInfo _sphereInfo;

	public:
		BEGIN_VISITABLES(SphereCollider);
		VISITABLE(float, _rad);
		END_VISITABLES;
	};
}

