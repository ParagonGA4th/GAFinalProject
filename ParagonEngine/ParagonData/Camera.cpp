#include "Camera.h"
#include "GameObject.h"
#include <cmath>
#include <numbers>

namespace Pg::Engine
{
	Camera::Camera(Pg::Data::GameObject* obj) :
		Pg::Data::Component(obj)
	{
		_cameraData = std::make_unique<Pg::Data::CameraData>();

		//ProjectionMatrix¸¦ ¸¶·Ã.
		//SetProjectionLens(0.4f * std::numbers::pi, static_cast<float>(screenWidth) / screenHeight, 0.0001f, 1000.0f);
	}

	void Camera::Update()
	{
		
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
	

}