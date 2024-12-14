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
			// 3D�϶��� �۵��ϰ� �ϴ� ��.
			// �������� ����?
			// Update���� Local Handler���� �����ϰ�,
			// LateUpdate ��� �̸� �ڴʰ� ȸ���ϴ� ��ĵ� ��� ��.
			
			//3D �ȿ� ������, ü���� ������Ű��.
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

		//GameManager�� �۵� : ������ 3D Scene�� ���� ���������.
		_isUpdating = (changedScene->GetIs3D()) ? true : false;

		// InGame���� Scene�� �ٲ���� �� �÷��̾��� ü�� �� ��� �Ȱ��� �̾�������
		float _prevHealthPoint = 0.f;
		float _prevManaPoint = 0.f;
		int _prevStaminaPoint = 0;
		int _prevPlayerLife = 0;

		//Title Scene �� 2D Scene���� �Ѿ�� ����� �� FIX.
		bool tIsInfoTransferPossible = (_recordedPreviousScene != nullptr) && (_recordedPreviousScene->GetIs3D());

		//���� ����.
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

		//TotalGameManager�� ������ �տ��� ������Ʈ�Ǿ��� ���̴�, 
		//���ο��� ������Ʈ�� HandlerBundle3D�� �޾ƿ´�. 2D�� ��� nullptr.
		_handlerBundle3D = _totalGameManager->GetCurrentHandlerBundle();

		//�ٲ� ����
		if (changedScene->GetIs3D())
		{
			_handlerBundle3D->_playerBehavior->healthPoint = _prevHealthPoint;
			_handlerBundle3D->_playerBehavior->manaPoint = _prevManaPoint;
			_handlerBundle3D->_playerBehavior->staminaPoint = _prevStaminaPoint;
			_handlerBundle3D->_playerBehavior->_playerlife = _prevPlayerLife;
		}

		//����, GameState�� ���� �����ϸ鼭 ����.

		//���� Switch�� ���� ���� ���� �ִ� Scene ���.
		_recordedPreviousScene = changedScene;

		//���� ���Ƶ�.
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
		//API �� �ޱ�.
		_pgScene = &singleton<Pg::API::PgScene>();
		_pgTime = &singleton<Pg::API::Time::PgTime>();
		_pgInput = &singleton<Pg::API::Input::PgInput>();

		// �ݵ�� �ش� Object�� Don't Destroy On Load ������ �Ǿ� �־�� �Ѵ�.
		assert(_object->GetDontDestroyOnLoad() && "XML���� �̷��� ���Ծ�� �Ѵ�");

		//Total Game Manager ��������. 
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

		//�۷ι� ��ü���� ����ϴ�.
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
			//���� ���� �̺�Ʈ���� �Ѳ����� �ڵ鸵 �� ���, �̷��� Ȱ���. 
			//const Event_PlayerDeath& demoEvent = static_cast<const Event_PlayerDeath&>(e);
			PG_ERROR("PLAYER_INGAMEMANGER_HANDLE_EVENTS");

			// ���� �ڽ��� ���� �ִ� ��.
			Pg::Data::Scene* tBelongScene = _handlerBundle3D->_playerBehavior->_object->GetScene();

			// ��ü �� �����ϱ�.
			TotalGameManager::GetInstance(nullptr)->CallForEntireSceneReset(tBelongScene, NULL, nullptr);

			//�״�����, Life �ϳ� ���.
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

//�̰� ���� ������ ��� ã�ƶ�.
/*
//���� �μ��� InGameManager ���̺꺻.

#include "InGameManager.h"
#include "Portal.h"

#include "../ParagonData/Scene.h"

#include <algorithm>
namespace Pg::DataScript
{
	void InGameManager::Awake()
	{
		_gameState = Pg::DataScript::eGameState::_NOTSTARTED;	// ������ �������� ����
		_playerState = Pg::DataScript::ePlayerState::_NONE;		// �÷��̾ ���� ���� ����
		_artifacts.reserve(3);	// ��Ƽ��Ʈ�� ����
		_playersLife = 3;		// �÷��̾��� ���� ��� ����
	}

	void InGameManager::Start()
	{
		_currentSceneName = _object->GetScene()->GetSceneName();

		/// portal -> next scene name ���� ���ֱ�
		// next scene Name ã��
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

		// ã�� next scene Name ����
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
			// -> ���� �� ���� ���Ͱ� �� ���� �ִ��� �Ǻ�
			// -> �������� 1�� ��� �� ���� ���� ���� �� ���� �ִ��� �Ǻ�

			// stage
			// -> ����. �� �������� ���� ���尡 �ٸ� �� ����. �Ǵ� ���� �� ����. �������� �ʰ� ��� �鸱 �� �ֵ���
			// -> �˾�. �������� 1�� ���, Ʃ�丮��� ����� �����̱⿡ �˾��� ���� ��. +) pause �˾�
			// -> ��� ������ ���� �ٽ� ��ȯ �� ��� ��ġ�� ����ϰ� �־�� �Ѵ�.(�� ��������, ���� ����)
			// ->> ������ �������� ���� �� ������, �׾��ٰ� �ٽ� ��ȯ �� ��� ���������� ���� �ϴ� ������

			// etc
			// -> ��Ƽ��Ʈ ����
			// -> ���� �� �ð� ����
			// --------------------------------------------------------------------------

			/// player
			// -> ü�¿� ���� live/die �Ǻ�
			// -> player�� �׾��� ��� ���� ���� �ð��� �����. (������ ���ݵ� �����..?)
			// -> player�� ��ġ�� �� ���� ��
			// -> player�� ��� ����(ü�� ���� �ٽ� �ǻ�� �� �� �ִ� ��� -> 3��)
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





