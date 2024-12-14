#include "EnemyBehaviour.h"
#include "EnemySight.h"
#include "PlayerHandler.h"
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

// ��ǥ �ÿ��� ���� �ӽ� ������ ���� ���
#include "../ParagonAPI/PgTime.h"
#include <cmath>

namespace Pg::DataScript
{
	EnemyBehaviour::EnemyBehaviour(Pg::Data::GameObject* obj) :
		ScriptInterface(obj), BaseMonsterInfo(100.f, 5.f)
	{
		_pgRayCast = &singleton<Pg::API::Raycast::PgRayCast>();

		_deltaTime = &singleton<Pg::API::Time::PgTime>();
	}

	void EnemyBehaviour::BeforePhysicsUpdate()
	{
		{
			//���������� Physics���� SceneSystem�� �Լ����� ���߿� ȣ���. �׷���, �̸� �� �� �ִ� ����� EngineMain-SceneSystem�� �����صξ���.
			_collider = _object->GetComponent<Pg::Data::CapsuleCollider>();
			assert(_collider != nullptr);
			_collider->SetLayer(Pg::Data::Enums::eLayerMask::LAYER_MONSTER);
			//_collider->SetCapsuleInfo(1.f, 1.f);
			_collider->FreezeAxisX(true);
			_collider->FreezeAxisY(true);
			_collider->FreezeAxisZ(true);
			_collider->FreezeLinearY(true);

			//�Կ������θ�.
			_collider->FreezeLinearX(true);
			_collider->FreezeLinearZ(true);

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
			if (aiSight == nullptr) break;
			//assert(aiSight != nullptr);

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

			//���� �����Ÿ� �ȿ� ������
			if (distance <= 5.f)
			{
				//�������� ��ȯ�ϱ�.
			}
			else
			{
				//�����Ÿ� ���̸� �÷��̾�� ��� �ٰ�����.
				Pg::Math::PGFLOAT3 tPosition = _object->_transform._position;
				tPosition = Pg::Math::PGFloat3Lerp(_object->_transform._position, plTrans._position, interpolation);
				_object->_transform._position.x = tPosition.x;
				_object->_transform._position.z = tPosition.z;
			}
		}
		//�þ߿� �Ⱥ��̸�
		else
		{
			//��� ����.
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