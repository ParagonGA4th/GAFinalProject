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
		return {};
	}

	PGQuaternion Transform::GetRotation() const
	{
		return {};

	}

	PGFLOAT3 Transform::GetScale() const
	{
		return {};

	}

	void Transform::SetPosition(const PGFLOAT3& pos)
	{
		SetPosition(pos.x, pos.y, pos.z);
	}

	void Transform::SetPosition(float x, float y, float z)
	{
		PGFLOAT4 result = { x, y, z, 1.f };

		if (HasParent())
		{
			// 부모가 있으면 부모 기준의 transform을 구하기 위해 역행렬을 곱해준다
			result = PGFloat4MultiplyMatrix(result, _parent->GetWorldTM().Inverse());
		}
		
		_position.x = result.x;
		_position.y = result.y;
		_position.z = result.z;
	}

	void Transform::SetRotation(const PGQuaternion& rot)
	{
		SetRotation(rot.w, rot.x, rot.y, rot.z);
	}

	void Transform::SetRotation(float w, float x, float y, float z)
	{
		PGQuaternion result = { w, x, y, z };

		if (HasParent())
		{
			result = PGQuaternionMultiply(result, _parent->GetRotation().Conjugate());
		}

		_rotation.w = result.w;
		_rotation.x = result.x;
		_rotation.y = result.y;
		_rotation.z = result.z;
	}

	void Transform::SetScale(const PGFLOAT3& scale)
	{
		SetScale(scale.x, scale.y, scale.z);
	}

	void Transform::SetScale(float x, float y, float z)
	{
		PGFLOAT4 result = { x, y, z, 1.f };

		//if (HasParent())
		//{
		//	result = PGFloat4MultiplyMatrix(result, _parent->)
		//}

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
		_position.x = x;
		_position.y = y;
		_position.z = z;
	}

	void Transform::SetLocalPosition(PGFLOAT3& pos)
	{
		_position = pos;
	}

	void Transform::SetLocalRotation(float w, float x, float y, float z)
	{
		PGQuaternion rot = { w, x, y, z };
		SetLocalRotation(rot);
	}

	void Transform::SetLocalRotation(PGQuaternion& rot)
	{
		_rotation = rot;
	}

	void Transform::SetLocalScale(float x, float y, float z)
	{
		PGFLOAT3 scale = { x, y, z };
		SetLocalScale(scale);
	}

	void Transform::SetLocalScale(PGFLOAT3& sca)
	{
		_scale = sca;
	}

	void Transform::SetLocalRotationEuler(float x, float y, float z)
	{
		PGQuaternion result = EulerToQuaternion(x, y, z);
		_rotation = result;
	}

	void Transform::SetLocalRotationEuler(PGFLOAT3& euler)
	{
		PGQuaternion result = EulerToQuaternion(euler);
		_rotation = result;
	}

	Pg::Math::PGQuaternion Transform::EulerToQuaternion(float x, float y, float z)
	{
		PGQuaternion quaternion;

		// 각 축의 값을 반으로 나눈 뒤... 계산
		double cy = cos(x * 0.5);
		double sy = sin(x * 0.5);
		double cp = cos(y * 0.5);
		double sp = sin(y * 0.5);
		double cr = cos(z * 0.5);
		double sr = sin(z * 0.5);

		quaternion.w = cy * cp * cr + sy * sp * sr;
		quaternion.x = cy * cp * sr - sy * sp * cr;
		quaternion.y = sy * cp * sr + cy * sp * cr;
		quaternion.z = sy * cp * cr - cy * sp * sr;

		return quaternion;
	}

	Pg::Math::PGQuaternion Transform::EulerToQuaternion(const PGFLOAT3& euler)
	{
		return EulerToQuaternion(euler.x, euler.y, euler.z);
	}

	Pg::Math::PGFLOAT3 Transform::QuaternionToEuler(const PGQuaternion& quaternion)
	{
		return QuaternionToEuler(quaternion.w, quaternion.x, quaternion.y, quaternion.z);
	}

	Pg::Math::PGFLOAT3 Transform::QuaternionToEuler(float w, float x, float y, float z)
	{
		PGFLOAT3 euler;

		// Roll (X 축 회전)
		double sinr_cosp = 2.0 * (w * x + y * z);
		double cosr_cosp = 1.0 - 2.0 * (x * x + y * y);
		euler.x = std::atan2(sinr_cosp, cosr_cosp);

		// Pitch (Y 축 회전)
		double sinp = 2.0 * (w * y - z * x);
		if (std::abs(sinp) >= 1)
			euler.y = std::copysign(PG_PI / 2.0, sinp); // use 90 degrees if out of range
		else
			euler.y = std::asin(sinp);

		// Yaw (Z 축 회전)
		double siny_cosp = 2.0 * (w * z + x * y);
		double cosy_cosp = 1.0 - 2.0 * (y * y + z * z);
		euler.z = std::atan2(siny_cosp, cosy_cosp);

		return euler;
	}

	bool Transform::HasParent()
	{
		return _parent != nullptr;
	}

	std::shared_ptr<Transform> Transform::GetParent()
	{
		if (HasParent())
		{
			return _parent;
		}
		// 부모가 없으면 그냥 리턴
		return nullptr;
	}

	/*void Transform::UpdateTransform()
	{
		if (HasParent())
		{
			GetWorldTM();
		}
		else
		{

		}
	}*/

	Pg::Math::PGFLOAT4X4 Transform::GetWorldTM()
	{
		PGFLOAT4X4 result = Pg::Math::PGScaleMatrix(_scale) * Pg::Math::PGRotationMatrix(_rotation) * Pg::Math::PGTranslateMatrix(_position);
		
		if (_parent)
		{
			_parent->GetWorldTM();
		}

		return result;
	}

	std::shared_ptr<Transform> Transform::GetChild(int index)
	{
		if (!_children.empty())
		{
			return _children.at(index);
		}
		// 자식이 없으면 그냥 리턴
		return nullptr;
	}

	void Transform::SetParent(Transform* parent)
	{
		_parent = std::make_shared<Transform>(parent->_object);
	}

	void Transform::SetParent(GameObject* obj)
	{
		_parent = std::make_shared<Transform>(obj);
	}

	const std::vector<std::shared_ptr<Transform>>& Transform::GetChildren()
	{
		return _children;
	}

	void Transform::AddChild(std::shared_ptr<Transform> child)
	{
		_children.emplace_back(child);
	}

}