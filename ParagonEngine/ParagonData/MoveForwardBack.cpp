#include "MoveForwardBack.h"
#include "GameObject.h"
#include "../ParagonUtil/InputSystem.h"
#include "Transform.h"
#include "DynamicCollider.h"
#include <cmath>
#include <singleton-cpp/singleton.h>

namespace Pg::Data
{
	MoveForwardBack::MoveForwardBack(GameObject* obj) : Component(obj)
	{
		_col = _object->GetComponent<DynamicCollider>();

		auto& tInput = singleton<Pg::Util::Input::InputSystem>();
		_input = &tInput;
	}

	void MoveForwardBack::Update()
	{
		//tMoveVar += 0.0005f;
		//
		//Transform& tTrans = this->_object->_transform;
		//PGFLOAT3 tCurPos = tTrans._position;
		//
		//tTrans._position = { fmod(cos(tMoveVar), 2.0f) * 10, tCurPos.y, fmod(sin(tMoveVar), 2.0f) * 10 };
		//tTrans._rotation = tTrans.EulerToQuaternion(tMoveVar, tMoveVar, tMoveVar);

		if (_input->GetKeyDown(API::Input::KeyB))
		{
			_col->SetActive(false);
		}
		if (_input->GetKeyDown(API::Input::KeyN))
		{
			_col->SetActive(true);
		}



	}
}