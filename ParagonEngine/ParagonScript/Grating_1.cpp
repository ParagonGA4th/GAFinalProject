#include "Grating_1.h"
#include "../ParagonData/Scene.h"
#include "../ParagonData/StaticMeshRenderer.h"
#include "../ParagonData/StaticBoxCollider.h"
#include "../ParagonData/Collider.h"

namespace Pg::DataScript
{

	Grating_1::Grating_1(Pg::Data::GameObject* obj)
		:ScriptInterface(obj)
	{

	}

	void Grating_1::GrabManagedObjects()
	{
		_renderer = _object->GetComponent<Pg::Data::StaticMeshRenderer>();
		assert(_renderer);

		for (auto col : _object->_transform.GetChildren())
		{
			_collider = col->_object->GetComponent<Pg::Data::StaticBoxCollider>();
			assert(_collider);
		}

		_otherCol = _object->GetScene()->FindObjectWithName("BattleArea_MiniGolem")->GetComponent<Pg::Data::Collider>();
	}

	void Grating_1::BeforePhysicsAwake()
	{
	}

	void Grating_1::Awake()
	{
		//_renderer->SetActive(true);
		_renderer->_alphaPercentage = 100.f;
		_collider->SetActive(true);
	}

	void Grating_1::Update()
	{
		if (!_otherCol->GetActive())
		{
			if (_renderer->_alphaPercentage <= std::numeric_limits<float>::epsilon())
			{
				//_renderer->SetActive(false);
				_collider->SetActive(false);
			}
			else
			{
				_renderer->_alphaPercentage -= ALPHA_PERCENT;
			}
		}
	}
}