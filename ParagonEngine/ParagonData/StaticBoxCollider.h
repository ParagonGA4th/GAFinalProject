#pragma once
#include "StaticCollider.h"
#include "DebugData.h"
#include "../ParagonMath/PgMath.h"

#include <visit_struct/visit_struct.hpp>
namespace Pg::Data
{
	class GameObject;

	class StaticBoxCollider : public StaticCollider
	{
	public:
		StaticBoxCollider(GameObject* owner);

		virtual void OnSerialize(SerializeVector& sv);
		virtual void OnDeserialize(SerializeVector& sv);
		
	public:
		virtual void Update() override;

		float GetWidth() const;
		float GetHeight() const;
		float GetDepth() const;

	public:
		void SetScale(float w, float h, float d);

		Pg::Data::BoxInfo _boxInfo;

	public:
		BEGIN_VISITABLES(StaticBoxCollider);
		VISITABLE(float, _width);
		VISITABLE(float, _height);
		VISITABLE(float, _depth);
		END_VISITABLES;
	};
}

