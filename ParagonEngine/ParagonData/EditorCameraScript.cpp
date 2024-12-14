#include "EditorCameraScript.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/Camera.h"
#include "../ParagonMath/PgMath.h"

// 업데이트에서 임시로 인풋을 돌리기 위해...
#include "../ParagonUtil/InputSystem.h"
#include "../ParagonUtil/TimeSystem.h"

#include <singleton-cpp/singleton.h>
#include <cassert>

//긴급 고치기 시도.
#include <DirectXMath.h>
#include <dxtk/SimpleMath.h>

namespace Pg::Data
{
	EditorCameraScript::EditorCameraScript(Pg::Data::GameObject* obj)
		:Pg::Data::Component(obj)
	{

	}

	void EditorCameraScript::Internal_EngineAwake()
	{
		// Input
		auto& tInputSystem = singleton<Pg::Util::Input::InputSystem>();
		tInput = &tInputSystem;

		// Time
		auto& tTimeSystem = singleton<Pg::Util::Time::TimeSystem>();
		tTime = &tTimeSystem;

		// Camera
		_camera = _object->GetComponent<Pg::Data::Camera>();
		assert(_camera != nullptr);
	}

	void EditorCameraScript::Internal_EngineUpdate()
	{
		using namespace Pg::Util::Input;
		using namespace Pg::API::Input;

		float _moveSpeed = _setMoveSpeed * tTime->GetDeltaTime();

		if (tInput->GetKey(MoveFront))
		{
			Pg::Math::PGFLOAT3 LOOK = _object->_transform.GetForward();
			Pg::Math::PGFLOAT3 POS = _object->_transform._position;
			float DISTANCE = _moveSpeed; // 임시로 거리를 설정함 (distance * camera speed * deltaTime)

			LOOK = { LOOK.x * DISTANCE, LOOK.y * DISTANCE, LOOK.z * DISTANCE };
			POS += LOOK;

			_object->_transform._position = POS;
		}
		if (tInput->GetKey(MoveBack))
		{
			Pg::Math::PGFLOAT3 LOOK = _object->_transform.GetForward();
			Pg::Math::PGFLOAT3 POS = _object->_transform._position;
			float DISTANCE = -_moveSpeed; // 임시로 거리를 설정함 (distance * camera speed * deltaTime)

			LOOK = { LOOK.x * DISTANCE, LOOK.y * DISTANCE, LOOK.z * DISTANCE };
			POS += LOOK;

			_object->_transform._position = POS;
		}
		if (tInput->GetKey(MoveLeft))
		{
			Pg::Math::PGFLOAT3 RIGHT = _object->_transform.GetRight();
			Pg::Math::PGFLOAT3 POS = _object->_transform._position;
			float DISTANCE = -_moveSpeed; // 임시로 거리를 설정함 (distance * camera speed * deltaTime)

			RIGHT = { RIGHT.x * DISTANCE, RIGHT.y * DISTANCE, RIGHT.z * DISTANCE };
			POS += RIGHT;

			_object->_transform._position = POS;
		}
		if (tInput->GetKey(MoveRight))
		{
			Pg::Math::PGFLOAT3 RIGHT = _object->_transform.GetRight();
			Pg::Math::PGFLOAT3 POS = _object->_transform._position;
			float DISTANCE = _moveSpeed; // 임시로 거리를 설정함 (distance * camera speed * deltaTime)

			RIGHT = { RIGHT.x * DISTANCE, RIGHT.y * DISTANCE, RIGHT.z * DISTANCE };
			POS += RIGHT;

			_object->_transform._position = POS;
		}
		if (tInput->GetKey(KeyE))
		{
			Pg::Math::PGFLOAT3 UP = _object->_transform.GetUp();
			Pg::Math::PGFLOAT3 POS = _object->_transform._position;
			float DISTANCE = _moveSpeed; // 임시로 거리를 설정함 (distance * camera speed * deltaTime)

			UP = { UP.x * DISTANCE, UP.y * DISTANCE, UP.z * DISTANCE };
			POS += UP;

			_object->_transform._position = POS;
		}
		if (tInput->GetKey(KeyQ))
		{
			Pg::Math::PGFLOAT3 UP = _object->_transform.GetUp();
			Pg::Math::PGFLOAT3 POS = _object->_transform._position;
			float DISTANCE = -_moveSpeed; // 임시로 거리를 설정함 (distance * camera speed * deltaTime)

			UP = { UP.x * DISTANCE, UP.y * DISTANCE, UP.z * DISTANCE };
			POS += UP;

			_object->_transform._position = POS;
		}

		if (tInput->GetKey(MouseRight) && tInput->IsMouseMoving())
		{
			using namespace Pg::Math;
			//RotateY(3.0f * tInput->GetMouseDX());
			//Pitch(3.0f * tInput->GetMouseDY());
			RotateFix(3.0f * tInput->GetMouseDX(), 3.0f * tInput->GetMouseDY());
			//RotateFix(-3.0f * tInput->GetMouseDX(), -3.0f * tInput->GetMouseDY());
			//RotateFix(3.0f * tInput->GetMouseDX(), 0.f);
			//RotateFix(0.f, 3.0f * tInput->GetMouseDY());
		}

		//돌아다니는 속도, ShiftL로 조정할 수 있게!

		if (tInput->GetKeyDown(ShiftL))
		{
			_setMoveSpeed *= 2.f;
		}
		if (tInput->GetKeyUp(ShiftL))
		{
			_setMoveSpeed /= 2.f;
		}

		//이건 그래픽 디버깅용.
		if (tInput->GetKeyDown(ShiftR))
		{
			_object->_transform._position = { 0,0,0 };
			_object->_transform._rotation = { 1,0,0,0 };
		}
	}

