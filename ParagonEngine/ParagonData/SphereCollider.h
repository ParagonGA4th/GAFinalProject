#pragma once
#include "DynamicCollider.h"
#include "../ParagonData/DebugData.h"

#include <auto_register/factory.h>

namespace Pg::Data
{
	class GameObject;

	class SphereCollider : public DynamicCollider, RegisteredInFactory<DynamicCollider, SphereCollider, GameObject*>
	{
	public:
		SphereCollider(GameObject* owner);

		//자동화된 Auto-Registration 작동 위해 필수.
		static DynamicCollider* CreateInstance(GameObject* go) { return new SphereCollider(go); }
		static const char* GetFactoryKey() { return "class Pg::Data::SphereCollider"; }

	public:
		virtual void Update() override;
		
		virtual float GetWidth() const override;
		virtual float GetHeight() const override;
		virtual float GetDepth() const override;

	public:
		float GetRadius() const;
		void SetRadius(float rad);

		Pg::Data::SphereInfo _sphereInfo;

	private:
		float _rad;
	};
}

