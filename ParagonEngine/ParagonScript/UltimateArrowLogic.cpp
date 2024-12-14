#include "UltimateArrowLogic.h"

#include "../ParagonData/SkinnedMeshRenderer.h"
#include "../ParagonData/BoxCollider.h"
#include "../ParagonData/StaticSphereCollider.h"
#include "../ParagonData/CapsuleCollider.h"
#include "../ParagonData/AudioSource.h"
#include "../ParagonData/LayerMask.h"
#include "../ParagonData/PhysicsCollision.h"

#include "../ParagonAPI/PgTime.h"
#include "../ParagonAPI/PgTween.h"
#include "../ParagonAPI/PgGraphics.h"
#include "../ParagonUtil/Log.h"

#include "BaseMonster.h"
#include "IEnemyBehaviour.h"
#include "PlayerHandler.h"
#include "PlayerMovementSector.h"
#include "TotalGameManager.h"
#include "ComboSystem.h"
#include "CombatSystem.h"

#include <cassert>
#include <algorithm>
#include <singleton-cpp/singleton.h>

namespace Pg::DataScript
{
	UltimateArrowLogic::UltimateArrowLogic(Pg::Data::GameObject* obj) : ScriptInterface(obj)
	{
		_pgTime = &singleton<Pg::API::Time::PgTime>();
		_pgTween = &singleton<Pg::API::Tween::PgTween>();
		_pgGraphics = &singleton<Pg::API::Graphics::PgGraphics>();
	}

	void UltimateArrowLogic::CleanOnSceneChange()
	{
		if (_voUltimateHitOnMonster != nullptr)
		{
			_pgGraphics->RemoveEffectObject(_voUltimateHitOnMonster);
			delete _voUltimateHitOnMonster;
			_voUltimateHitOnMonster = nullptr;
		}
	}

	void UltimateArrowLogic::GrabManagedObjects()
	{
		//
	}

	void UltimateArrowLogic::BeforePhysicsAwake()
	{
		//CombatSystem �޾ƿ���.
		_combatSystem = CombatSystem::GetInstance(nullptr);

		//���������� Physics���� SceneSystem�� �Լ����� ���߿� ȣ���. �׷���, �̸� �� �� �ִ� ����� EngineMain-SceneSystem�� �����صξ���.
		_collider = _object->GetComponent<Pg::Data::StaticSphereCollider>();
		assert(_collider != nullptr);
		_collider->SetLayer(Pg::Data::Enums::eLayerMask::LAYER_PROJECTILES);
		_collider->SetActive(false);

		HandlerBundle3D* tH3d = TotalGameManager::GetInstance(nullptr)->GetHandlerBundleByScene(_object->GetScene());
		_playerHandler = tH3d->_playerBehavior;
	}

	void UltimateArrowLogic::Awake()
	{
		_meshRenderer = _object->GetComponent<Pg::Data::SkinnedMeshRenderer>();
		assert(_meshRenderer != nullptr);
		//_meshRenderer->SetActive(false);
		_meshRenderer->_alphaPercentage = 0.f;

		_meshRenderer->SetAnimation("ult_atk_0.pganim", false);
		_meshRenderer->PauseAnim();

		if (_voUltimateHitOnMonster == nullptr)
		{
			_voUltimateHitOnMonster = new Pg::Data::VisualEffectRenderObject();
			_pgGraphics->RegisterEffectObject("Effect_Ultimate", _voUltimateHitOnMonster);
		}
		_voUltimateHitOnMonster->SetActive(false);
		_isHitEffectAlive = false;
	}

	void UltimateArrowLogic::Start()
	{
		_comboSystem = ComboSystem::GetInstance();
	}

	void UltimateArrowLogic::Update()
	{
		CarryOutShoot();
		EndShootingSelf();
		UpdateEffect();
	}

	void UltimateArrowLogic::CarryOutShoot()
	{
		if (_isSkillStart)
		{
			///���⿡ �ñر� �ִϸ��̼� �� �ݶ��̴� �߰��Ǹ� �ȴ�.
			_meshRenderer->PlayAnim();
			_collider->SetActive(true);
			//_meshRenderer->SetActive(true);
			_meshRenderer->_alphaPercentage = 100.f;
			_playerHandler->GetPlayerMovementSector()->SetUseUltimateSkill(true);

			//���� ���
			_playerHandler->GetUltimateSkillAudio()->Play();

			//�÷��̾� ����
			_playerHandler->GetPlayerSelfCol()->SetActive(false);
			_isSkillStart = false;
		}
	}

