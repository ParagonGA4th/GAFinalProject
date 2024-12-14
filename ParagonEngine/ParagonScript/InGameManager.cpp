#include "InGameManager.h"
#include "PlayerHandler.h"
#include "PlayerMovementSector.h"

//#include "Portal.h"
#include "TotalGameManager.h"
#include "CombatSystem.h"

#include "EventList_PlayerRelated.h"
#include "EventList_GameFlowRelated.h"

#include "../ParagonData/Scene.h"
#include "../ParagonUtil/Log.h"

#include "../ParagonAPI/PgScene.h"
#include "../ParagonAPI/PgTime.h"
#include "../ParagonAPI/PgInput.h"

#include <algorithm>
#include <cassert>
#include <singleton-cpp/singleton.h>

namespace Pg::DataScript
{
	void InGameManager::Awake()
	{
		RegisterToCombatSystemEvents();
	}

	void InGameManager::Start()
	{

	}

	void InGameManager::Update()
	{
		if (_handlerBundle3D != nullptr)
		{
			// 3D일때만 작동하게 하는 것.
			// 내부적인 연동?
			// Update에서 Local Handler한테 지시하고,
			// LateUpdate 등에서 이를 뒤늦게 회수하는 방식도 고려 중.
			
			//3D 안에 있으니, 체력을 증가시키자.
			const float INCREASE_FACTOR = 0.4f;
			_combatSystem->ChangePlayerHealth(_pgTime->GetDeltaTime() * INCREASE_FACTOR);



		}
	}

	void InGameManager::FixedUpdate()
	{
		if (_handlerBundle3D != nullptr)
		{


		}
	}

	void InGameManager::LateUpdate()
	{
		if (_handlerBundle3D != nullptr)
		{



		}
	}

	void InGameManager::OnSceneChange_Global(Pg::Data::Scene* changedScene)
	{
		if (!_isManagingInitializeCalled)
		{
			Initialize(changedScene);
			_isManagingInitializeCalled = true;
		}

		//GameManager가 작동 : 무조건 3D Scene일 때만 관리해줘야.
		_isUpdating = (changedScene->GetIs3D()) ? true : false;

		// InGame에서 Scene이 바뀌었을 시 플레이어의 체력 등 모두 똑같이 이어져야함
		float _prevHealthPoint = 0.f;
		float _prevManaPoint = 0.f;
		int _prevStaminaPoint = 0;
		int _prevPlayerLife = 0;

		//Title Scene 등 2D Scene에서 넘어가는 경우의 수 FIX.
		bool tIsInfoTransferPossible = (_recordedPreviousScene != nullptr) && (_recordedPreviousScene->GetIs3D());

		//예전 시점.
		if (changedScene->GetIs3D() && tIsInfoTransferPossible)
		{
			_prevHealthPoint = _handlerBundle3D->_playerBehavior->healthPoint;
			_prevManaPoint = _handlerBundle3D->_playerBehavior->manaPoint;
			_prevStaminaPoint = _handlerBundle3D->_playerBehavior->staminaPoint;
			_prevPlayerLife = _handlerBundle3D->_playerBehavior->_playerlife;
		}
		else
		{
			_prevHealthPoint = PlayerHandler::MAX_PLAYER_HEALTH;
			_prevManaPoint = 0.f;
			_prevStaminaPoint = PlayerHandler::MAX_PLAYER_STAMINA;
			_prevPlayerLife = PlayerHandler::MAX_LIFE_COUNT;
		}

		//TotalGameManager가 무조건 앞에서 업데이트되었을 것이니, 
		//내부에서 업데이트된 HandlerBundle3D를 받아온다. 2D일 경우 nullptr.
		_handlerBundle3D = _totalGameManager->GetCurrentHandlerBundle();

		//바뀐 시점
		if (changedScene->GetIs3D())
		{
			_handlerBundle3D->_playerBehavior->healthPoint = _prevHealthPoint;
			_handlerBundle3D->_playerBehavior->manaPoint = _prevManaPoint;
			_handlerBundle3D->_playerBehavior->staminaPoint = _prevStaminaPoint;
			_handlerBundle3D->_playerBehavior->_playerlife = _prevPlayerLife;
		}

		//또한, GameState를 따로 설정하면서 관리.

		//다음 Switch를 위해 전에 속해 있던 Scene 기록.
		_recordedPreviousScene = changedScene;

		//점프 막아둠.
		if (_recordedPreviousScene->GetSceneName() == "BossStage")
		{
			_combatSystem->Post(Event_DisableJump(), NULL, NULL);
			_combatSystem->Post(Event_OnFinalBossGameAreaEnter(), NULL, NULL);
		}
		else if (_recordedPreviousScene->GetSceneName() == "Stage1")
		{
			_combatSystem->Post(Event_DisableJump(), NULL, NULL);
		}
		else if (_recordedPreviousScene->GetSceneName() == "Stage2")
		{
			_combatSystem->Post(Event_EnableJump(), NULL, NULL);
		}
	
	}

