#include "Grating_2.h"
#include "HandleBundle3D.h"
#include "TotalGameManager.h"
#include "PlayerHandler.h"

#include "../ParagonData/Scene.h"
#include "../ParagonData/StaticMeshRenderer.h"
#include "../ParagonData/StaticBoxCollider.h"

namespace Pg::DataScript
{

	Grating_2::Grating_2(Pg::Data::GameObject* obj)
		:ScriptInterface(obj)
	{

	}

	void Grating_2::GrabManagedObjects()
	{
		_renderer = _object->GetComponent<Pg::Data::StaticMeshRenderer>();
		assert(_renderer);

		for (auto col : _object->_transform.GetChildren())
		{
			_collider = col->_object->GetComponent<Pg::Data::StaticBoxCollider>();
			assert(_collider);
		}
	}

	void Grating_2::BeforePhysicsAwake()
	{
	}

	void Grating_2::Awake()
	{
		HandlerBundle3D* tH3d = TotalGameManager::GetInstance(nullptr)->GetHandlerBundleByScene(_object->GetScene());
		_playerHandler = tH3d->_playerBehavior;

		//_renderer->SetActive(true);
		_renderer->_alphaPercentage = 100.f;
		_collider->SetActive(true);
	}

	void Grating_2::Update()
	{
		if (_playerHandler->artifactCount >= 3)
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