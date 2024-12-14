#include "ArrowAction.h"
#include "../ParagonUtil/InputSystem.h"
#include "../ParagonData/Scene.h"
#include "../ParagonData/DynamicCollider.h"
#include "../ParagonMath/PgMath.h"
#include <singleton-cpp/singleton.h>
#include <cassert>

ArrowAction::ArrowAction(Pg::Data::GameObject* obj) : Pg::Data::Component(obj)
{
	auto& tInput = singleton<Pg::Util::Input::InputSystem>();
	_input = &tInput;

	//_collider = _object->GetComponent<Pg::Data::DynamicCollider>();
	//assert(_collider != nullptr);
}

void ArrowAction::Start()
{

}

void ArrowAction::Update()
{
	if (!_restrainer)
	{
		Pg::Math::PGFLOAT3 DIR = { _object->_transform.GetForward().x * 0.1f,
			0, _object->_transform.GetForward().z * 0.1f };

		//_object->_transform._rotation = Pg::Math::PGEulerToQuaternion(DIR);
		
		_object->_transform._position += DIR;
	}
}
