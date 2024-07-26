#pragma once
#include "ScriptInterface.h"
#include "IConfinedArea.h"
#include "../ParagonMath/PgMath.h"

#include <visit_struct/visit_struct.hpp>

namespace Pg::Data 
{ 
	class Collider; 
	class GameObject;
	class SphereCollider;
	class ImageRenderer;
}

namespace Pg::API
{
	namespace Time
	{
		class PgTime;
	}
}

namespace Pg::DataScript
{
	class CombatSystem;
	class PlayerHandler;
	class BaseEnemyHandler;
	class BaseAreaHandler;

	class BattleArea : public ScriptInterface<BattleArea>, public IConfinedArea
	{
		DEFINE_PARAGON_SCRIPT(BattleArea);

	public:
		BattleArea(Pg::Data::GameObject* obj);

		virtual void OnDeserialize(SerializeVector& sv) override;
		virtual void OnSerialize(SerializeVector& sv) override;

		virtual void GrabManagedObjects() override;
		virtual void Awake() override;
		virtual void Start() override;
		virtual void Update() override;
	public:
		virtual void ResetAll() override;
		virtual unsigned int GetDesignatedAreaIndex() override;
		virtual void SetActivate(bool val) override;

	public:
		virtual void OnTriggerEnter(Pg::Data::Collider** _colArr, unsigned int count) override;
		virtual void OnTriggerExit(Pg::Data::Collider** _colArr, unsigned int count) override;

	private:
		void ConfinePlayer();
		void ConfineBoss();
		void DeactivateArea();
		
	public:
		BEGIN_VISITABLES(BattleArea);
		VISITABLE(int, _areaIndex);
		END_VISITABLES;

	private:
		Pg::Data::SphereCollider* _collider;
		Pg::API::Time::PgTime* _pgTime{ nullptr };
		Pg::DataScript::PlayerHandler* _player;
		Pg::Data::GameObject* _boss;
		Pg::Data::ImageRenderer* _killMonsterUI;
		Pg::Data::ImageRenderer* _golemBossUI;

		//int _monster = 1;
		bool _onTriggerStay{ false };

	private:
		//자신이 속해 있는 씬의 기타 핸들러들.
		BaseAreaHandler* _areaHandler{ nullptr };
		BaseEnemyHandler* _enemyHandler{ nullptr };
		CombatSystem* _combatSystem{ nullptr };

	private:
		bool _isActivated{ true };
		bool _isUI{ true };
		float dTime = 0.f;

	};
}

