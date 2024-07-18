#include "Stage2_Bridge.h"
#include "BridgeHelper.h"

#include "../ParagonData/Scene.h"
#include "../ParagonData/StaticMeshRenderer.h"  
#include "../ParagonData/StaticBoxCollider.h"

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

		//auto obj = _object->GetScene()->FindObjectWithName("BridgeHelper");
		//if (obj != nullptr)
		//{
		//	_bridgeHelper = obj->GetComponent<Pg::DataScript::BridgeHelper>();
		//	assert(_bridgeHelper != nullptr);
		//}
	}

	void Stage2_Bridge::BeforePhysicsAwake()
	{

	}

	void Stage2_Bridge::Awake()
	{
		for (auto i : _renderers)
		{
			//i->_object->_transform._position.y = -3.f;
			//i->SetActive(false);
			i->_alphaPercentage = 0.f;
		}

		//_collider->SetActive(false);
	}

	void Stage2_Bridge::Update()
	{
		//if (_bridgeHelper->_isTrig)
		//{
		//	//auto renderer = _renderers.at(_index);
		//	//renderer->SetActive(true);

		//	//if (renderer->_object->_transform._position.y >= _rollbackPos)
		//	//{
		//	//	if (_index == 2) _rollbackPos = 4.024f;
		//	//	if (_index == 3)
		//	//	{
		//	//		_index = 0;
		//	//		_bridgeHelper->_isTrig = false;

		//	//		_collider->SetActive(true);
		//	//	}

		//	//	_index++;
		//	//}

		//	//renderer->_object->_transform._position.y += POSITION_PERCENT;

		//	for (auto ren : _renderers)
		//	{
		//		if (ren->_alphaPercentage >= 100.f)
		//		{
		//			_collider->SetActive(true);
		//		}
		//		else
		//		{
		//			ren->_alphaPercentage += ALPHA_PERCENT;
		//		}
		//	}
		//}
	}
}
