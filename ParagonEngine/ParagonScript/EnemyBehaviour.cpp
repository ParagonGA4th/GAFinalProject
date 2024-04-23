#include "EnemyBehaviour.h"
#include "EnemySight.h"
#include "../ParagonData/StaticBoxCollider.h"
#include "../ParagonData/SkinnedMeshRenderer.h"
#include "../ParagonAPI/PgRayCast.h"
#include <cassert>

namespace Pg::DataScript
{
	EnemyBehaviour::EnemyBehaviour(Pg::Data::GameObject* obj) :
		ScriptInterface(obj)
	{

	}

	void EnemyBehaviour::Awake()
	{
		_renderer = _object->GetComponent<Pg::Data::SkinnedMeshRenderer>();
		assert(_renderer != nullptr);
	}

	void EnemyBehaviour::Start()
	{
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

			_renderer->SetAnimation("PpakMonster_Punch.pganim", true);

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