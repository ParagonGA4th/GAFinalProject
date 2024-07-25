#pragma once
#include "ScriptInterface.h"
#include "../ParagonData/GameObject.h"

namespace Pg::Data
{
	class StaticMeshRenderer;
	class StaticBoxCollider;
}

namespace Pg::DataScript
{
	class PlayerHandler;
	class Grating_2 : public ScriptInterface<Grating_2>
	{
		DEFINE_PARAGON_SCRIPT(Grating_2);

	public:
		Grating_2(Pg::Data::GameObject* obj);

		virtual void GrabManagedObjects() override;

		virtual void BeforePhysicsAwake() override;
		virtual void Awake() override;
		virtual void Update() override;

	private:
		Pg::Data::StaticMeshRenderer* _renderer;
		Pg::Data::StaticBoxCollider* _collider;

		PlayerHandler* _playerHandler{ nullptr };

		inline static const float ALPHA_PERCENT = 0.8f;
	};
}


