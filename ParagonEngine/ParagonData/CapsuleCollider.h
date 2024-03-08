#pragma once
#include "DynamicCollider.h"
#include "../ParagonMath/PgMath.h"
#include "../ParagonData/DebugData.h"

#include <auto_register/factory.h>

namespace Pg::Data
{
	class GameObject;
}

namespace Pg::Data
{
	class CapsuleCollider : public DynamicCollider, RegisteredInFactory<DynamicCollider, CapsuleCollider, GameObject*>
	{
	public:
		CapsuleCollider(GameObject* owner);

		static DynamicCollider* CreateInstance(GameObject* go) { return new CapsuleCollider(go); }
		static const char* GetFactoryKey() { return "class Pg::Data::CapsuleCollider"; }

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

	private:
		float _radius;
		float _halfHeight;
	};
}

