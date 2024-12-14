#pragma once
#include "ScriptInterface.h"
#include <string>

namespace Pg::Data { class BoxCollider; }
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

		void SetNextScene(std::string nextScene);

	private:
		virtual void OnTriggerEnter(Pg::Data::Collider** _colArr, unsigned int count) override;

	private:
		Pg::Data::BoxCollider* _collider;
		Pg::API::PgScene* _sceneHelper;

		std::string _nextScene;
	};
}

