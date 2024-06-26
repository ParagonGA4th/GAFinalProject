#include "PlayerCombatSector.h"
#include "../ParagonData/Scene.h"
#include "ArrowLogic.h"
#include "PlayerHandler.h"
#include "PlayerMovementSector.h"
#include "../ParagonUtil/Log.h"

#include "../ParagonAPI/PgInput.h"
#include "../ParagonAPI/PgTime.h"

#include <singleton-cpp/singleton.h>

#include "../ParagonData/LayerMask.h"

namespace Pg::DataScript
{
	PlayerCombatSector::PlayerCombatSector(PlayerHandler* playerHandler) : _playerHandler(playerHandler), _object(playerHandler->_object)
	{
		_pgInput = &singleton<Pg::API::Input::PgInput>();
		_pgTime = &singleton<Pg::API::Time::PgTime>();

		_meshRenderer = _object->GetComponent<Pg::Data::SkinnedMeshRenderer>();
		assert(_meshRenderer != nullptr);

		_selfCol = _object->GetComponent<Pg::Data::DynamicCollider>();
		assert(_selfCol != nullptr);
	}

	void PlayerCombatSector::GrabManagedObjects()
	{
		FindAllArrowsInMap();
	}

	void PlayerCombatSector::BeforePhysicsAwake()
	{
		_selfCol = _object->GetComponent<Pg::Data::DynamicCollider>();
		assert(_selfCol != nullptr);

		_meshRenderer = _object->GetComponent<Pg::Data::SkinnedMeshRenderer>();
		assert(_meshRenderer != nullptr);

		_selfCol->FreezeAxisX(true);
		_selfCol->FreezeAxisY(true);
		_selfCol->FreezeAxisZ(true);
		_selfCol->SetMass(2.0f);
		//자기 자신이 Player이니, Collider의 레이어를 설정해준다.
		_selfCol->SetLayer(Pg::Data::Enums::eLayerMask::LAYER_PLAYER);
	}

	void PlayerCombatSector::Awake()
	{

	}

	void PlayerCombatSector::Start()
	{
		

		_commonAttackSound = _object->GetScene()->FindObjectWithName("PlayerCommonAttackSound");
		_commonAttackAudio = _commonAttackSound->GetComponent<Pg::Data::AudioSource>();
	}

	void PlayerCombatSector::Update()
	{
		ArrowShootingLogic();
		
		//나머지 로직은 Combat System으로 이동.
	}

	void PlayerCombatSector::LateUpdate()
	{
		PlayAdequateAnimation();
	}

	void PlayerCombatSector::HandleEvents(const IEvent& e, UsedVariant usedVar1, UsedVariant usedVar2)
	{

	}

	void PlayerCombatSector::ResetAll()
	{

	}

	void PlayerCombatSector::FindAllArrowsInMap()
	{
		//ArrowLogic의 Awake에서 자신의 Tag를 이미 "TAG_Arrow"로 설정해놨었을 것이다.
		//모든 Arrow들 받아오기. 한 30개는 되어야 빈틈을 눈치를 못 챌 것이다.
		std::vector<Pg::Data::GameObject*> allObjects = _object->GetScene()->FindObjectsWithTag("TAG_Arrow");
		for (auto& it : allObjects)
		{
			auto tALogic = it->GetComponent<Pg::DataScript::ArrowLogic>();
			if (tALogic != nullptr)
			{
				//Arrow에 자신 할당.
				tALogic->_playerBattleBehavior = _playerHandler;
				_arrowVec.push_back(tALogic);
			}
		}
	}

