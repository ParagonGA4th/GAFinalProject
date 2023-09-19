#include "Camera.h"

namespace Pg::Engine
{
	Camera::Camera(Pg::Core::GameObject* obj) :
		Pg::Core::Component(obj)
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
	}

	void Camera::SetNearZ(float nearZ)
	{

	}

	void Camera::SetFarZ(float farZ)
	{

	}

	void Camera::SetAspect(float aspect)
	{

	}

	void Camera::SetFovY(float fovY)
	{

	}

	Pg::Core::CameraData Camera::GetCameraData()
	{
		return this->_cameraData;
	}

}