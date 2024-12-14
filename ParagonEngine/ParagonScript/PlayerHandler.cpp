#include "PlayerHandler.h"
#include "CombatSystem.h"
#include "ComboSystem.h"
#include "ArrowLogic.h"
#include "PlayerMovementSector.h"
#include "PlayerCombatSector.h"

#include "../ParagonData/Scene.h"
#include "../ParagonData/LayerMask.h"
#include "../ParagonData/CapsuleCollider.h"
#include "../ParagonData/SkinnedMeshRenderer.h"
#include "../ParagonData/AudioSource.h"
#include "../ParagonData/ImageRenderer.h"
#include "../ParagonAPI/PgInput.h"
#include "../ParagonAPI/PgTime.h"
#include "../ParagonAPI/PgScene.h"
#include "../ParagonAPI/PgGraphics.h"
#include "../ParagonUtil/Log.h"

//�Ͼ �� �ִ� Event���� ����Ʈ
#include "EventList_PlayerRelated.h"
#include "EventList_GameFlowRelated.h"

#include <singleton-cpp/singleton.h>

#include <algorithm>
#include <limits>

namespace Pg::DataScript
{
	PlayerHandler::PlayerHandler(Pg::Data::GameObject* obj) : ScriptInterface(obj)
	{
		_playerMovementSector = std::make_unique<PlayerMovementSector>(this);
		_playerCombatSector = std::make_unique<PlayerCombatSector>(this);

		_pgTime = &singleton<Pg::API::Time::PgTime>();
		_pgGraphics = &singleton<Pg::API::Graphics::PgGraphics>();
	}

	PlayerHandler::~PlayerHandler()
	{
		if (_groundDustRO != nullptr)
		{
			_pgGraphics->RemoveEffectObject(_groundDustRO);
			delete _groundDustRO;
			_groundDustRO = nullptr;
		}

		if (_hitRO != nullptr)
		{
			_pgGraphics->RemoveEffectObject(_hitRO);
			delete _hitRO;
			_hitRO = nullptr;
		}
	}

	void PlayerHandler::GrabManagedObjects()
	{
		//���������� �Լ� ����.
		_playerMovementSector->GrabManagedObjects();
		_playerCombatSector->GrabManagedObjects();

		//artifact
		_imgRenderer = _object->GetScene()->FindObjectWithName("ArtifactText")->GetComponent<Pg::Data::ImageRenderer>();
		assert(_imgRenderer);

		_sceneHelper = &singleton<Pg::API::PgScene>();
	}

	void PlayerHandler::BeforePhysicsAwake()
	{
		GetInternalVariables();

		//������ �����ڴ� �ȵ�! -> AddComponent���� ������� ��������, Static Variable Initialization�� ���� ��������� �ʴ´�.
		//CombatSystem�� TitleScene�� ���� �� ��.
		_combatSystem = CombatSystem::GetInstance(nullptr);
		_comboSystem = ComboSystem::GetInstance();

		//���������� �Լ� ����.
		_playerMovementSector->BeforePhysicsAwake();
		_playerCombatSector->BeforePhysicsAwake();

		{
			//Scene�� �̸��� ������� ������ ��������, �Ұ������� ������.
			std::string tBelongSceneName = _object->GetScene()->GetSceneName();
			bool tCanJump = (tBelongSceneName.compare("Stage2") == 0);
			_playerMovementSector->SetIsAbleToJump(tCanJump);
		}
	}

