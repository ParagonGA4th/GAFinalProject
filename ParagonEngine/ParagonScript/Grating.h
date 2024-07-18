#pragma once
#include "ScriptInterface.h"
#include "../ParagonData/GameObject.h"

namespace Pg::Data
{
	class StaticMeshRenderer;
}

namespace Pg::DataScript
{
	class Grating : public ScriptInterface<Grating>
	{
		DEFINE_PARAGON_SCRIPT(Grating);

	public:
		Grating(Pg::Data::GameObject* obj);

		virtual void GrabManagedObjects() override;

		virtual void BeforePhysicsAwake() override;
		virtual void Awake() override;
		virtual void Update() override;

	private:
		Pg::Data::StaticMeshRenderer* _renderers;

		inline static const float ALPHA_PERCENT = 1.0f;
	};
}