	void InGameManager::Initialize(Pg::Data::Scene* changedScene)
	{
		//API 값 받기.
		_pgScene = &singleton<Pg::API::PgScene>();
		_pgTime = &singleton<Pg::API::Time::PgTime>();
		_pgInput = &singleton<Pg::API::Input::PgInput>();

		// 반드시 해당 Object는 Don't Destroy On Load 설정이 되어 있어야 한다.
		assert(_object->GetDontDestroyOnLoad() && "XML에서 이렇게 들어왔어야 한다");

		//Total Game Manager 가져오기. 
		_totalGameManager = TotalGameManager::GetInstance(nullptr);
		assert(_totalGameManager != nullptr);

		_combatSystem = CombatSystem::GetInstance(nullptr);
		assert(_combatSystem != nullptr);
	}

	bool InGameManager::GetEnableGameManagerUpdate()
	{
		return _isUpdating;
	}

	void InGameManager::ChangeMonsterKillCount(int level)
	{
		_numberOfMonstersKilled = std::clamp<int>(_numberOfMonstersKilled + level, 0, MAX_MONSTER_COUNT_IN_LEVEL);
	}

	void InGameManager::RegisterToCombatSystemEvents()
	{
		InGameManager* bSelf = this;

		//글로벌 객체에서 등록하니.
		_combatSystem->Subscribe(Event_PlayerDeath::_identifier,
			std::bind(&InGameManager::HandleEvents, bSelf, std::placeholders::_1,
				std::placeholders::_2, std::placeholders::_3), true);

		_combatSystem->Subscribe(Event_OnFinalBossDeathGameWin::_identifier,
			std::bind(&InGameManager::HandleEvents, bSelf, std::placeholders::_1,
				std::placeholders::_2, std::placeholders::_3), true);

		_combatSystem->Subscribe(Event_EnableJump::_identifier,
			std::bind(&InGameManager::HandleEvents, bSelf, std::placeholders::_1,
				std::placeholders::_2, std::placeholders::_3), true);

		_combatSystem->Subscribe(Event_DisableJump::_identifier,
			std::bind(&InGameManager::HandleEvents, bSelf, std::placeholders::_1,
				std::placeholders::_2, std::placeholders::_3), true);

		_combatSystem->Subscribe(Event_OnGameOver::_identifier,
			std::bind(&InGameManager::HandleEvents, bSelf, std::placeholders::_1,
				std::placeholders::_2, std::placeholders::_3), true);
	}

	void InGameManager::HandleEvents(const IEvent& e, UsedVariant usedVar1, UsedVariant usedVar2)
	{
		//PG_ERROR("ENTERED");

		if (e.GetIdentifier() == Event_PlayerDeath::_identifier)
		{
			//여러 개의 이벤트들이 한꺼번에 핸들링 될 경우, 이렇게 활용됨. 
			//const Event_PlayerDeath& demoEvent = static_cast<const Event_PlayerDeath&>(e);
			PG_ERROR("PLAYER_INGAMEMANGER_HANDLE_EVENTS");

			// 현재 자신이 속해 있는 씬.
			Pg::Data::Scene* tBelongScene = _handlerBundle3D->_playerBehavior->_object->GetScene();

			// 전체 씬 리셋하기.
			TotalGameManager::GetInstance(nullptr)->CallForEntireSceneReset(tBelongScene, NULL, nullptr);

			//그다음에, Life 하나 깎기.
			_combatSystem->ChangePlayerLife(-1);
		}
		else if (e.GetIdentifier() == Event_OnFinalBossDeathGameWin::_identifier)
		{
			_pgScene->SetCurrentScene("TitleScene");
		}
		else if (e.GetIdentifier() == Event_EnableJump::_identifier)
		{
			TotalGameManager::GetInstance(nullptr)->GetCurrentHandlerBundle()->_playerBehavior->
				GetPlayerMovementSector()->SetIsAbleToJump(true);
		}
		else if (e.GetIdentifier() == Event_DisableJump::_identifier)
		{
			TotalGameManager::GetInstance(nullptr)->GetCurrentHandlerBundle()->_playerBehavior->
				GetPlayerMovementSector()->SetIsAbleToJump(false);
		}
		else if (e.GetIdentifier() == Event_OnGameOver::_identifier)
		{
			_pgScene->SetCurrentScene("GameOverScene");
		}
	}
}