	void PlayerCombatSector::ArrowShootingLogic()
	{
		if (_playerHandler->GetPlayerMovementSector()->GetIsMoving() == false)
		{
			// 경과 시간을 누적
			_timeSinceLastShot += _pgTime->GetDeltaTime();

			// 쿨다운이 아직 남아 있으면 리턴
			if (_timeSinceLastShot < _shootCooldown)
			{
				return;
			}

			//마우스 좌클릭 시 공격.
			if (_pgInput->GetKeyDown(Pg::API::Input::eKeyCode::MouseLeft))
			{
				_isHit = true;
				_hitCount++;
				if (_hitCount >= 4) _hitCount = 1;

				bool tDidShoot = false;

				for (int i = 0; i < _arrowVec.size(); i++)
				{
					//지금 쏘고 있지 않은 컴포넌트만 허용해야.
					if (!(_arrowVec[i]->GetIsNowShooting()))
					{
						using namespace Pg::Math;
						//우리 Forward랑 다른 로직이 된 것 같다. 그러니, Forward를 Rotation을 갖고 Custom으로 구해주자.
						//PlayerBehavior랑 같은 위치. -> 나중에 PhysX 연동은 고쳐져야!

						//시작점 역시 Offset 하기로 했었다.
						Pg::Math::PGFLOAT3 tStartingPosition = _object->_transform._position + Pg::Math::PGFLOAT3(0.f, 2.f, 0.f);


						//Z축 향해 뒤집기. 어디에서 불완전한 연결이 일어나는지는 확인해봐야 할 것 같다.
						Pg::Math::PGFLOAT3 tShouldShootDir = Pg::Math::PGReflectVectorAgainstAxis(-_object->_transform.GetForward(), { 0,0,1 });
						tShouldShootDir = Pg::Math::PGFloat3Normalize(tShouldShootDir);

						//Pg::Math::PGFLOAT3 tShouldShootDir = Pg::Math::PGFloat3Normalize(_object->_transform.GetForward());

						float tDistanceToSpawnFrom = 0.01f;
						//_arrowVec[i]->ShootArrow(_object->_transform._position + tShouldShootDir * tDistanceToSpawnFrom, tShouldShootDir);
						_arrowVec[i]->ShootArrow(tStartingPosition + tShouldShootDir * tDistanceToSpawnFrom, tShouldShootDir);
						tDidShoot = true;

						//쿨타임 초기화
						_timeSinceLastShot = 0.0f;
						break;
					}

				}

				if (!tDidShoot)
				{
					//쐈어야 하는데 기존의 것 중에서 준비되어 있는 오브젝트가 존재하지 않는다!
					//기존 것 중에서 오래된 요소를 찾아서, (FixedSizeQueue) 여기 기준으로 마지막을 원래대로 돌려야 한다.
					//여튼, 그건 나중에 할 일.
					PG_TRACE("아직 충분히 반환되지 않음. 나중에 FixedSizeQueue로?");
				}

				_commonAttackAudio->Play();
			}
		}
		else
		{
			_hitCount = 0;
			return;
		}
	}

	
	void PlayerCombatSector::PlayAdequateAnimation()
	{
		//우선, 디폴트로 출력되는 것은 Idle Animation. 

		//Idle 초기 상태 세팅.
		std::string tToPlayAnimationName = "PA_00001.pganim";
		bool isLooping = true;

		if (_isHit)
		{
			//공격 애니매이션
			isLooping = false;
			tToPlayAnimationName = "PA_0000" + std::to_string(_hitCount + 4) + ".pganim";
		}
		//else if (_isAvoiding_Animation)
		//{
		//	//스킬 애니메이션.
		//	tToPlayAnimationName = "PA_00004.pganim";
		//	isLooping = false;
		//}
		//else if (_isJumping_Animation)
		//{
		//	//궁극기 애니메이션.
		//	tToPlayAnimationName = "PA_00003.pganim";
		//	isLooping = false;
		//}
		//else if (_isMoving_Animation)
		//{
		//	//걷기 애니메이션
		//	tToPlayAnimationName = "PA_00002.pganim";
		//	isLooping = true;
		//}

		//만약에 전 스트링과 같지 않을 시에.
		if (_prevAnimationInput.compare(tToPlayAnimationName) != 0)
		{
			_meshRenderer->SetAnimation(tToPlayAnimationName, isLooping);
		}

		//애니메이션 인풋 스트링 기록.
		_prevAnimationInput = tToPlayAnimationName;
	}

	void PlayerCombatSector::OnAnimationEnd()
	{
		// Loop가 안되는 모든 애니매이션의 flag는 여기서 false로 변경
		_isHit = false;
	}

	void PlayerCombatSector::FixedUpdate()
	{

	}

	

}