	void PlayerHandler::Awake()
	{
		_playerMovementSector->Awake();
		_playerCombatSector->Awake();

		CreateEffectObjects();

		if(_object->GetScene()->GetSceneName() == "Stage1" || _object->GetScene()->GetSceneName() == "BossStage") _imgRenderer->SetActive(false);
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

		UpdateStamina();
		UpdateVisualEffectObjects();

		_playerMovementSector->Update();
		_playerCombatSector->Update();

		if (_object->GetScene()->GetSceneName() == "Stage2")
		{
			if (artifactCount == 3)
			{
				_sceneHelper->SetCurrentScene("BossStage");
			}
		}
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

	void PlayerHandler::OnAnimationEnd(const std::string& justEndedAnimation)
	{
		_playerMovementSector->OnAnimationEnd(justEndedAnimation);
		_playerCombatSector->OnAnimationEnd(justEndedAnimation);
	}

	void PlayerHandler::HandleEvents(const IEvent& e, UsedVariant usedVar1, UsedVariant usedVar2)
	{
		if (e.GetIdentifier() == Event_PlayerDeath::_identifier)
		{
			//���� ���� �̺�Ʈ���� �Ѳ����� �ڵ鸵 �� ���, �̷��� Ȱ���. 
			//const Event_PlayerDeath& demoEvent = static_cast<const Event_PlayerDeath&>(e);
		}
		else if (e.GetIdentifier() == Event_PlayerGetArtifact::_identifier)
		{
			artifactCount++;
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

	void PlayerHandler::ChangePlayerHealth(float level, bool isTrap)
	{
		if(!isTrap && std::signbit(level)) _meshRenderer->SetAnimation("PA_00012.pganim", false);

		healthPoint = std::clamp<float>(healthPoint + level, 0.0f, MAX_PLAYER_HEALTH);

		if (healthPoint > std::numeric_limits<float>::epsilon() &&
			healthPoint < PLAYER_HEALTH_LOW_BARRIER)
		{
			_combatSystem->Post(Event_PlayerOnLowHealth(), _object, healthPoint);
		}
		else if (healthPoint < std::numeric_limits<float>::epsilon())
		{
			//���� ü���� 0�� ���, 
			_combatSystem->Post(Event_PlayerDeath(), _object, 0.0f);
		}
	}

	void PlayerHandler::ChangePlayerMana(float level)
	{
		manaPoint = std::clamp<float>(manaPoint + level, 0.0f, MAX_PLAYER_MANA);
	}

	void PlayerHandler::ChangePlayerStamina(int level)
	{
		staminaPoint = std::clamp<int>(staminaPoint + level, 0, MAX_PLAYER_STAMINA);

		if (staminaPoint == 0)
		{
			_shouldStaminaCharge = true;
		}
	}

	void PlayerHandler::ResetAll()
	{
		//assert(false && "not implemented yet");
		_playerCombatSector->ResetAll();
		_playerMovementSector->ResetAll();
		
		//�ٽ� ���¹̳ʸ� �� �� ���� ���̴�.
		_isStaminaReadyToUse = true;
		_shouldStaminaCharge = false;
		_staminaCountingTime = 0.f;

		//���� ���� : Heal
		healthPoint = MAX_PLAYER_HEALTH;
		manaPoint = 0; //Mana�� ó�� 0���� �����Ѵ�.
		staminaPoint = MAX_PLAYER_STAMINA;

		// ��Ƽ��Ʈ Ȥ�� �𸣴ϱ�..
		//artifactCount = 0;
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

	const int* PlayerHandler::ReturnPlayerStaminaPointPointerConst() const
	{
		return &staminaPoint;
	}

	void PlayerHandler::GetInternalVariables()
	{
		_selfCol = _object->GetComponent<Pg::Data::CapsuleCollider>();
		_selfCol->FreezeAxisX(true);
		_selfCol->FreezeAxisY(true);
		_selfCol->FreezeAxisZ(true);
		_selfCol->SetMass(5.0f);
		_selfCol->SetLayer(Pg::Data::Enums::eLayerMask::LAYER_PLAYER);
		PG_TRACE(_selfCol->GetLayer());

		_meshRenderer = _object->GetComponent<Pg::Data::SkinnedMeshRenderer>();
		//_meshRenderer->SetRendererOffset(_rendererOffSet);
		assert(_meshRenderer != nullptr);


		//Sounds.
		Pg::Data::GameObject* tCommonAttackSound = _object->GetScene()->FindObjectWithName("PlayerCommonAttackSound");
		_commonAttackAudio = tCommonAttackSound->GetComponent<Pg::Data::AudioSource>();

		Pg::Data::GameObject* tPlayerWalkSound = _object->GetScene()->FindObjectWithName("PlayerWalkOutSound");
		_walkAudio = tPlayerWalkSound->GetComponent<Pg::Data::AudioSource>();

		Pg::Data::GameObject* tPlayerJumpSound = _object->GetScene()->FindObjectWithName("PlayerJumpSound");
		_jumpAudio = tPlayerJumpSound->GetComponent<Pg::Data::AudioSource>();

		Pg::Data::GameObject* tPlayerAvoidSound = _object->GetScene()->FindObjectWithName("PlayerAvoidSound");
		_avoidAudio = tPlayerAvoidSound->GetComponent<Pg::Data::AudioSource>();

		Pg::Data::GameObject* tPlayerFireSkillSound = _object->GetScene()->FindObjectWithName("PlayerFireSkillAttackSound");
		_fireSkillAudio = tPlayerFireSkillSound->GetComponent<Pg::Data::AudioSource>();

		Pg::Data::GameObject* tPlayerIceSkillSound = _object->GetScene()->FindObjectWithName("PlayerIceSkillound");
		_iceSkillAudio = tPlayerIceSkillSound->GetComponent<Pg::Data::AudioSource>();

		Pg::Data::GameObject* tPlayerUltimateSkillSound = _object->GetScene()->FindObjectWithName("PlayerUltimateSkillSound");
		_ultimateSkillAudio = tPlayerUltimateSkillSound->GetComponent<Pg::Data::AudioSource>();

		Pg::Data::GameObject* tPlayerHitSound = _object->GetScene()->FindObjectWithName("PlayerHitSound");
		_hitAudio = tPlayerHitSound->GetComponent<Pg::Data::AudioSource>();

		Pg::Data::GameObject* tPlayerDieSound = _object->GetScene()->FindObjectWithName("PlayerDieSound");
		_dieAudio = tPlayerDieSound->GetComponent<Pg::Data::AudioSource>();

		//

	}

	bool PlayerHandler::GetIsStaminaReadyToUse()
	{
		return _isStaminaReadyToUse;
	}

	Pg::Data::CapsuleCollider* PlayerHandler::GetPlayerSelfCol()
	{
		return _selfCol;
	}

	void PlayerHandler::UpdateStamina()
	{
		if (_shouldStaminaCharge)
		{
			_staminaCountingTime += _pgTime->GetDeltaTime();
			if (_staminaCountingTime > STAMINA_ONE_SLOT_CHARGE_TIME)
			{
				//�ϳ��� ����.
				ChangePlayerStamina(1);
				_staminaCountingTime = 0.f;
			}
			
			if (staminaPoint == MAX_PLAYER_STAMINA)
			{
				_shouldStaminaCharge = false;
				_staminaCountingTime = 0.f;
			
				//���� ���¹̳� �� �� �ְ� ����.
				_isStaminaReadyToUse = true;
			}
		}

		if (staminaPoint == 0)
		{
			// ���� ���¹̳� ����Ʈ�� 0�̸�, ��� false �Ҵ�.
			// ������ ���� �� �̻� ������Ʈ���� �ʴ´�.
			_isStaminaReadyToUse = false;
			//Stamina Charge
			_shouldStaminaCharge = true;
		}

		if (_object->GetScene()->GetSceneName() == "Stage1" && artifactCount > 0)
		{
			_imgRenderer->SetActive(true);
		}

		_imgRenderer->SetImageIndex(artifactCount);
	}

	int PlayerHandler::GetPlayerLife()
	{
		return _playerlife;
	}

	void PlayerHandler::ChangePlayerLife(int level)
	{
		_playerlife = std::clamp<int>(_playerlife + level, 0, MAX_LIFE_COUNT);
		if (_playerlife == 0)
		{
			_combatSystem->Post(Event_OnGameOver(), NULL, NULL);
		}
	}

	void PlayerHandler::CreateEffectObjects()
	{
		if (_groundDustRO == nullptr)
		{
			_groundDustRO = new Pg::Data::VisualEffectRenderObject();
			_pgGraphics->RegisterEffectObject("Effect_GroundDust", _groundDustRO);
			_dustImagePointer = _pgGraphics->GetEffectTextureIndexPointer("Effect_GroundDust");
			_groundDustRO->SetActive(false);
		}

		if (_hitRO == nullptr)
		{
			_hitRO = new Pg::Data::VisualEffectRenderObject();
			_pgGraphics->RegisterEffectObject("Effect_Normal_Hit", _hitRO);
			_hitRO->SetActive(false);

			_chosenPtrEffect = _pgGraphics->GetEffectTextureIndexPointer("Effect_Normal_Hit");
			assert(_chosenPtrEffect != nullptr);
		}

		_isEffectPlayOnHit = false;
		_effectPlayOnHitTime = 0.f;
	}

	void PlayerHandler::UpdateVisualEffectObjects()
	{
		//Ground Dust
		if (_playerMovementSector->GetIsMoving())
		{
			_groundDustRO->SetActive(true);

			_dustCounterVar++;

			if (_dustCounterVar > _groundDustMaxIndex)
			{
				_dustCounterVar = 0;
			}

			if (_dustCounterVar <= _groundDustMaxIndex)
			{
				*_dustImagePointer = _dustCounterVar;
			}
			
			_groundDustRO->_position = _object->_transform._position + Pg::Math::PGFLOAT3(1, 0.1, 1);
			_groundDustRO->_scale = { 1,2,1 };
		}
		else
		{
			_groundDustRO->SetActive(false);
		}

		//Hit RO.
		_hitRO->SetActive(false);

		const float CLOSING_TIME = 0.5f;

		if (_isEffectPlayOnHit)
		{
			_hitRO->SetActive(true);
			_effectPlayOnHitTime += _pgTime->GetDeltaTime();
			_hitRO->_position = _object->_transform._position;
			_hitRO->_scale = { 9,9,9 };

			if (_effectPlayOnHitTime < 0.1f) { (*_chosenPtrEffect) = 0; }
			else if (_effectPlayOnHitTime < 0.2f) { (*_chosenPtrEffect) = 1; }
			else if (_effectPlayOnHitTime < 0.3f) { (*_chosenPtrEffect) = 2; }
			else { (*_chosenPtrEffect) = 3; }

			if (_effectPlayOnHitTime > CLOSING_TIME)
			{
				_isEffectPlayOnHit = false;
				_effectPlayOnHitTime = 0.f;
			}
		}
	}

	void PlayerHandler::InvokeHitEffect()
	{
		_isEffectPlayOnHit = true;
		_effectPlayOnHitTime = 0.f;
	}
	

}