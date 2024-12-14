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

		//�ڽ��� ���� Camera Component�� ã�� �����Ѵ�.
		_selfCamera = _object->GetComponent<Pg::Data::Camera>();
		assert(_selfCamera != nullptr);

		//Tag�� �������� ã�´�.
		auto plVec = _object->GetScene()->FindObjectsWithTag("TAG_Player");
		//�÷��̾�� �Ѹ� �־�� �Ѵ�.
		assert(plVec.size() == 1 && "�÷��̾� ��ü�� �� �ϳ��� �߰ߵǾ��!");
		//�÷��̾� Transform ����.
		_playerTransform = plVec.at(0)->GetComponent<Pg::Data::Transform>();

		//���� �ڱ� �ڽ��� Rotation ����. (������ �� 40�� �ٶ����)

		//45���� �����.
		//_object->_transform._rotation = PGEulerToQuaternion({ PGConvertToRadians(-20.161f), PGConvertToRadians(-44.745f), PGConvertToRadians(18.860f) });
		//_object->_transform._rotation = PGEulerToQuaternion({ PGConvertToRadians(-40),0, 0 });
		_object->_transform._rotation = PGEulerToQuaternion({ 0, 0, 0 });

		//�ڱ� �ڽ��� ���� Camera�� MainCamera�� ����.
		_object->GetScene()->SetMainCamera(_selfCamera);

		//�ڱ� �ڽ��� ���� ���� �̸��� �������, � ������Ʈ�� �ؾ� ������ ���Ѵ�.
		std::string tBelongSceneName = _object->GetScene()->GetSceneName();
		InGameCameraBehavior* tBased = this;
		if (tBelongSceneName.compare("BossStage") == 0)
		{
			_cameraUpdateMainFunc = std::bind(&InGameCameraBehavior::FixCameraMode, tBased);
		}
		else
		{
			_cameraUpdateMainFunc = std::bind(&InGameCameraBehavior::Default_PlayerFollowMode, tBased);
		}

		//������ �� ���ε�.
		assert(_cameraUpdateMainFunc);
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
		//Default_PlayerFollowMode();
		//Boss_RotateAroundMode();

		//Script ��ü���� �����ϰ� �� ���̴�.
		_cameraUpdateMainFunc();

		if (GetAsyncKeyState(VK_F8) & 0x8000)
		{
			auto pCamera = _object->GetScene()->FindObjectsWithTag("TAG_EditorCamera");
			auto cameraObj = pCamera.at(0)->GetComponent<Pg::Data::Camera>();
			if (cameraObj != nullptr) _object->GetScene()->SetMainCamera(cameraObj);
		}
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

	void InGameCameraBehavior::Default_PlayerFollowMode()
	{
		UpdateTargetTransforms();
		LerpFollowPlayer();
	}

	void InGameCameraBehavior::Boss_RotateAroundMode()
	{
		//const float tAboveValue = 15.f;
		//const float tDistance = 15.0f;
		//static float tRotAmount = 270.f;
		//static float tLookDownRotTemp = 60.f;

		float cameraDistance = 25.0f; // CENTER OF CIRCLE���� ������ ����
		float cameraHeight = 20.0f;    // XZ Plane �� Height��.

		DirectX::XMVECTOR position = DirectX::XMVectorZero();
		DirectX::XMVECTOR rotationQuaternion = DirectX::XMVectorZero();

		using namespace DirectX;
		XMFLOAT3 playerPosition = Pg::Math::PG2XM_FLOAT3(_playerTransform->_position);
		XMFLOAT3 basePosition = PG2XM_FLOAT3(CENTER_OF_BOSS_STAGE_CIRCLE);

		// CENTEROFCIRCLE���� �÷��̾�� ���� ���� ����
		//XMVECTOR baseToPlayer = XMVectorSubtract(XMLoadFloat3(&playerPosition), XMLoadFloat3(&basePosition));
		XMVECTOR baseToPlayer = XMVectorSubtract(XMLoadFloat3(&basePosition), XMLoadFloat3(&playerPosition));

		float tDistSq = XMVectorGetX(XMVector3LengthSq(baseToPlayer));

		//DistanceSquared�� �߾����κ��� ���� �Ÿ� �̻��� ���� ���� ó���� ����.
		//const float ERROR_PROOF_DISTANCE_SQUARED = 4.0f;
		//PG_WARN(tDistSq);
		//if (tDistSq < ERROR_PROOF_DISTANCE_SQUARED)
		//{
		//	return;
		//}

		//������ XZ Plane���� �翵 (Y ����)
		XMVECTOR direction = XMVectorSetY(baseToPlayer, 0.0f);
		direction = XMVector3Normalize(direction);

		// ī�޶� ��ġ ���.
		position = XMLoadFloat3(&basePosition) - direction * cameraDistance;
		position = XMVectorSetY(position, XMVectorGetY(position) + cameraHeight);

		// Rotation ���.
		XMVECTOR upVector = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		XMVECTOR forwardVector = XMVector3Normalize(XMLoadFloat3(&basePosition) - position);
		XMVECTOR rightVector = XMVector3Cross(upVector, forwardVector);

		XMMATRIX rotationMatrix = XMMatrixLookToLH(position, forwardVector, upVector);
		rotationQuaternion = XMQuaternionRotationMatrix(rotationMatrix);


		//�̷��� ��������� �Ѵ�.
		_targetCamRotation = Pg::Math::XM2PG_QUATERNION(rotationQuaternion);
		_targetCamPosition = Pg::Math::XM2PG_FLOAT3_VECTOR(position);

		//TargetPosition���� �� ����, ���߿� ������ ��.
		float interpolation = std::clamp<float>(_speed * _pgTime->GetDeltaTime(), 0.f, 1.f);
		float faster_interpolation = std::clamp<float>(_speed * 3.f * _pgTime->GetDeltaTime(), 0.f, 1.f);

		//Position
		Pg::Math::PGFLOAT3 tPosition = _object->_transform._position;
		tPosition = Pg::Math::PGFloat3Lerp(_object->_transform._position, _targetCamPosition, interpolation);
		_object->_transform._position = tPosition;

		//Rotation
		_object->_transform._rotation = PGQuaternionSlerp(_object->_transform._rotation, _targetCamRotation, faster_interpolation);

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

	void InGameCameraBehavior::FixCameraMode()
	{
		using namespace Pg::Math;

		//ī�޶� rotation�� position�� ����
		Pg::Math::PGFLOAT3 fixedPosition = _fixedPos;
		Pg::Math::PGQuaternion fixedRotation = Pg::Math::PGEulerToQuaternion({ PGConvertToRadians(_fixedRot.x), PGConvertToRadians(_fixedRot.y), PGConvertToRadians(_fixedRot.z) }); // Example fixed rotation

		_object->_transform._position = fixedPosition;
		_object->_transform._rotation = fixedRotation;

		_targetCamPosition = fixedPosition;
		_targetCamRotation = fixedRotation;
	}

	void InGameCameraBehavior::UpdateTargetTransforms()
	{
		using namespace Pg::Math;
		using namespace DirectX;

		//Target Position ����. -> ���߿� Lerp�� �� �ݿ��� ���̴�. �ʱ� ���� ����.
		_targetCamPosition.x = _playerTransform->_position.x + _camOffset.x;
		_targetCamPosition.y = _playerTransform->_position.y + _camOffset.y;
		_targetCamPosition.z = _playerTransform->_position.z + _camOffset.z;

		//Rotation�� �ٷ� �ݿ��� �ȴ�. -> �ϴ� �ʱ� ���¸� ����.

		//����� �ݿ��� ��.
		XMVECTOR position = XMVectorSet(_targetCamPosition.x, _targetCamPosition.y, _targetCamPosition.z, 1.0f);
		XMVECTOR rotation = PG2XM_QUATERNION_VECTOR(_object->_transform._rotation);
		//Presave.
		{
			//���� Rotate Around ���. - Position.
			//Player�� X and Z�� �޵�, Y�� �ڽ����� ����.
			//Y���� �������� �� ���̴�.
			XMVECTOR pivotPoint = XMVectorSet(_playerTransform->_position.x, _object->_transform._position.y, _playerTransform->_position.z, 1.0f);
			XMVECTOR rotationAxis = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

			//�̰� ���������� Transform.RotateAround�̶� ���� ���̴�.
			//_currentRotationAmt�� ���߿� FollowLogic�� ������ �̸� ������� ������ �� ���� ��.
			if (GetAsyncKeyState(VK_LSHIFT) & 0x8000)
			{
				_currentRotation += 1.f;
			}

			float rotationAngle = XMConvertToRadians(fmod(_currentRotation, 360.f));

			//Rotation Axis & Angle.
			XMMATRIX rotationMatrix = XMMatrixRotationAxis(rotationAxis, rotationAngle);

			//���� Forward Direction ����.
			XMVECTOR forward = XMVector3Rotate(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), rotation);

			//Pivot Point�� ������Ʈ Translate.
			XMMATRIX translationToPivot = XMMatrixTranslationFromVector(XMVectorNegate(pivotPoint));
			position = XMVector3Transform(position, translationToPivot);

			//Pivot Point �߽����� Object Rotation.
			XMMATRIX rotationAroundPivot = rotationMatrix;
			position = XMVector3Transform(position, rotationAroundPivot);

			//������Ʈ�� Pivot Point�� ���������, ����ġ�� ���� Translate.
			XMMATRIX translationFromPivot = XMMatrixTranslationFromVector(pivotPoint);
			position = XMVector3Transform(position, translationFromPivot);
		}

		//if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		//{
		//	_lookDownAngle += 0.5f;
		//}

		//TargetRotation���� �� ����, ���߿� ������ ��.
		Pg::Math::PGFLOAT3 tSameYPT = { _playerTransform->_position.x, _object->_transform._position.y, _playerTransform->_position.z };
		Pg::Math::PGFLOAT3 tLookVector = -PGFloat3Normalize(tSameYPT - _object->_transform._position);
		_targetCamRotation = PGLookRotation(tLookVector, Pg::Math::PGFLOAT3::GlobalUp());
		//Y�� ���� Flip:  
		_targetCamRotation.w *= -1.0f;
		DirectX::XMVECTOR tCurrentCamRot = PG2XM_QUATERNION_VECTOR(_targetCamRotation);

		DirectX::XMVECTOR tTemp = DirectX::XMQuaternionRotationRollPitchYaw(-PGConvertToRadians(_lookDownAngle), 0, 0);
		_targetCamRotation = XM2PG_QUATERNION(DirectX::XMQuaternionMultiply(tCurrentCamRot, tTemp));

		//TargetPosition���� �� ����, ���߿� ������ ��.
		_targetCamPosition = XM2PG_FLOAT3_VECTOR(position);
	}

	Pg::Math::PGFLOAT3 InGameCameraBehavior::GetTargetCamPosition()
	{
		return _targetCamPosition;
	}
}