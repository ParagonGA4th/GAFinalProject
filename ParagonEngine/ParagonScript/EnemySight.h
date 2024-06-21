#pragma once
#include "ScriptInterface.h"

namespace Pg::Data
{
	class Collider;
	class BoxCollider;
	class StaticCollider;
	class StaticBoxCollider;
}

namespace Pg::DataScript
{
	class PlayerHandler;

	class EnemySight : public ScriptInterface<EnemySight>
	{
		DEFINE_PARAGON_SCRIPT(EnemySight);

	public:
		EnemySight(Pg::Data::GameObject* obj);

	public:
		virtual void Start() override;
		virtual void Update() override;

		virtual void OnTriggerEnter(Pg::Data::Collider* c);
		virtual void OnTriggerExit(Pg::Data::Collider* c);

		bool _playerDetected = false;

	private:
		PlayerHandler* _playerBattleBehavior;
		//Pg::Data::StaticCollider* _collider;
		Pg::Data::BoxCollider* _collider;

	};
}