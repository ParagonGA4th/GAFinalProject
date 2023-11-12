#include "EditorCameraScript.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/Camera.h"
#include "../ParagonMath/PgMath.h"

// 업데이트에서 임시로 인풋을 돌리기 위해...
#include "../ParagonGameEngine/InputSystem.h"

#include <singleton-cpp/singleton.h>
#include <cassert>

//긴급 고치기 시도.
#include <DirectXMath.h>
#include <dxtk/SimpleMath.h>

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
		float DISTANCE = _moveSpeed; // 임시로 거리를 설정함 (distance * camera speed * deltaTime)

		LOOK = {LOOK.x * DISTANCE, LOOK.y * DISTANCE, LOOK.z * DISTANCE };
		POS += LOOK;

		_object->_transform.SetPosition(POS);
	}
	if (tInput->GetKey(MoveBack))
	{
		Pg::Math::PGFLOAT3 LOOK = _object->_transform.GetForward();
		Pg::Math::PGFLOAT3 POS = _object->_transform.GetPosition();
		float DISTANCE = -_moveSpeed; // 임시로 거리를 설정함 (distance * camera speed * deltaTime)

		LOOK = { LOOK.x * DISTANCE, LOOK.y * DISTANCE, LOOK.z * DISTANCE };
		POS += LOOK;

		_object->_transform.SetPosition(POS);
	}
	if (tInput->GetKey(MoveLeft))
	{
		Pg::Math::PGFLOAT3 RIGHT = _object->_transform.GetRight();
		Pg::Math::PGFLOAT3 POS = _object->_transform.GetPosition();
		float DISTANCE = -_moveSpeed; // 임시로 거리를 설정함 (distance * camera speed * deltaTime)

		RIGHT = { RIGHT.x * DISTANCE, RIGHT.y * DISTANCE, RIGHT.z * DISTANCE };
		POS += RIGHT;

		_object->_transform.SetPosition(POS);
	}
	if (tInput->GetKey(MoveRight))
	{
		Pg::Math::PGFLOAT3 RIGHT = _object->_transform.GetRight();
		Pg::Math::PGFLOAT3 POS = _object->_transform.GetPosition();
		float DISTANCE = _moveSpeed; // 임시로 거리를 설정함 (distance * camera speed * deltaTime)

		RIGHT = { RIGHT.x * DISTANCE, RIGHT.y * DISTANCE, RIGHT.z * DISTANCE };
		POS += RIGHT;

		_object->_transform.SetPosition(POS);
	}
	if (tInput->GetKey(MoveUp))
	{
		Pg::Math::PGFLOAT3 UP = _object->_transform.GetUp();
		Pg::Math::PGFLOAT3 POS = _object->_transform.GetPosition();
		float DISTANCE = _moveSpeed; // 임시로 거리를 설정함 (distance * camera speed * deltaTime)

		UP = { UP.x * DISTANCE, UP.y * DISTANCE, UP.z * DISTANCE };
		POS += UP;

		_object->_transform.SetPosition(POS);
	}
	if (tInput->GetKey(MoveDown))
	{
		Pg::Math::PGFLOAT3 UP = _object->_transform.GetUp();
		Pg::Math::PGFLOAT3 POS = _object->_transform.GetPosition();
		float DISTANCE = -_moveSpeed; // 임시로 거리를 설정함 (distance * camera speed * deltaTime)

		UP = { UP.x * DISTANCE, UP.y * DISTANCE, UP.z * DISTANCE };
		POS += UP;

		_object->_transform.SetPosition(POS);
	}
	if (tInput->GetKey(MouseRight) && tInput->IsMouseMoving())
	{
		using namespace Pg::Math;
		RotateY(3.0f * tInput->GetMouseDX());
		//Pitch(3.0f * tInput->GetMouseDY());
		//RotateAllTry(3.0f * tInput->GetMouseDX(), 0.f);
		//RotateAllTry(0.f, 3.0f * tInput->GetMouseDY());
		//RotateAllTry(3.0f * tInput->GetMouseDX(), 3.0f * tInput->GetMouseDY());
	}
	else
	{
		//if (tInput->GetKey(MouseLeft) && tInput->IsMouseMoving())
		//{
		//	using namespace Pg::Math;
		//	Pitch(3.0f * tInput->GetMouseDY());
		//	//RotateY(3.0f * tInput->GetMouseDX());
		//}
	}
}

void EditorCameraScript::RotateY(float angle)
{
	using namespace Pg::Math;

	PGQuaternion tOldRotQuat = _object->_transform.GetLocalRotation();

	PGQuaternion tRotatedQuat = PGRotateQuaternion(tOldRotQuat, PGFLOAT3(0.f,1.f,0.f), -angle);
	tRotatedQuat = PGQuaternionNormalize(tRotatedQuat);

	_object->_transform.SetLocalRotation(tRotatedQuat);
}

void EditorCameraScript::Pitch(float angle)
{
	using namespace Pg::Math;

	PGQuaternion tOldRotQuat = _object->_transform.GetLocalRotation();
	
	PGQuaternion tRotatedQuat = PGRotateQuaternion(tOldRotQuat, PGFLOAT3(1.f, 0.f, 0.f), -angle);

	tRotatedQuat = PGQuaternionNormalize(tRotatedQuat);

	_object->_transform.SetLocalRotation(tRotatedQuat);	
}

void EditorCameraScript::RotateAllTry(float yaw, float pitch)
{
	using namespace Pg::Math;
	using namespace DirectX::SimpleMath;

	Vector3 tLocalUpVector = { _object->_transform.GetUp().x, _object->_transform.GetUp().y, _object->_transform.GetUp().z };
	Vector3 tLocalRightVector = { _object->_transform.GetRight().x, _object->_transform.GetRight().y, _object->_transform.GetRight().z };

	Quaternion qPitch = Quaternion::CreateFromAxisAngle(tLocalRightVector, pitch);
	//Quaternion qPitch = Quaternion::CreateFromAxisAngle({1.f, 0.f, 0.f}, pitch);
	//Quaternion qYaw = Quaternion::CreateFromAxisAngle(tLocalUpVector, yaw);
	Quaternion qYaw = Quaternion::CreateFromAxisAngle({0.f, 1.f, 0.f}, yaw);

	Quaternion qFinal = qPitch * qYaw;
	qFinal.Normalize();

	PGQuaternion tOldRotQuat = _object->_transform.GetLocalRotation();
	Quaternion tSMOldRotQuat = { tOldRotQuat.x, tOldRotQuat.y, tOldRotQuat.z, tOldRotQuat.w };
	tSMOldRotQuat.Normalize();

	Quaternion tReturn = qFinal * tSMOldRotQuat;
	//Quaternion tReturn = tSMOldRotQuat * qFinal;
	tReturn.Normalize();

	PGQuaternion tNewRotQuat = { tReturn.w, tReturn.x, tReturn.y, tReturn.z };
	_object->_transform.SetLocalRotation(tNewRotQuat);
}





