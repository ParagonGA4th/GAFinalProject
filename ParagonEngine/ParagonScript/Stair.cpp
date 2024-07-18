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
		for (auto i : _object->GetScene()->FindObjectsWithTag("TAG_Stair"))
		{
			_renderers.emplace_back(i->GetComponent<Pg::Data::StaticMeshRenderer>());
		}

		_area = _object->GetScene()->FindObjectWithName("BattleArea_GolemBoss")->GetComponent<Pg::Data::SphereCollider>();
		assert(_area != nullptr);
	}

	void Stair::BeforePhysicsAwake()
	{

	}

	void Stair::Awake()
	{
		for (auto i : _renderers)
		{
			i->_object->_transform._position.y = -3.5f;
			i->SetActive(false);
			i->_object->GetComponent<Pg::Data::StaticBoxCollider>()->SetActive(false);
			//i->_alphaPercentage = 0.f;
		}
		_isGimickDone = false;
	}

	void Stair::Update()
	{
		if (!_area->GetActive() && !_isGimickDone)
		{
			auto renderer = _renderers.at(_index);
			renderer->SetActive(true);
			renderer->_object->GetComponent<Pg::Data::StaticBoxCollider>()->SetActive(true);

			if (renderer->_object->_transform._position.y >= _rollbackPos)
			{
				if (_index == 0) _rollbackPos = 10.098f;
				if (_index == 1) _rollbackPos = 11.140f;
				if (_index == 2) _rollbackPos = 12.439f;
				if (_index == 3)
				{
					_index = 0;
					_isGimickDone = true;
					return;
				}
				_index++;
			}
			else
			{
				renderer->_object->_transform._position.y += POSITION_PERCENT;
			}

			//if (_renderer->GetAlphaPercentage() >= 100.f)
			//else _renderer->SetAlphaPercentage(_renderer->GetAlphaPercentage() + ALPHA_PERCENT);
		}
	}
}