	void UltimateArrowLogic::EndShootingSelf()
	{
		if (_isAnimEnd)
		{
			_meshRenderer->_alphaPercentage -= ALPHA_PERCENT;

			if (_meshRenderer->_alphaPercentage <= std::numeric_limits<float>::epsilon())
			{
				_meshRenderer->SetAnimation("ult_atk_0.pganim", false);
				_meshRenderer->PauseAnim();
				_collider->SetActive(false);
				_playerHandler->GetPlayerMovementSector()->SetUseUltimateSkill(false);

				//�÷��̾� ���� ����.
				_playerHandler->GetPlayerSelfCol()->SetActive(true);
				_isAnimEnd = false;
				_ultimateSkill = false;
			}
		}
	}

	void UltimateArrowLogic::OnTriggerEnter(Pg::Data::Collider** _colArr, unsigned int count)
	{
		if (_comboSystem == nullptr)
		{
			return;
		}

		//SceneSystem�� �Լ����� �̰� �߻� �� ȣ��ȴ�. ���� �����ּ� �ϴ� �� �´�.
		for (int i = 0; i < count; i++)
		{
			//������ �ش� ����ŭ�� ���´�.
			Pg::Data::Collider* tCol = _colArr[i];

			//Physics Layer�� �˻��Ѵ�.
			//������ �� �����ϴ� ���̴�.
			if (tCol->GetLayer() == Pg::Data::Enums::eLayerMask::LAYER_MONSTER ||
				tCol->GetLayer() == Pg::Data::Enums::eLayerMask::LAYER_BOSS)
			{
				//���� ���ȴٴ� ��.
				//�ڽ��� ���� �������� �����ϴ� ���� �ƴϴ�! 
				//����ؼ� PlayerBattleBehavior�� ó���� �� ��.
				IEnemyBehaviour* tEnemyBehaviour = tCol->_object->GetComponent<IEnemyBehaviour>();
				assert((tEnemyBehaviour != nullptr) && "������ ã�Ҿ�� �ߴ�");

				//ComboSystem���� �� ���ȴٰ� ����.
				_comboSystem->HitObject(true);

				//�̶�, ����Ʈ ���� ȣ��������.
				StartEffectHit();

				//�ش� �������� �Է�, PlayerBattleBehavior�� �Ͽ��� �̸� ó���� �� �ְ� �����.
				_combatSystem->AddMonsterHitList(tEnemyBehaviour->ReturnBaseMonsterInfo(), -(ULTIMATE_ATTACK_POWER), ePartialAttackType::eULTIMATE_HIT);
				_combatSystem->AddMonsterOnHitList(tEnemyBehaviour->ReturnBaseMonsterInfo());
			}
			else
			{
				//ComboSystem���� ���ȴµ� ���Ͱ� �ƴϾ��ٰ� ����.
				_comboSystem->HitObject(false);
			}
		}
	}

	void UltimateArrowLogic::OnAnimationEnd(const std::string& justEndedAnimation)
	{
		_isAnimEnd = true;
	}

	void UltimateArrowLogic::StartSkill()
	{
		_isSkillStart = true;
		_ultimateSkill = true;
	}

	bool UltimateArrowLogic::GetUltimateSkillEnd()
	{
		return _ultimateSkill;
	}

	void UltimateArrowLogic::StartEffectHit()
	{
		_isHitEffectAlive = true;
		_effectCountTime = 0.f;
	}

	void UltimateArrowLogic::UpdateEffect()
	{
		const float EFFECT_LASTING_TIME = 0.5f;

		_voUltimateHitOnMonster->SetActive(false);

		if (_isHitEffectAlive && _ultimateSkill)
		{
			if (_effectCountTime > EFFECT_LASTING_TIME)
			{
				_effectCountTime = 0.f;
				_isHitEffectAlive = false;
			}

			_effectCountTime += _pgTime->GetDeltaTime();

			_voUltimateHitOnMonster->SetActive(true);
			_voUltimateHitOnMonster->_position = _object->_transform._position + Pg::Math::PGFLOAT3(1, 2, 1);
			_voUltimateHitOnMonster->_scale = { 7,7,7 };
		}
		else
		{
			_effectCountTime = 0.f;
		}
	}


}