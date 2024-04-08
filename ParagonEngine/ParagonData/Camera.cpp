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

		//ProjectionMatrix를 마련.
		//SetProjectionLens(0.4f * std::numbers::pi, static_cast<float>(screenWidth) / screenHeight, 0.0001f, 1000.0f);
	}

	void Camera::Update()
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

	void Camera::ScreenPointToRayInfo(Pg::Math::PGFLOAT2 screenPointNormalized, Pg::Math::PGFLOAT3& outRayOrigin, Pg::Math::PGFLOAT3& outRayDir)
	{
		using namespace Pg::Math;
		using namespace DirectX;

		//역투영 : 정규화된 ScreenPoint를 뷰 공간으로!
		Pg::Data::CameraData* tCamData = GetCameraData();
		XMMATRIX viewMat = PG2XM_MATRIX4X4(tCamData->_viewMatrix);
		XMMATRIX projMat = PG2XM_MATRIX4X4(tCamData->_projMatrix);

		//Pixel 기준으로 받기. GameConstantData 기준.
		XMFLOAT2 tSP = PG2XM_FLOAT2(screenPointNormalized);
		tSP.x *= GameConstantData::WIDTH;
		tSP.y *= GameConstantData::HEIGHT;
		XMVECTOR screenPointInPixels = XMLoadFloat2(&tSP);

		//RayNear / Ray Far 구하기 (Direction을 구하기 위해.
		XMVECTOR rayNear = XMVector3Unproject(screenPointInPixels, 0, 0, GameConstantData::WIDTH, GameConstantData::HEIGHT,
			tCamData->_nearZ, tCamData->_nearZ, projMat, viewMat, PG2XM_MATRIX4X4(_object->_transform.GetWorldTM()));

		XMVECTOR rayFar = XMVector3Unproject(screenPointInPixels, 0, 0, GameConstantData::WIDTH, GameConstantData::HEIGHT,
			tCamData->_nearZ, tCamData->_farZ, projMat, viewMat, PG2XM_MATRIX4X4(_object->_transform.GetWorldTM()));

		// 레이 방향과 오리진 -> 참조자로 리턴.
		outRayDir = XM2PG_FLOAT3_VECTOR(XMVector3Normalize(XMVectorSubtract(rayFar, rayNear)));
		outRayOrigin = XM2PG_FLOAT3_VECTOR(rayNear);
	}

}