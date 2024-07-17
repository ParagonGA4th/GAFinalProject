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
	class Bridge : public ScriptInterface<Bridge>
	{
		DEFINE_PARAGON_SCRIPT(Bridge);

	public:
		Bridge(Pg::Data::GameObject* obj);

		virtual void GrabManagedObjects() override;

		virtual void BeforePhysicsAwake() override;
		virtual void Awake() override;
		virtual void Update() override;

	private:
		std::vector<Pg::Data::StaticMeshRenderer*> _renderers;
		Pg::Data::StaticBoxCollider* _collider;

		inline static const float ALPHA_PERCENT = 1.f;
	};
}


