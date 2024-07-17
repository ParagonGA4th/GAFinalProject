#include "Bridge.h"

#include "../ParagonData/Scene.h"
#include "../ParagonData/StaticMeshRenderer.h"  
#include "../ParagonData/StaticBoxCollider.h"

namespace Pg::DataScript
{

	Bridge::Bridge(Pg::Data::GameObject* obj)
		:ScriptInterface(obj)
	{

	}

	void Bridge::GrabManagedObjects()
	{
		for (auto i : _object->GetScene()->FindObjectsWithTag("TAG_Bridge"))
		{
			if (i->GetName().compare("BridgeCol") == 0) _collider = i->GetComponent<Pg::Data::StaticBoxCollider>();
			else _renderers.emplace_back(i->GetComponent<Pg::Data::StaticMeshRenderer>());
		}
	}

	void Bridge::BeforePhysicsAwake()
	{

	}

	void Bridge::Awake()
	{
		for (auto i : _renderers)
		{
			i->SetActive(false);
		}

		_collider->SetActive(false);
	}

	void Bridge::Update()
	{

	}
}