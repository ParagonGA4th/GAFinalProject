#include "Camera.h"
#include "GameObject.h"
#include <cmath>
#include <numbers>

namespace Pg::Data
{
	Camera::Camera(Pg::Data::GameObject* obj) :
		Pg::Data::Component(obj)
	{
		_cameraData = std::make_unique<Pg::Data::CameraData>();

		//ProjectionMatrix를 마련.
		//SetProjectionLens(0.4f * std::numbers::pi, static_cast<float>(screenWidth) / screenHeight, 0.0001f, 1000.0f);
	}

	void Camera::Update()
	{
		//Projection Matrix는 종횡비를 Data딴에서 알 수 없기 때문에 이를 알고 있는 곳에서 만들어진다.
		//반면, View Matrix는 Camera가 전적으로 담당해야 한다.

		
	}

	float Camera::GetNearZ() const
	{
		return _nearZ;
	}

	float Camera::GetFarZ() const
	{
		return _farZ;
	}

	float Camera::GetFovY() const
	{
		return _fovY;
	}

	float Camera::GetAspect() const
	{
		return _aspect;
	}

	Pg::Math::PGFLOAT4X4 Camera::GetViewMatrix() const
	{
		Pg::Math::PGFLOAT4X4 tReturn =
		{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 3.0f, 1.0f,
		};
		return tReturn;
		//return _viewMatrix;
	}

	Pg::Math::PGFLOAT4X4 Camera::GetProjMatrix() const
	{
		Pg::Math::PGFLOAT4X4 tReturn =
		{
			1.35799503f, 0.0f, 0.0f, 0.0f,
			0.0f, 2.41421342f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.00000012f, 1.0f,
			0.0f, 0.0f, -0.000100000012f, 0.0f,
		};
		return tReturn;
		//return _projMatrix;
	}

	void Camera::SetNearZ(float nearZ)
	{
		_nearZ = nearZ;
	}

	void Camera::SetFarZ(float farZ)
	{
		_farZ = farZ;
	}

	void Camera::SetAspect(float aspect)
	{

	}

	void Camera::SetFovY(float fovY)
	{

	}

	Pg::Data::CameraData* Camera::GetCameraData()
	{
		_cameraData->_position = _object->_transform.GetPosition();
		_cameraData->_rotation = _object->_transform.GetRotation();

		_cameraData->_farZ = GetFarZ();
		_cameraData->_fovY = GetFovY();
		_cameraData->_nearZ = GetNearZ();
		_cameraData->aspect = GetAspect();

		_cameraData->_viewMatrix = GetViewMatrix();
		_cameraData->_projMatrix = GetProjMatrix();

		return _cameraData.get();
	}

	void Camera::SetProjectionLens(float fovY, float aspect, float zn, float zf)
	{
		// cache properties
		this->_fovY = fovY;
		this->_aspect = aspect;
		this->_nearZ = zn;
		this->_farZ = zf;

		this->_nearWindowHeight = 2.0f * _nearZ * tanf(0.5f * _fovY);
		this->_farWindowHeight = 2.0f * _farZ * tanf(0.5f * _fovY);

		this->_projMatrix = PGMatrixPerspectiveFovLH(fovY, aspect, zn, zf);
	}

	void Camera::UpdateViewMatrix()
	{
		//using namespace Pg::Math;
		//
		//PGFLOAT3 R = _object->_transform.GetRight();
		//PGFLOAT3 U = _object->_transform.GetUp();
		//PGFLOAT3 L = _object->_transform.GetForward();
		//PGFLOAT3 P = _object->_transform.GetPosition();
		//
		//// Keep camera's axes orthogonal to each other and of unit length.
		//L = PGFloat3Normalize(L);
		//U = PGFloat3Normalize(PGFloat3Cross(L, R));
		//
		//// U, L already ortho-normal, so no need to normalize cross product.
		//R = PGFloat3Cross(U, L);
		//
		//// Fill in the view matrix entries.
		//float x = -PGFloat3Dot(P, R);
		//float y = -PGFloat3Dot(P, U);
		//float z = -PGFloat3Dot(P, L);

		//XMStoreFloat3(&mRight, R);
		//XMStoreFloat3(&mUp, U);
		//XMStoreFloat3(&mLook, L);
		//
		//_viewMatrix(0, 0) = mRight.x;
		//_viewMatrix(1, 0) = mRight.y;
		//_viewMatrix(2, 0) = mRight.z;
		//_viewMatrix(3, 0) = x;
		//
		//_viewMatrix(0, 1) = mUp.x;
		//_viewMatrix(1, 1) = mUp.y;
		//_viewMatrix(2, 1) = mUp.z;
		//_viewMatrix(3, 1) = y;
		//
		//_viewMatrix(0, 2) = mLook.x;
		//_viewMatrix(1, 2) = mLook.y;
		//_viewMatrix(2, 2) = mLook.z;
		//_viewMatrix(3, 2) = z;
		//
		//_viewMatrix(0, 3) = 0.0f;
		//_viewMatrix(1, 3) = 0.0f;
		//_viewMatrix(2, 3) = 0.0f;
		//_viewMatrix(3, 3) = 1.0f;
	}


}