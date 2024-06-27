#include "InGameCameraBehavior.h"
#include "../ParagonData/Transform.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/Scene.h"
#include "../ParagonData/Camera.h"
#include "../ParagonAPI/PgTime.h"
#include "../ParagonAPI/PgInput.h"
#include "../ParagonUtil/Log.h"

#include <cassert>
#include <FastNoiseLite/FastNoiseLite.h>
#include <singleton-cpp/singleton.h>
#include <windows.h>

namespace Pg::DataScript
{
	InGameCameraBehavior::InGameCameraBehavior(Pg::Data::GameObject* obj) : ScriptInterface(obj)
	{
		_pgTime = &singleton<Pg::API::Time::PgTime>();
		//_pgInput = &singleton<Pg::API::Input::PgInput>();
	}

	void InGameCameraBehavior::Awake()
	{
		using namespace Pg::Math;

		//자신이 속한 Camera Component를 찾아 보관한다.
		_selfCamera = _object->GetComponent<Pg::Data::Camera>();
		assert(_selfCamera != nullptr);

		//Tag를 기준으로 찾는다.
		auto plVec = _object->GetScene()->FindObjectsWithTag("TAG_Player");
		//플레이어는 한명만 있어야 한다.
		assert(plVec.size() == 1 && "플레이어 객체는 단 하나만 발견되어야!");
		//플레이어 Transform 보관.
		_playerTransform = plVec.at(0)->GetComponent<Pg::Data::Transform>();

		//현재 자기 자신의 Rotation 세팅. (밑으로 약 40도 바라봐야)

		//45도로 맞춘거.
		//_object->_transform._rotation = PGEulerToQuaternion({ PGConvertToRadians(-20.161f), PGConvertToRadians(-44.745f), PGConvertToRadians(18.860f) });
		//_object->_transform._rotation = PGEulerToQuaternion({ PGConvertToRadians(-40),0, 0 });
		_object->_transform._rotation = PGEulerToQuaternion({ 0, 0, 0 });

		//자기 자신이 속한 Camera를 MainCamera로 설정.
		_object->GetScene()->SetMainCamera(_selfCamera);
	}

	void InGameCameraBehavior::OnDeserialize(SerializeVector& sv)
	{
		Pg::Data::SerializerHelper::OnDeserializerHelper(this, sv);
	}

	void InGameCameraBehavior::OnSerialize(SerializeVector& sv)
	{
		Pg::Data::SerializerHelper::OnSerializerHelper(this, sv);
	}

	void InGameCameraBehavior::Start()
	{

	}

	void InGameCameraBehavior::FixedUpdate()
	{
		UpdateTargetTransforms();
		LerpFollowPlayer();

		//if (GetAsyncKeyState(VK_F8) & 0x8000)
		//{
		//	_object->_transform._rotation = Pg::Math::PGRotateQuaternion(_object->_transform._rotation, { 0,1,0 }, 0.1);
		//}
		//if (GetAsyncKeyState(VK_F9) & 0x8000)
		//{
		//	_object->_transform._rotation = Pg::Math::PGRotateQuaternion(_object->_transform._rotation, { 1,0,0 }, 0.1);
		//}
		//if (GetAsyncKeyState(VK_F10) & 0x8000)
		//{
		//	_object->_transform._rotation = Pg::Math::PGRotateQuaternion(_object->_transform._rotation, { 0,0,1 }, 0.1);
		//}
		//if (GetAsyncKeyState(VK_F11) & 0x8000)
		//{
		//	_object->_transform._rotation = Pg::Math::PGRotateQuaternion(_object->_transform._rotation, _object->_transform.GetUp(), 0.1);
		//}
		//if (GetAsyncKeyState(VK_LSHIFT) & 0x8000)
		//{
		//	std::string tVal = std::to_string(_object->_transform._rotation.w);
		//	tVal += ", ";
		//	tVal += std::to_string(_object->_transform._rotation.x);
		//	tVal += ", ";
		//	tVal += std::to_string(_object->_transform._rotation.y);
		//	tVal += ", ";
		//	tVal += std::to_string(_object->_transform._rotation.z);
		//	PG_TRACE(tVal.c_str());
		//}


	}

