#include "Transform.h"

namespace Pg::Core
{
	using namespace Pg::Math;

	Transform::Transform(GameObject* obj) :
		Component(obj),
		_position(0.0f,0.0f,0.0f),
		_rotation(0.0f,0.0f,0.0f,0.0f),
		_scale(1.0f,1.0f,1.0f)
	{

	}

	PGFLOAT3 Transform::GetPosition() const
	{
		return _position;
	}

	PGQuaternion Transform::GetRotation() const
	{
		return _rotation;
	}

	PGFLOAT3 Transform::GetScale() const
	{
		return _scale;
	}

	void Transform::SetPosition(const PGFLOAT3& pos)
	{
		_position.x = pos.x;
		_position.y = pos.y;
		_position.z = pos.z;
	}

	void Transform::SetRotation(const PGQuaternion& rot)
	{
		_rotation.x = rot.x;
		_rotation.y = rot.y;
		_rotation.z = rot.z;
		_rotation.w = rot.w;
	}

	void Transform::SetScale(const PGFLOAT3& scale)
	{
		_scale.x = scale.x;
		_scale.y = scale.y;
		_scale.z = scale.z;
	}

	Pg::Math::PGFLOAT3 Transform::GetLocalScale()
	{
		return _scale;
	}

	Pg::Math::PGFLOAT3 Transform::GetLocalPosition()
	{
		return _position;
	}

	PGQuaternion Transform::GetLocalRotation()
{
		return _rotation;
	}

	void Transform::SetLocalPosition(float x, float y, float z)
	{

	}

	void Transform::SetLocalPosition(PGFLOAT3& pos)
	{

	}

	void Transform::SetLocalRotation(float w, float x, float y, float z)
	{

	}

	void Transform::SetLocalRotation(PGQuaternion& rot)
	{

	}

	void Transform::SetLocalScale(float x, float y, float z)
	{

	}

	void Transform::SetLocalScale(PGFLOAT3& sca)
	{

	}

	void Transform::SetLocalRotationEuler(float x, float y, float z)
	{

	}

	void Transform::SetLocalRotationEuler(PGFLOAT3& euler)
	{

	}

	Pg::Math::PGQuaternion Transform::EulerToQuaternion(float x, float y, float z)
	{
		return _rotation;
	}

	Pg::Math::PGFLOAT3 Transform::QuaternionToEuler(const PGQuaternion& quaternion)
	{
		return {};
	}

	bool Transform::HasParent()
	{
		return {};
	}

	std::shared_ptr<Transform> Transform::GetParent()
	{
		return {};
	}

}