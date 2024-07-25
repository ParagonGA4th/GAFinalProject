#pragma once
#include "ScriptInterface.h"
#include "../ParagonData/GameObject.h"

namespace Pg::Data
{
	class StaticMeshRenderer;
	class StaticBoxCollider;
	class Collider;
}

namespace Pg::DataScript
{
	class Grating_1 : public ScriptInterface<Grating_1>
	{
		DEFINE_PARAGON_SCRIPT(Grating_1);

	public:
		Grating_1(Pg::Data::GameObject* obj);

		virtual void GrabManagedObjects() override;

		virtual void BeforePhysicsAwake() override;
		virtual void Awake() override;
		virtual void Update() override;

	private:
		Pg::Data::StaticMeshRenderer* _renderer;
		Pg::Data::StaticBoxCollider* _collider;
		Pg::Data::Collider* _otherCol;

		inline static const float ALPHA_PERCENT = 0.8f;
	};
}