//이거 보고 참고할 요소 찾아라.
/*
//기존 민서의 InGameManager 세이브본.

#include "InGameManager.h"
#include "Portal.h"

#include "../ParagonData/Scene.h"

#include <algorithm>
namespace Pg::DataScript
{
	void InGameManager::Awake()
	{
		_gameState = Pg::DataScript::eGameState::_NOTSTARTED;	// 게임이 시작하지 않음
		_playerState = Pg::DataScript::ePlayerState::_NONE;		// 플레이어에 대한 정보 없음
		_artifacts.reserve(3);	// 아티팩트의 개수
		_playersLife = 3;		// 플레이어의 실제 목숨 개수
	}

	void InGameManager::Start()
	{
		_currentSceneName = _object->GetScene()->GetSceneName();

		/// portal -> next scene name 전달 해주기
		// next scene Name 찾기
		auto currentIt = std::find_if(_sceneList.begin(), _sceneList.end(),
			[&](std::string sceneName) {return _currentSceneName == sceneName; });

		std::string nextSceneName;
		if (currentIt != _sceneList.end())
		{
			auto nextIt = std::next(currentIt, 1);
			nextSceneName = *nextIt;
		}
		else
			nextSceneName = (*_sceneList.begin());

		// 찾은 next scene Name 전달
		for (auto& obj : _object->GetScene()->GetObjectList())
		{
			if (obj->GetComponent<Pg::DataScript::Portal>() != nullptr)
			{
				obj->GetComponent<Pg::DataScript::Portal>()->SetNextScene(nextSceneName);
			}
		}
	}

	void InGameManager::Update()
	{
		// Play -> UI Button Click
		_gameState = Pg::DataScript::eGameState::_PLAY;

		if (_gameState == eGameState::_PLAY)
		{
			// Pause -> UI Button Click
			_gameState = Pg::DataScript::eGameState::_PAUSE;
			// gameTime -> stop
			// player, monster -> not move

			// monster
			// -> 현재 씬 내에 몬스터가 몇 마리 있는지 판별
			// -> 스테이지 1의 경우 각 전투 구역 마다 몇 마리 있는지 판별

			// stage
			// -> 사운드. 각 스테이지 마다 사운드가 다를 수 있음. 또는 같을 수 있음. 끊어지지 않게 계속 들릴 수 있도록
			// -> 팝업. 스테이지 1의 경우, 튜토리얼과 비슷한 느낌이기에 팝업이 떠야 함. +) pause 팝업
			// -> 목숨 개수에 따라 다시 소환 될 경우 위치를 기억하고 있어야 한다.(몇 스테이지, 무슨 구역)
			// ->> 구역을 나누지는 않을 것 같으니, 죽었다가 다시 소환 될 경우 스테이지만 복기 하는 것으로

			// etc
			// -> 아티팩트 관리
			// -> 게임 내 시간 관리
			// --------------------------------------------------------------------------

			/// player
			// -> 체력에 따라 live/die 판별
			// -> player가 죽었을 경우 게임 내의 시간을 멈춘다. (몬스터의 공격도 멈춘다..?)
			// -> player의 위치는 곧 현재 씬
			// -> player의 목숨 관리(체력 말고 다시 되살아 날 수 있는 목숨 -> 3개)
			if (_playerState == ePlayerState::_DIE)
			{
				if (_playersLife == 0)
				{
					// 1. playersLife == 0
					_gameState = Pg::DataScript::eGameState::_STOP;

					// 2. current scene == play button in UI
					_gameState = Pg::DataScript::eGameState::_NOTSTARTED;
				}
				else
					_playersLife -= 1;
			}
		}
	}

	void InGameManager::ResetGame()
	{
		_gameTime = 0.0f;
		_gameState = eGameState::_NOTSTARTED;
		_playersLife = 3;

		for (auto& artifact : _artifacts)
		{
			artifact.second = false;
		}
	}
}



*/





