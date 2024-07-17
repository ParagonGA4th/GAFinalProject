#pragma once
#include "ScriptInterface.h"
#include "../ParagonData/GameObject.h"

namespace Pg::Data 
{
	class StaticMeshRenderer;
	class StaticBoxCollider;
	class SphereCollider;
}

namespace Pg::DataScript
{
	class Stair : public ScriptInterface<Stair>
	{
		DEFINE_PARAGON_SCRIPT(Stair);

	public:
		Stair(Pg::Data::GameObject* obj);

		virtual void GrabManagedObjects() override;

		virtual void BeforePhysicsAwake() override;
		virtual void Awake() override;
		virtual void Update() override;

	private:
		Pg::Data::StaticMeshRenderer* _renderer;
		Pg::Data::StaticBoxCollider* _collider;
		Pg::Data::SphereCollider* _area;

		inline static const float ALPHA_PERCENT = 1.f;
	};
}


