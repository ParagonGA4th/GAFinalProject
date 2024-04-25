#include "EnemyBehaviour.h"
#include "EnemySight.h"
#include "BaseMonster.h"
#include "../ParagonData/StaticBoxCollider.h"
#include "../ParagonData/CapsuleCollider.h"
#include "../ParagonData/SkinnedMeshRenderer.h"
#include "../ParagonAPI/PgRayCast.h"
#include "../ParagonData/LayerMask.h"
#include "../ParagonData/Transform.h"
#include "../ParagonData/Scene.h"
#include "../ParagonUtil/Log.h"
namespace Pg::DataScript
{
	EnemyBehaviour::EnemyBehaviour(Pg::Data::GameObject* obj) :
		ScriptInterface(obj), BaseMonster(100.f, 5.f)
	{
		_pgRayCast = &singleton<Pg::API::Raycast::PgRayCast>();
	}

	void EnemyBehaviour::BeforePhysicsUpdate()
	{
		{
			//내부적으로 Physics보다 SceneSystem의 함수들이 나중에 호출됨. 그러니, 미리 할 수 있는 방법을 EngineMain-SceneSystem에 연결해두었다.
			_collider = _object->GetComponent<Pg::Data::CapsuleCollider>();
			assert(_collider != nullptr);
			_collider->SetLayer(Pg::Data::Enums::eLayerMask::LAYER_MONSTER);
			//_collider->SetCapsuleInfo(1.f, 1.f);
			_collider->FreezeAxisX(true);
			_collider->FreezeAxisY(true);
			_collider->FreezeAxisZ(true);

			//Debouncer.
			_alreadyCalledBPU = true;
		}
	}

	void EnemyBehaviour::Awake()
	{
		_renderer = _object->GetComponent<Pg::Data::SkinnedMeshRenderer>();
		assert(_renderer != nullptr);
	}	

	void EnemyBehaviour::Start()
	{
		auto objName = _object->GetScene()->FindObjectWithName("EnemySight");
		_object->GetComponent<Pg::Data::Transform>()->AddChild(objName);
		
		for (auto& iter : _object->_transform.GetChildren())
		{
			Pg::Data::StaticBoxCollider* staticCol = iter->_object->GetComponent<Pg::Data::StaticBoxCollider>();
			assert(staticCol != nullptr);
			EnemySight* aiSight = iter->_object->GetComponent<EnemySight>();
			assert(aiSight != nullptr);

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
				it->_playerDetected = false;

			}
		}

		if (_colVecActive == false && !_isAnimStart)
		{
			for (auto& iter : colVec)
			{
				iter->SetActive(false);
				
			}

			_renderer->SetAnimation("PpakMonster_Punch.pganim", true);
			PG_TRACE("애니메이션 재생 막 시작");

			_isAnimStart = true;
		}
		else if(_colVecActive)
		{
			for (auto& iter : colVec)
			{
				iter->SetActive(true);
			}

			_isAnimStart = false;
		}
	}
}