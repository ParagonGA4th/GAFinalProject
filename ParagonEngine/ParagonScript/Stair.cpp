#include "Stair.h"

#include "TotalGameManager.h"

#include "../ParagonData/StaticMeshRenderer.h"
#include "../ParagonData/StaticBoxCollider.h"
#include "../ParagonData/SphereCollider.h"


namespace Pg::DataScript
{
	Stair::Stair(Pg::Data::GameObject* obj)
		:ScriptInterface(obj)
	{
	}

	void Stair::GrabManagedObjects()
	{
		_renderer = _object->GetComponent<Pg::Data::StaticMeshRenderer>();
		assert(_renderer != nullptr);

		_collider = _object->GetComponent<Pg::Data::StaticBoxCollider>();
		assert(_collider != nullptr);

		_area = _object->GetScene()->FindObjectWithName("BattleArea_GolemBoss")->GetComponent<Pg::Data::SphereCollider>();
		assert(_area != nullptr);
	}

	void Stair::BeforePhysicsAwake()
	{

	}

	void Stair::Awake()
	{
		//_renderer->SetAlphaPercentage(0.f);
		_renderer->SetActive(false);
		_collider->SetActive(false);
	}

	void Stair::Update()
	{
		if (!_area->GetActive())
		{
			_renderer->SetActive(true);
			_collider->SetActive(true);

			//if (_renderer->GetAlphaPercentage() >= 100.f)
			//else _renderer->SetAlphaPercentage(_renderer->GetAlphaPercentage() + ALPHA_PERCENT);
		}
	}
}