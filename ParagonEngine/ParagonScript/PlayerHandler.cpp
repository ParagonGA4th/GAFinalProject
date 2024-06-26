#include "PlayerHandler.h"
#include "CombatSystem.h"
#include "ComboSystem.h"
#include "ArrowLogic.h"
#include "PlayerMovementSector.h"
#include "PlayerCombatSector.h"

#include "../ParagonData/Scene.h"
#include "../ParagonData/LayerMask.h"
#include "../ParagonData/DynamicCollider.h"
#include "../ParagonData/SkinnedMeshRenderer.h"
#include "../ParagonData/AudioSource.h"
#include "../ParagonAPI/PgInput.h"
#include "../ParagonAPI/PgTime.h"
#include "../ParagonUtil/Log.h"

//일어날 수 있는 Event들의 리스트
#include "EventList_PlayerRelated.h"

#include <singleton-cpp/singleton.h>

#include <algorithm>
#include <limits>

namespace Pg::DataScript
{
	PlayerHandler::PlayerHandler(Pg::Data::GameObject* obj) : ScriptInterface(obj)
	{
		_playerMovementSector = std::make_unique<PlayerMovementSector>(this);
		_playerCombatSector = std::make_unique<PlayerCombatSector>(this);
	}

	void PlayerHandler::GrabManagedObjects()
	{
		//개별적으로 함수 실행.
		_playerMovementSector->GrabManagedObjects();
		_playerCombatSector->GrabManagedObjects();
	}

	void PlayerHandler::BeforePhysicsAwake()
	{
		//무조건 생성자는 안됨! -> AddComponent에서 만들어진 다음에는, Static Variable Initialization에 따라 재생성되지 않는다.
		//CombatSystem은 TitleScene에 들어가야 할 것.
		_combatSystem = CombatSystem::GetInstance(nullptr);
		_comboSystem = ComboSystem::GetInstance();

		//개별적으로 함수 실행.
		_playerMovementSector->BeforePhysicsAwake();
		_playerCombatSector->BeforePhysicsAwake();
	}

	void PlayerHandler::Awake()
	{
		_playerMovementSector->Awake();
		_playerCombatSector->Awake();
	}

	void PlayerHandler::Start()
	{
		_comboSystem->SystemStart();

		_playerMovementSector->Start();
		_playerCombatSector->Start();
	}

	void PlayerHandler::Update()
	{
		_comboSystem->SystemUpdate();

		_playerMovementSector->Update();
		_playerCombatSector->Update();
	}

	void PlayerHandler::FixedUpdate()
	{
		_playerMovementSector->FixedUpdate();
		_playerCombatSector->FixedUpdate();
	}

	void PlayerHandler::LateUpdate()
	{
		_playerMovementSector->LateUpdate();
		_playerCombatSector->LateUpdate();
	}

	void PlayerHandler::OnAnimationEnd()
	{
		_playerMovementSector->OnAnimationEnd();
		_playerCombatSector->OnAnimationEnd();
	}

	void PlayerHandler::HandleEvents(const IEvent& e, UsedVariant usedVar1, UsedVariant usedVar2)
	{
		if (e.GetIdentifier() == Event_PlayerDeath::_identifier);
		{
			//여러 개의 이벤트들이 한꺼번에 핸들링 될 경우, 이렇게 활용됨. 
			//const Event_PlayerDeath& demoEvent = static_cast<const Event_PlayerDeath&>(e);



		}
	}

	void PlayerHandler::OnTriggerEnter(Pg::Data::Collider** _colArr, unsigned int count)
	{
		for (int i = 0; i < count; i++)
		{
			Pg::Data::Collider* col = _colArr[i];

			PG_TRACE("HIT : PLAYER");
		}
	}

	void PlayerHandler::ChangePlayerHealth(float level)
	{
		healthPoint = std::clamp<float>(healthPoint + level, 0.0f, MAX_PLAYER_HEALTH);

		if (healthPoint > std::numeric_limits<float>::epsilon() &&
			healthPoint < PLAYER_HEALTH_LOW_BARRIER)
		{
			_combatSystem->Post(Event_PlayerOnLowHealth(), _object, healthPoint);
		}
		else if (healthPoint < std::numeric_limits<float>::epsilon())
		{
			//만약 체력이 0일 경우, 
			_combatSystem->Post(Event_PlayerDeath(), _object, 0.0f);
		}

	}

	void PlayerHandler::ChangePlayerMana(float level)
	{
		manaPoint = std::clamp<float>(manaPoint + level, 0.0f, MAX_PLAYER_MANA);
	}

	void PlayerHandler::ChangePlayerStamina(float level)
	{
		staminaPoint = std::clamp<float>(staminaPoint + level, 0.0f, MAX_PLAYER_STAMINA);
	}

	void PlayerHandler::ResetAll()
	{
		//assert(false && "not implemented yet");
		_playerCombatSector->ResetAll();
		_playerMovementSector->ResetAll();

	}

	void PlayerHandler::SetPlayerMoveSpeed(float val)
	{
		_playerMovementSector->moveSpeed = val;
	}

	float PlayerHandler::GetPlayerMoveSpeed()
	{
		return _playerMovementSector->moveSpeed;
	}

	PlayerMovementSector* PlayerHandler::GetPlayerMovementSector()
	{
		return _playerMovementSector.get();
	}

	PlayerCombatSector* PlayerHandler::GetPlayerCombatSector()
	{
		return _playerCombatSector.get();
	}

	const float* PlayerHandler::ReturnPlayerHealthPointPointerConst() const
	{
		return &healthPoint;
	}

	const float* PlayerHandler::ReturnPlayerManaPointPointerConst() const
	{
		return &manaPoint;
	}

	const float* PlayerHandler::ReturnPlayerStaminaPointPointerConst() const
	{
		return &staminaPoint;
	}
	

}