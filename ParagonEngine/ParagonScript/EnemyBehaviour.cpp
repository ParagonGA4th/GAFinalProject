#include "EnemyBehaviour.h"
#include "EnemySight.h"
#include "PlayerBattleBehavior.h"
#include "BaseMonster.h"
#include "EnemyInfo.h"
#include "../ParagonData/MonsterHelper.h"
#include "../ParagonData/StaticBoxCollider.h"
#include "../ParagonData/BoxCollider.h"
#include "../ParagonData/CapsuleCollider.h"
#include "../ParagonData/SkinnedMeshRenderer.h"
#include "../ParagonAPI/PgRayCast.h"
#include "../ParagonData/LayerMask.h"
#include "../ParagonData/Transform.h"
#include "../ParagonData/Scene.h"
#include "../ParagonUtil/Log.h"
#include "../ParagonUtil/CheckInBox.h"

// 발표 시연을 위한 임시 변수를 위한 헤더
#include "../ParagonAPI/PgTime.h"
#include <cmath>

namespace Pg::DataScript
{
	EnemyBehaviour::EnemyBehaviour(Pg::Data::GameObject* obj) :
		ScriptInterface(obj), BaseMonster(100.f, 5.f)
	{
		_pgRayCast = &singleton<Pg::API::Raycast::PgRayCast>();

		_deltaTime = &singleton<Pg::API::Time::PgTime>();
	}

	void EnemyBehaviour::BeforePhysicsUpdate()
	{
		{
			//내부적으로 Physics보다 SceneSystem의 함수들이 나중에 호출됨. 그러니, 미리 할 수 있는 방법을 EngineMain-SceneSystem에 연결해두었다.
			//_collider = _object->GetComponent<Pg::Data::CapsuleCollider>();
			//assert(_collider != nullptr);
			//_collider->SetLayer(Pg::Data::Enums::eLayerMask::LAYER_MONSTER);
			////_collider->SetCapsuleInfo(1.f, 1.f);
			//_collider->FreezeAxisX(true);
			//_collider->FreezeAxisY(true);
			//_collider->FreezeAxisZ(true);
			//_collider->FreezeLinearY(true);

			//촬영용으로만.
			//_collider->FreezeLinearX(true);
			//_collider->FreezeLinearZ(true);

			//Debouncer.
		}
	}

	void EnemyBehaviour::Awake()
	{
		_renderer = _object->GetComponent<Pg::Data::SkinnedMeshRenderer>();
		assert(_renderer != nullptr);
	}

	void EnemyBehaviour::Start()
	{
		//auto objName = _object->GetScene()->FindObjectWithName("EnemySight");
		//if(objName == nullptr) _object->GetComponent<Pg::Data::Transform>()->AddChild(objName);

		_monsterHelper = _object->AddComponent<Pg::Data::MonsterHelper>();

		for (auto& iter : _object->_transform.GetChildren())
		{
			Pg::Data::StaticBoxCollider* staticCol = iter->_object->GetComponent<Pg::Data::StaticBoxCollider>();
			//Pg::Data::BoxCollider* Col = iter->_object->GetComponent<Pg::Data::BoxCollider>();
			//assert(staticCol != nullptr);
			EnemySight* aiSight = iter->_object->GetComponent<EnemySight>();
			assert(aiSight != nullptr);

			colVec.push_back(staticCol);
			//boxColVec.push_back(Col);
			aiSightVec.push_back(aiSight);
		}
	}

	void EnemyBehaviour::Update()
	{
		if (aiSightVec.at(0)->_playerDetected)
		{
			float interpolation = 0.2f * _deltaTime->GetDeltaTime();

			auto plVec = _object->GetScene()->FindObjectsWithTag("TAG_Player");
			auto plTrans = plVec.at(0)->_transform;

			float distance = std::abs(std::sqrt(std::pow(plTrans._position.x - _object->_transform._position.x, 2)
				+ std::pow(plTrans._position.z - _object->_transform._position.z, 2)));

			//일정 사정거리 안에 들어오면
			if (distance <= 5.f)
			{
				//공격으로 전환하기.
			}
			else
			{
				//사정거리 밖이면 플레이어로 계속 다가가기.
				Pg::Math::PGFLOAT3 tPosition = _object->_transform._position;
				tPosition = Pg::Math::PGFloat3Lerp(_object->_transform._position, plTrans._position, interpolation);
				_object->_transform._position.x = tPosition.x;
				_object->_transform._position.z = tPosition.z;
			}
		}
		else
		{
			if (_renderer->GetAnimation() != "GMA_00001.pganim")
			{
				_renderer->SetAnimation("GMA_00001.pganim", true);
			}
		}

		for (auto& it : aiSightVec)
		{
			if (it->_playerDetected == true)
			{
				_monsterHelper->_isPlayerDetected = true;
			}
			else
			{
				_monsterHelper->_isPlayerDetected = false;
			}
		}
	}
}