#pragma once
#include "ScriptInterface.h"

namespace Pg::Data { class StaticBoxCollider; }
namespace Pg::API { class PgScene; }

namespace Pg::DataScript
{
	class Portal : public ScriptInterface<Portal>
	{
		DEFINE_PARAGON_SCRIPT(Portal);

	public:
		Portal(Pg::Data::GameObject* obj);

		virtual void Awake() override;
		virtual void Start() override;
		virtual void Update() override;

	private:
		virtual void OnTriggerEnter(Pg::Data::Collider* col) override;

	private:
		Pg::Data::StaticBoxCollider* _collider;
		Pg::API::PgScene* _sceneHelper;
	};
}

