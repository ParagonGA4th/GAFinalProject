#include "Stage2_Bridge.h"
#include "BridgeHelper.h"

#include "../ParagonData/Scene.h"
#include "../ParagonData/StaticMeshRenderer.h"  
#include "../ParagonData/StaticBoxCollider.h"
#include "../ParagonData/SphereCollider.h"

#include "../ParagonUtil/Log.h"

namespace Pg::DataScript
{
	Stage2_Bridge::Stage2_Bridge(Pg::Data::GameObject* obj)
		:ScriptInterface(obj)
	{

	}

	void Stage2_Bridge::GrabManagedObjects()
	{
		for (auto i : _object->GetScene()->FindObjectsWithTag("TAG_Bridge"))
		{
			if (i->GetName().compare("BridgeCol") == 0) _collider = i->GetComponent<Pg::Data::StaticBoxCollider>();
			else _renderers.emplace_back(i->GetComponent<Pg::Data::StaticMeshRenderer>());
		}

		_area = _object->GetScene()->FindObjectWithName("BattleArea_1")->GetComponent<Pg::Data::SphereCollider>();
		assert(_area != nullptr);
	}

	void Stage2_Bridge::BeforePhysicsAwake()
	{

	}

	void Stage2_Bridge::Awake()
	{
		for (auto i : _renderers)
		{
			//i->_object->_transform._position.y = -3.f;
			i->SetActive(false);
			i->_alphaPercentage = 0.f;
		}

		_collider->SetActive(false);
	}

	void Stage2_Bridge::Update()
	{
		if (!_area->GetActive())
		{
			for (auto ren : _renderers)
			{
				ren->SetActive(true);

				if (ren->_alphaPercentage >= 100.f)
				{
					_collider->SetActive(true);
				}
				else
				{
					ren->_alphaPercentage += ALPHA_PERCENT;
				}
			}
		}
	}
}
