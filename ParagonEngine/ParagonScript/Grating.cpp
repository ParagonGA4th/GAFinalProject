#include "Grating.h"
#include "HandleBundle3D.h"
#include "TotalGameManager.h"
#include "PlayerHandler.h"

#include "../ParagonData/Scene.h"
#include "../ParagonData/StaticMeshRenderer.h"
#include "../ParagonData/StaticBoxCollider.h"
#include "../ParagonData/Collider.h"

namespace Pg::DataScript
{

	Grating::Grating(Pg::Data::GameObject* obj)
		:ScriptInterface(obj)
	{

	}

	void Grating::GrabManagedObjects()
	{
		_renderer = _object->GetComponent<Pg::Data::StaticMeshRenderer>();
		assert(_renderer);


		for (auto col : _object->_transform.GetChildren())
		{
			_collider = col->_object->GetComponent<Pg::Data::StaticBoxCollider>();
			assert(_collider);
		}

		if (_areaIndex == 0)
		{
			_otherCol = _object->GetScene()->FindObjectWithName("BattleArea_MiniGolem")->GetComponent<Pg::Data::Collider>();
		}
		else
		{
			HandlerBundle3D* tH3d = TotalGameManager::GetInstance(nullptr)->GetHandlerBundleByScene(_object->GetScene());
			_playerHandler = tH3d->_playerBehavior;
		}
	}

	void Grating::BeforePhysicsAwake()
	{

	}

	void Grating::Awake()
	{
		//_renderer->SetActive(true);
		_renderer->_alphaPercentage = 100.f;
		_collider->SetActive(true);
	}

	void Grating::Update()
	{
		bool isStart = false;
		if (_areaIndex == 0) isStart = !_otherCol->GetActive();
		else isStart = (_playerHandler->artifactCount >= 3);

		if (isStart)
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