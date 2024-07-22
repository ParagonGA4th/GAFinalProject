#pragma once
#include "ScriptInterface.h"
#include "../ParagonData/GameObject.h"

namespace Pg::Data
{
	class StaticBoxCollider;
}

namespace Pg::DataScript
{
	class BridgeHelper : public ScriptInterface<BridgeHelper>
	{
		DEFINE_PARAGON_SCRIPT(BridgeHelper);

	public:
		BridgeHelper(Pg::Data::GameObject* obj);

		virtual void BeforePhysicsAwake() override;
		virtual void Awake() override;
		virtual void Update() override;

		virtual void OnTriggerEnter(Pg::Data::Collider** _colArr, unsigned int count) override;

	public:
		bool _isTrig{ false };
	};
}


