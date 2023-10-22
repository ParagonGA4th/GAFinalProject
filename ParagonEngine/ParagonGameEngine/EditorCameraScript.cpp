#include "EditorCameraScript.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/Camera.h"

// 업데이트에서 임시로 인풋을 돌리기 위해...
#include "../ParagonGameEngine/InputSystem.h"

#include <singleton-cpp/singleton.h>
#include <cassert>

EditorCameraScript::EditorCameraScript(Pg::Data::GameObject* obj)
	:Pg::Data::Script(obj)
{

}

void EditorCameraScript::Start()
{
	// Input
	auto& tInputSystem = singleton<Pg::Engine::Input::InputSystem>();
	tInput = &tInputSystem;

	// Camera
	_camera = _object->GetComponent<Pg::Data::Camera>();
	assert(_camera != nullptr);
}

void EditorCameraScript::Update()
{
	using namespace Pg::Engine::Input;
	using namespace Pg::API::Input;

	if (tInput->GetKey(MoveFront))
	{
		Pg::Math::PGFLOAT3 LOOK = _object->_transform.GetForward();
		Pg::Math::PGFLOAT3 POS = _object->_transform.GetPosition();
		float DISTANCE = 0.01f; // 임시로 거리를 설정함 (distance * camera speed * deltaTime)

		LOOK = {LOOK.x * DISTANCE, LOOK.y * DISTANCE, LOOK.z * DISTANCE };
		POS += LOOK;

		_object->_transform.SetPosition(POS);
	}
	if (tInput->GetKey(MoveBack))
	{
		Pg::Math::PGFLOAT3 LOOK = _object->_transform.GetForward();
		Pg::Math::PGFLOAT3 POS = _object->_transform.GetPosition();
		float DISTANCE = -0.01f; // 임시로 거리를 설정함 (distance * camera speed * deltaTime)

		LOOK = { LOOK.x * DISTANCE, LOOK.y * DISTANCE, LOOK.z * DISTANCE };
		POS += LOOK;

		_object->_transform.SetPosition(POS);
	}
	if (tInput->GetKey(MoveLeft))
	{
		Pg::Math::PGFLOAT3 RIGHT = _object->_transform.GetRight();
		Pg::Math::PGFLOAT3 POS = _object->_transform.GetPosition();
		float DISTANCE = -0.01f; // 임시로 거리를 설정함 (distance * camera speed * deltaTime)

		RIGHT = { RIGHT.x * DISTANCE, RIGHT.y * DISTANCE, RIGHT.z * DISTANCE };
		POS += RIGHT;

		_object->_transform.SetPosition(POS);
	}
	if (tInput->GetKey(MoveRight))
	{
		Pg::Math::PGFLOAT3 RIGHT = _object->_transform.GetRight();
		Pg::Math::PGFLOAT3 POS = _object->_transform.GetPosition();
		float DISTANCE = 0.01f; // 임시로 거리를 설정함 (distance * camera speed * deltaTime)

		RIGHT = { RIGHT.x * DISTANCE, RIGHT.y * DISTANCE, RIGHT.z * DISTANCE };
		POS += RIGHT;

		_object->_transform.SetPosition(POS);
	}
	if (tInput->GetKey(MoveUp))
	{
		Pg::Math::PGFLOAT3 UP = _object->_transform.GetUp();
		Pg::Math::PGFLOAT3 POS = _object->_transform.GetPosition();
		float DISTANCE = 0.01f; // 임시로 거리를 설정함 (distance * camera speed * deltaTime)

		UP = { UP.x * DISTANCE, UP.y * DISTANCE, UP.z * DISTANCE };
		POS += UP;

		_object->_transform.SetPosition(POS);
	}
	if (tInput->GetKey(MoveDown))
	{
		Pg::Math::PGFLOAT3 UP = _object->_transform.GetUp();
		Pg::Math::PGFLOAT3 POS = _object->_transform.GetPosition();
		float DISTANCE = -0.01f; // 임시로 거리를 설정함 (distance * camera speed * deltaTime)

		UP = { UP.x * DISTANCE, UP.y * DISTANCE, UP.z * DISTANCE };
		POS += UP;

		_object->_transform.SetPosition(POS);
	}
	if (tInput->GetKey(MouseRight) && tInput->IsMouseMoving())
	{
		Pg::Math::PGFLOAT3 RIGHT	= _object->_transform.GetRight();
		Pg::Math::PGFLOAT3 UP		= _object->_transform.GetUp();
		Pg::Math::PGFLOAT3 FORWARD	= _object->_transform.GetForward();

		
		Pg::Math::PGFLOAT3 POS = _object->_transform.GetPosition();
		float ANGLE = 0.01f;
		
		Pg::Math::PGFLOAT4 r = { 1.0f,0.0f,0.0f,1.0f };
		r = Pg::Math::PGFloat4MultiplyMatrix(r, _object->_transform.GetLocalRotationMatrix());
		Pg::Math::PGQuaternion newRot = PGRotateQuaternion(_object->_transform.GetLocalRotation(), { r.x,r.y,r.z }, 1.f);
		_object->_transform.SetLocalRotation(newRot);

		//Pg::Math::PGFLOAT3 tLookDir = { 45.f, 0.f, 0.f };
		//_object->_transform.SetLocalRotationEuler(tLookDir);
		_object->_transform.SetPosition(POS);
	}



}
