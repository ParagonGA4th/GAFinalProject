#include "Camera.h"
#include "GameObject.h"
#include "GameConstantData.h"
#include "CameraData.h"
#include <cmath>
#include <numbers>

namespace Pg::Data
{
	Camera::Camera(Pg::Data::GameObject* obj) :
		Pg::Data::Component(obj),
		_nearZ(0.1f), _farZ(300.0f), _fovY(PG_PI * 0.4f)
	{
		FACTORY_INIT;

		_cameraData = std::make_unique<Pg::Data::CameraData>();

		//기본 정보 설정.
		this->_object->_transform._position = { 0.f, 3.0f, -10.f };
		this->_object->_transform._rotation = { 0.0f, 0.0f, 0.0f, 0.0f };
		this->SetScreenSize((float)Pg::Data::GameConstantData::WIDTH, (float)Pg::Data::GameConstantData::HEIGHT);
		//ProjectionMatrix를 마련.
		//SetProjectionLens(0.4f * std::numbers::pi, static_cast<float>(screenWidth) / screenHeight, 0.0001f, 1000.0f);
	}

	void Camera::Internal_EngineUpdate()
	{
		//Projection Matrix는 종횡비를 Data딴에서 알 수 없기 때문에 이를 알고 있는 곳에서 만들어진다.
		//반면, View Matrix는 Camera가 전적으로 담당해야 한다.
		UpdateViewMatrix();
	}

	void Camera::OnDeserialize(SerializeVector& sv)
	{
		Pg::Data::SerializerHelper::OnDeserializerHelper(this, sv);
	}

	void Camera::OnSerialize(SerializeVector& sv)
	{
		Pg::Data::SerializerHelper::OnSerializerHelper(this, sv);
	}

	//float Camera::GetNearZ() const
	//{
	//	return _nearZ;
	//}

	//float Camera::GetFarZ() const
	//{
	//	return _farZ;
	//}

	//float Camera::GetFovY() const
	//{
	//	return _fovY;
	//}

	//void Camera::SetNearZ(float nearZ)
	//{
	//	_nearZ = nearZ;
	//}

	//void Camera::SetFarZ(float farZ)
	//{
	//	_farZ = farZ;
	//}

	//void Camera::SetFovY(float fovY)
	//{
	//	_fovY = fovY;
	//}

	void Camera::SetScreenSize(float width, float height)
	{
		_screenWidth = width;
		_screenHeight = height;
	}

	Pg::Math::PGFLOAT4X4 Camera::GetViewMatrix() const
	{
		return _viewMatrix;
	}

	Pg::Math::PGFLOAT4X4 Camera::GetProjMatrix() const
	{
		return _projMatrix;
	}

	Pg::Data::CameraData* Camera::GetCameraData()
	{
		_cameraData->_position = _object->_transform._position;
		_cameraData->_rotation = _object->_transform._rotation;

		_cameraData->_farZ = _farZ;
		_cameraData->_fovY = _fovY;
		_cameraData->_nearZ = _nearZ;

		_cameraData->_viewMatrix = GetViewMatrix();

		return _cameraData.get();
	}

	void Camera::UpdateViewMatrix()
	{
		using namespace Pg::Math;

		PGFLOAT3 R = _object->_transform.GetRight();
		PGFLOAT3 U = _object->_transform.GetUp();
		PGFLOAT3 L = _object->_transform.GetForward();
		PGFLOAT3 P = _object->_transform._position;

		// Keep camera's axes orthogonal to each other and of unit length.
		L = PGFloat3Normalize(L);
		U = PGFloat3Normalize(PGFloat3Cross(L, R));

		// U, L already ortho-normal, so no need to normalize cross product.
		R = PGFloat3Cross(U, L);

		// Fill in the view matrix entries.
		float x = -PGFloat3Dot(P, R);
		float y = -PGFloat3Dot(P, U);
		float z = -PGFloat3Dot(P, L);

		_viewMatrix.m[0][0] = R.x;
		_viewMatrix.m[1][0] = R.y;
		_viewMatrix.m[2][0] = R.z;
		_viewMatrix.m[3][0] = x;

		_viewMatrix.m[0][1] = U.x;
		_viewMatrix.m[1][1] = U.y;
		_viewMatrix.m[2][1] = U.z;
		_viewMatrix.m[3][1] = y;

		_viewMatrix.m[0][2] = L.x;
		_viewMatrix.m[1][2] = L.y;
		_viewMatrix.m[2][2] = L.z;
		_viewMatrix.m[3][2] = z;

		_viewMatrix.m[0][3] = 0.0f;
		_viewMatrix.m[1][3] = 0.0f;
		_viewMatrix.m[2][3] = 0.0f;
		_viewMatrix.m[3][3] = 1.0f;

		_cameraData->_nearWindowHeight = 2.0f * _nearZ * tanf(0.5f * _fovY);
		_cameraData->_farWindowHeight = 2.0f * _farZ * tanf(0.5f * _fovY);

		_cameraData->_aspect = _screenWidth / _screenHeight;
		_projMatrix = Pg::Math::PGMatrixPerspectiveFovLH(_fovY, _cameraData->_aspect, _nearZ, _farZ);
		_cameraData->_projMatrix = _projMatrix;
	}

	Pg::Math::PGFLOAT3 Camera::ScreenPointToWorldPlanePoint(Pg::Math::PGFLOAT2 screenPointNormalized, Pg::Math::PGFLOAT3 normalVec, float distance)
	{
		///Special Thanks To 종화형.
		// screen 좌표계를 ndc로 변환
		float normalizedX = screenPointNormalized.x * 2.0f - 1.0f;
		float normalizedY = 1 - screenPointNormalized.y * 2.0f;

		Pg::Data::CameraData* tCamData = GetCameraData();
		DirectX::XMMATRIX worldMat = PG2XM_MATRIX4X4(_object->_transform.GetWorldTM());
		DirectX::XMMATRIX viewMat = PG2XM_MATRIX4X4(tCamData->_viewMatrix);
		DirectX::XMMATRIX projMat = PG2XM_MATRIX4X4(tCamData->_projMatrix);

		// ndc에 proj 역행렬을 곱해 view 공간으로 변환
		float viewX = normalizedX /	projMat.r[0].m128_f32[0];
		float viewY = normalizedY / projMat.r[1].m128_f32[1];

		DirectX::XMVECTOR rayDir = DirectX::XMVectorSet(viewX, viewY, 1.0f, 1.0f);
		rayDir = DirectX::XMVector3Normalize(rayDir);

		DirectX::XMMATRIX invView = DirectX::XMMatrixInverse(nullptr, viewMat);

		DirectX::XMVECTOR rayOrigin = DirectX::XMVectorSet(tCamData->_position.x, tCamData->_position.y, tCamData->_position.z, 1.0f);

		// view 공간에 view 행렬의 역행렬을 곱해 world 공간으로 변환
		DirectX::XMVECTOR rayDirToWorld = DirectX::XMVector3TransformCoord(rayDir, invView);

		DirectX::XMVECTOR intersectionPoint = DirectX::XMPlaneIntersectLine(
			DirectX::XMVectorSet(normalVec.x, normalVec.y, normalVec.z, distance),        // 평면의 방정식 계수
			rayOrigin,		// 라인의 시작점 (카메라 위치)
			rayDirToWorld   // 라인의 방향
		);

		return Pg::Data::PGFLOAT3(intersectionPoint.m128_f32[0], intersectionPoint.m128_f32[1], intersectionPoint.m128_f32[2]);
	}


}