	void InGameCameraBehavior::LerpFollowPlayer()
	{
		float interpolation = std::clamp<float>(_speed * _pgTime->GetDeltaTime(), 0.f, 1.f);
		float faster_interpolation = std::clamp<float>(_speed * 3.f * _pgTime->GetDeltaTime(), 0.f, 1.f);

		//Position
		Pg::Math::PGFLOAT3 tPosition = _object->_transform._position;
		tPosition = Pg::Math::PGFloat3Lerp(_object->_transform._position, _targetCamPosition, interpolation);
		_object->_transform._position = tPosition;

		//Rotation
		_object->_transform._rotation = PGQuaternionSlerp(_object->_transform._rotation, _targetCamRotation, faster_interpolation);
	}

	void InGameCameraBehavior::UpdateTargetTransforms()
	{
		using namespace Pg::Math;
		using namespace DirectX;

		//Target Position 설정. -> 나중에 Lerp할 때 반영될 것이다. 초기 상태 세팅.
		_targetCamPosition.x = _playerTransform->_position.x + _camOffset.x;
		_targetCamPosition.y = _playerTransform->_position.y + _camOffset.y;
		_targetCamPosition.z = _playerTransform->_position.z + _camOffset.z;

		//Rotation은 바로 반영이 된다. -> 일단 초기 상태를 세팅.
		
		//계산을 반영할 것.
		XMVECTOR position = XMVectorSet(_targetCamPosition.x, _targetCamPosition.y, _targetCamPosition.z, 1.0f);
		XMVECTOR rotation = PG2XM_QUATERNION_VECTOR(_object->_transform._rotation);
		//Presave.
		{
			//내부 Rotate Around 계산. - Position.
			//Player의 X and Z는 받되, Y는 자신으로 유지.
			//Y축을 기준으로 돌 것이다.
			XMVECTOR pivotPoint = XMVectorSet(_playerTransform->_position.x, _object->_transform._position.y, _playerTransform->_position.z, 1.0f);
			XMVECTOR rotationAxis = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

			//이게 실질적으로 Transform.RotateAround이랑 같을 것이다.
			//_currentRotationAmt는 나중에 FollowLogic이 나오면 이를 기반으로 동작할 수 있을 것.

			//if (GetAsyncKeyState(VK_LSHIFT) & 0x8000)
			//{
			//	_currentRotationAmt += 1.f;
			//}

			float rotationAngle = XMConvertToRadians(fmod(_currentRotation, 360.f));

			//Rotation Axis & Angle.
			XMMATRIX rotationMatrix = XMMatrixRotationAxis(rotationAxis, rotationAngle);

			//현재 Forward Direction 보존.
			XMVECTOR forward = XMVector3Rotate(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), rotation);

			//Pivot Point로 오브젝트 Translate.
			XMMATRIX translationToPivot = XMMatrixTranslationFromVector(XMVectorNegate(pivotPoint));
			position = XMVector3Transform(position, translationToPivot);

			//Pivot Point 중심으로 Object Rotation.
			XMMATRIX rotationAroundPivot = rotationMatrix;
			position = XMVector3Transform(position, rotationAroundPivot);

			//오브젝트를 Pivot Point와 상대적으로, 원위치를 향해 Translate.
			XMMATRIX translationFromPivot = XMMatrixTranslationFromVector(pivotPoint);
			position = XMVector3Transform(position, translationFromPivot);
		}

		//if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		//{
		//	_lookDownAngle += 0.5f;
		//}

		//TargetRotation으로 역 대입, 나중에 보간될 것.
		Pg::Math::PGFLOAT3 tSameYPT = { _playerTransform->_position.x, _object->_transform._position.y, _playerTransform->_position.z };
		Pg::Math::PGFLOAT3 tLookVector = -PGFloat3Normalize(tSameYPT - _object->_transform._position);
		_targetCamRotation = PGLookRotation(tLookVector, Pg::Math::PGFLOAT3::GlobalUp());
		//Y축 대한 Flip:  
		_targetCamRotation.w *= -1.0f;
		DirectX::XMVECTOR tCurrentCamRot = PG2XM_QUATERNION_VECTOR(_targetCamRotation);
		
		DirectX::XMVECTOR tTemp = DirectX::XMQuaternionRotationRollPitchYaw(-PGConvertToRadians(_lookDownAngle), 0, 0);
		_targetCamRotation = XM2PG_QUATERNION(DirectX::XMQuaternionMultiply(tCurrentCamRot, tTemp));

		//TargetPosition으로 역 대입, 나중에 보간될 것.
		_targetCamPosition = XM2PG_FLOAT3_VECTOR(position);
	}

	Pg::Math::PGFLOAT3 InGameCameraBehavior::GetTargetCamPosition()
	{
		return _targetCamPosition;
	}



}