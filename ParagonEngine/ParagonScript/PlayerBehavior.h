#pragma once
#include "ScriptInterface.h"

namespace Pg::Data
{
	class Camera;
	class DynamicCollider;
}

namespace Pg::DataScript
{
	class PlayerBehavior : public ScriptInterface<PlayerBehavior>
	{
		DEFINE_PARAGON_SCRIPT(PlayerBehavior);

	public:
		PlayerBehavior(Pg::Data::GameObject* obj);

		virtual void Awake() override;
		virtual void Start() override;
		virtual void Update() override;

	private:
		Pg::Data::Camera* mainCam{ nullptr };
		Pg::Data::DynamicCollider* selfCol{ nullptr };


	};
}



