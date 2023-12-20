#pragma once
#include "DynamicCollider.h"
#include "../ParagonMath/PgMath.h"
#include "../ParagonData/DebugData.h"

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
		virtual void Start() override;
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

	private:
		float _radius;
		float _halfHeight;
	};
}

