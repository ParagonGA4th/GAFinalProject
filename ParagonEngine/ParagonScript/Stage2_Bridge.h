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
	class BridgeHelper;
	class Stage2_Bridge : public ScriptInterface<Stage2_Bridge>
	{
		DEFINE_PARAGON_SCRIPT(Stage2_Bridge);

	public:
		Stage2_Bridge(Pg::Data::GameObject* obj);

		virtual void GrabManagedObjects() override;

		virtual void BeforePhysicsAwake() override;
		virtual void Awake() override;
		virtual void Update() override;

	private:
		std::vector<Pg::Data::StaticMeshRenderer*> _renderers;
		Pg::Data::StaticBoxCollider* _collider;
		Pg::Data::SphereCollider* _area;

		Pg::DataScript::BridgeHelper* _bridgeHelper;

		inline static const float ALPHA_PERCENT = 1.0f;
		inline static const float POSITION_PERCENT = 0.3f;

		int _index = 0;
		float _rollbackPos = 3.513f;
	};
}


