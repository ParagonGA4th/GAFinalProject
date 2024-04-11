#include "DemoPlayerAction.h"
#include "ArrowAction.h"
#include "../ParagonUtil/InputSystem.h"
#include "../ParagonData/Scene.h"
#include "../ParagonData/DynamicCollider.h"
#include <singleton-cpp/singleton.h>
#include <cassert>

	DemoPlayerAction::DemoPlayerAction(Pg::Data::GameObject* obj) : Pg::Data::Component(obj)
	{
		
	}

	void DemoPlayerAction::Start()
	{
		auto& tInput = singleton<Pg::Util::Input::InputSystem>();
		_input = &tInput;

		_collider = _object->GetComponent<Pg::Data::DynamicCollider>();
		assert(_collider != nullptr);

		for (auto& it : _object->GetScene()->FindObjectsWithTag("ArrowTag"))
		{
			auto t = it->GetComponent<ArrowAction>();
			assert(t != nullptr);
			_arrowObjects.push_back(t);
		}

		assert(!_arrowObjects.empty());
	}

	void DemoPlayerAction::Update()
	{
		for (auto& it : _arrowObjects)
		{
			it->_object->_transform._position = _object->_transform._position;
		}

		Pg::Math::PGFLOAT3 LOOK = _object->_transform.GetForward();
		float DISTANCE = 5.0f;

		LOOK = { LOOK.x * DISTANCE, LOOK.y * DISTANCE, LOOK.z * DISTANCE };

		if (_input->GetKey(Pg::API::Input::eKeyCode::KeyB))
		{
			_collider->AddForce({ LOOK }, Pg::Data::ForceMode::eFORCE);
		}
		if (_input->GetKey(Pg::API::Input::eKeyCode::KeyN))
		{
			_object->_transform._rotation = PGQuaternionMultiply(_object->_transform._rotation, { 0.9999619f, 0.f, 0.0087265f, 0.f });
			
			for (auto& it : _arrowObjects)
			{
				it->_object->_transform._rotation = _object->_transform._rotation;
			}
		}
		if (_input->GetKey(Pg::API::Input::eKeyCode::KeyM))
		{
			//Y축 기준 회전.
			_object->_transform._rotation = PGQuaternionMultiply(_object->_transform._rotation, { 0.9999619f, 0.f, -0.0087265f, 0.f });
			for (auto& it : _arrowObjects)
			{
				it->_object->_transform._rotation = _object->_transform._rotation;
			}
		}

		if (_input->GetKeyDown(Pg::API::Input::eKeyCode::Space))
		{
			_collider->AddForce({ 0,5,0 }, Pg::Data::ForceMode::eIMPULSE);
		}

		//_arrowObject->
		if (_input->GetKeyDown(Pg::API::Input::eKeyCode::KeyZ) && (!_arrowObjects.empty()))
		{
			ArrowAction* tAA = _arrowObjects.back();
			tAA->_restrainer = false;
			_arrowObjects.pop_back();
		}
		
	}