	//Now Defunct
	void EditorCameraScript::RotateY(float angle)
	{
		using namespace Pg::Math;

		PGQuaternion tOldRotQuat = _object->_transform._rotation;

		PGQuaternion tRotatedQuat = PGRotateQuaternion(tOldRotQuat, PGFLOAT3(0.f, 1.f, 0.f), -angle);
		tRotatedQuat = PGQuaternionNormalize(tRotatedQuat);

		_object->_transform._rotation = tRotatedQuat;
	}

	//Now Defunct
	void EditorCameraScript::Pitch(float angle)
	{
		using namespace Pg::Math;

		PGQuaternion tOldRotQuat = _object->_transform._rotation;

		PGQuaternion tRotatedQuat = PGRotateQuaternion(tOldRotQuat, PGFLOAT3(1.f, 0.f, 0.f), -angle);

		tRotatedQuat = PGQuaternionNormalize(tRotatedQuat);

		_object->_transform._rotation = tRotatedQuat;
	}

	void EditorCameraScript::RotateFix(float yaw, float pitch)
	{
		using namespace Pg::Math;
		using namespace DirectX::SimpleMath;

		Vector3 tLocalUpVector = { _object->_transform.GetUp().x, _object->_transform.GetUp().y, _object->_transform.GetUp().z };
		Vector3 tLocalRightVector = { _object->_transform.GetRight().x, _object->_transform.GetRight().y, _object->_transform.GetRight().z };
		Vector3 tLocalForwardVector = { _object->_transform.GetForward().x, _object->_transform.GetForward().y, _object->_transform.GetForward().z };

		tLocalUpVector.Normalize();
		tLocalRightVector.Normalize();
		tLocalForwardVector.Normalize();

		//RotateY (Yaw)
		{
			DirectX::XMMATRIX R = DirectX::XMMatrixRotationY(yaw);

			DirectX::XMStoreFloat3(&tLocalRightVector, DirectX::XMVector3TransformNormal(DirectX::XMLoadFloat3(&tLocalRightVector), R));
			DirectX::XMStoreFloat3(&tLocalUpVector, DirectX::XMVector3TransformNormal(DirectX::XMLoadFloat3(&tLocalUpVector), R));
			DirectX::XMStoreFloat3(&tLocalForwardVector, DirectX::XMVector3TransformNormal(DirectX::XMLoadFloat3(&tLocalForwardVector), R));
		}

		//Pitch
		{
			DirectX::XMMATRIX R = DirectX::XMMatrixRotationAxis(DirectX::XMLoadFloat3(&tLocalRightVector), pitch);

			DirectX::XMStoreFloat3(&tLocalUpVector, DirectX::XMVector3TransformNormal(DirectX::XMLoadFloat3(&tLocalUpVector), R));
			DirectX::XMStoreFloat3(&tLocalForwardVector, DirectX::XMVector3TransformNormal(DirectX::XMLoadFloat3(&tLocalForwardVector), R));
		}

		//정규직교화.
		{
			DirectX::XMVECTOR U = DirectX::XMLoadFloat3(&tLocalUpVector);
			DirectX::XMVECTOR L = DirectX::XMLoadFloat3(&tLocalForwardVector);
			DirectX::XMVECTOR R = DirectX::XMLoadFloat3(&tLocalRightVector);

			// Keep camera's axes orthogonal to each other and of unit length.
			L = DirectX::XMVector3Normalize(L);
			U = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(L, R));

			// U, L already ortho-normal, so no need to normalize cross product.
			R = DirectX::XMVector3Cross(U, L);

			DirectX::XMStoreFloat3(&tLocalRightVector, R);
			DirectX::XMStoreFloat3(&tLocalUpVector, U);
			DirectX::XMStoreFloat3(&tLocalForwardVector, L);
		}

		//Rotation Matrix에서 DirectX Quaternion으로.
		DirectX::XMFLOAT4 tReturn;
		{
			//Forward Vector 반대로 뒤집지 않고 처리.
			DirectX::XMMATRIX rotationMatrix(
				tLocalRightVector.x, tLocalUpVector.x, tLocalForwardVector.x, 0.0f,
				tLocalRightVector.y, tLocalUpVector.y, tLocalForwardVector.y, 0.0f,
				tLocalRightVector.z, tLocalUpVector.z, tLocalForwardVector.z, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			);

			DirectX::XMVECTOR quaternion = DirectX::XMQuaternionRotationMatrix(rotationMatrix);
			DirectX::XMStoreFloat4(&tReturn, quaternion);
		}

		PGQuaternion tNewRotQuat = { tReturn.w, tReturn.x, tReturn.y, tReturn.z };
		_object->_transform._rotation = tNewRotQuat;
	}




}