#pragma once
#include "ScriptInterface.h"
#include "IConfinedArea.h"
#include "../ParagonMath/PgMath.h"

#include <visit_struct/visit_struct.hpp>

namespace Pg::Data { class Collider; class StaticBoxCollider; }
namespace Pg::API::Time { class PgTime; }

namespace Pg::DataScript
{
	class PlayerMovementSector;
	class PlayerHandler;

	class TrapArea : public ScriptInterface<TrapArea>, public IConfinedArea
	{
		DEFINE_PARAGON_SCRIPT(TrapArea);

	public:
		TrapArea(Pg::Data::GameObject* obj);

		virtual void Awake() override;
		virtual void Start() override;
		virtual void Update() override;

	public:
		virtual void ResetAll() override;
		virtual unsigned int GetDesignatedAreaIndex() override;
		virtual void SetActivate(bool val) override { _isActivated = val; }
	public:
		//Private이면 안된다.
		virtual void OnTriggerEnter(Pg::Data::Collider** _colArr, unsigned int count) override;
		virtual void OnTriggerExit(Pg::Data::Collider** _colArr, unsigned int count) override;

	public:
		BEGIN_VISITABLES(TrapArea);
		VISITABLE(int, _areaIndex);
		END_VISITABLES;

	private:
		Pg::API::Time::PgTime* _deltaTime;
		Pg::Data::StaticBoxCollider* _collider;
		//Pg::DataScript::PlayerMovementSector* _playerMovement;
		Pg::DataScript::PlayerHandler* _playerBattleBehavior;

		// 플레이어의 기본 속도
		float _previousMoveSpeed;
		float _damage = 0.2f;	 // 체력 감소 속도	

		bool _onTriggerStay;
		
	private:
		bool _isActivated{ true };
	};
}

