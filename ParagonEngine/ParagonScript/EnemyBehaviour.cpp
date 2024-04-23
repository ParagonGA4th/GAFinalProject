#include "EnemyBehaviour.h"
#include "EnemySight.h"
#include "../ParagonData/StaticBoxCollider.h"
#include "../ParagonData/CapsuleCollider.h"
#include "../ParagonAPI/PgRayCast.h"
#include "../ParagonData/LayerMask.h"
#include "../ParagonData/Transform.h"
#include "../ParagonData/Scene.h"

namespace Pg::DataScript
{

	EnemyBehaviour::EnemyBehaviour(Pg::Data::GameObject* obj) :
		ScriptInterface(obj)
	{
		_pgRayCast = &singleton<Pg::API::Raycast::PgRayCast>();
	}

	void EnemyBehaviour::BeforePhysicsUpdate()
	{
		{
			//내부적으로 Physics보다 SceneSystem의 함수들이 나중에 호출됨. 그러니, 미리 할 수 있는 방법을 EngineMain-SceneSystem에 연결해두었다.
			_collider = _object->GetComponent<Pg::Data::CapsuleCollider>();
			assert(_collider != nullptr);
			_collider->SetLayer(Pg::Data::Enums::eLayerMask::LAYER_MONSTER); // 자기 자신이 Projectile이라고 해주기.
			_collider->FreezeAxisX(true);
			_collider->FreezeAxisZ(true);

			//Debouncer.
			_alreadyCalledBPU = true;
		}
	}

	void EnemyBehaviour::Awake()
	{
	}

	void EnemyBehaviour::Start()
	{
		auto objName = _object->GetScene()->FindObjectWithName("EnemySight");
		_object->GetComponent<Pg::Data::Transform>()->AddChild(objName);
		
		for (auto& iter : _object->_transform.GetChildren())
		{
			Pg::Data::StaticBoxCollider* staticCol = iter->_object->GetComponent<Pg::Data::StaticBoxCollider>();
			EnemySight* aiSight = iter->_object->GetComponent<EnemySight>();

			colVec.push_back(staticCol);
			aiSightVec.push_back(aiSight);
		}
	}

	void EnemyBehaviour::Update()
	{
		for (auto& it : aiSightVec)
		{
			if (it->_playerDetected == true)
			{
				_colVecActive = false;

			}
			else
			{
				_colVecActive = true;
			}
		}

		if (_colVecActive == false)
		{
			for (auto& iter : colVec)
			{
				iter->SetActive(false);
			}

			_colVecActive = true;
		}
		else
		{
			for (auto& iter : colVec)
			{
				iter->SetActive(true);
			}
		}
	}
}