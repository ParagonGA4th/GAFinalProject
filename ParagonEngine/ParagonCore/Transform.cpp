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

}