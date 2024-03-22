#include "Transform.h"
#include "GameObject.h"
#include <cmath>
#include <limits>
#include <algorithm>

namespace Pg::Data
{
	using namespace Pg::Math;

	Transform::Transform(GameObject* obj) :
		Component(obj),
		_position(0.0f, 0.0f, 0.0f),
		_rotation(1.0f, 0.0f, 0.0f, 0.0f),
		_scale(1.0f, 1.0f, 1.0f),
		_forward(0.f, 0.f, 1.f), _right(1.f, 0.f, 0.f), _up(0.f, 1.f, 0.f), _is3D(true)
	{
		FACTORY_INIT;
	}

	void Transform::OnDeserialize(SerializeVector& sv)
	{
		Pg::Data::SerializerHelper::OnDeserializerHelper<Transform, 3>(this, sv);
	}

	void Transform::OnSerialize(SerializeVector& sv)
	{
		//Pg::Data::SerializerHelper::OnSerializerHelper<Transform, 3>(this, sv);
	}

	//PGFLOAT3 Transform::GetPosition() const
	//{
	//	return _position;
	//}

	//PGQuaternion Transform::GetRotation() const
	//{
	//	return _rotation;

	//}

	//PGFLOAT3 Transform::GetScale() const
	//{
	//	return _scale;

	//}

	//void Transform::SetPosition(const PGFLOAT3& pos)
	//{
	//	SetPosition(pos.x, pos.y, pos.z);
	//}

	//void Transform::SetPosition(float x, float y, float z)
	//{
	//	PGFLOAT4 result = { x, y, z, 1.f };

	//	if (HasParent())
	//	{
	//		// 부모가 있으면 부모 기준의 transform을 구하기 위해 역행렬을 곱해준다
	//		result = PGFloat4MultiplyMatrix(result, _parent->GetWorldTM().Inverse());
	//	}
	//	
	//	_position.x = result.x;
	//	_position.y = result.y;
	//	_position.z = result.z;
	//}

	//void Transform::SetRotation(const PGQuaternion& rot)
	//{
	//	SetRotation(rot.w, rot.x, rot.y, rot.z);
	//}

	//void Transform::SetRotation(float w, float x, float y, float z)
	//{
	//	PGQuaternion result = { w, x, y, z };

	//	if (HasParent())
	//	{
	//		result = PGQuaternionMultiply(result, _parent->GetRotation().Conjugate());
	//	}

	//	_rotation.w = result.w;
	//	_rotation.x = result.x;
	//	_rotation.y = result.y;
	//	_rotation.z = result.z;
	//}

	//void Transform::SetScale(const PGFLOAT3& scale)
	//{
	//	SetScale(scale.x, scale.y, scale.z);
	//}

	//void Transform::SetScale(float x, float y, float z)
	//{
	//	PGFLOAT4 result = { x, y, z, 1.f };

	//	if (HasParent())
	//	{
	//		PGFLOAT4X4 scaleInverseMatrix = _parent->GetWorldScaleMatrix();
	//		scaleInverseMatrix.m[0][0] = 1 / scaleInverseMatrix.m[0][0];
	//		scaleInverseMatrix.m[1][1] = 1 / scaleInverseMatrix.m[1][1];
	//		scaleInverseMatrix.m[2][2] = 1 / scaleInverseMatrix.m[2][2];
	//		scaleInverseMatrix.m[3][3] = 1 / scaleInverseMatrix.m[3][3];
	//		result = PGFloat4MultiplyMatrix(result, scaleInverseMatrix);
	//	}

	//	_scale.x = result.x;
	//	_scale.y = result.y;
	//	_scale.z = result.z;
	//}

	//Pg::Math::PGFLOAT3 Transform::GetLocalScale()
	//{
	//	return _scale;
	//}

	//Pg::Math::PGFLOAT3 Transform::GetLocalPosition()
	//{
	//	return _position;
	//}

//	PGQuaternion Transform::GetLocalRotation()
//	{
//		return _rotation;
//	}

	//void Transform::SetLocalPosition(float x, float y, float z)
	//{
	//	PGFLOAT3 pos = { x, y, z };
	//	SetLocalPosition(pos);
	//}

	//void Transform::SetLocalPosition(PGFLOAT3& pos)
	//{
	//	_position = pos;
	//}

	//void Transform::SetLocalRotation(float w, float x, float y, float z)
	//{
	//	PGQuaternion rot = { w, x, y, z };
	//	SetLocalRotation(rot);
	//}

	//void Transform::SetLocalRotation(PGQuaternion& rot)
	//{
	//	_rotation = rot;
	//}

	//void Transform::SetLocalScale(float x, float y, float z)
	//{
	//	PGFLOAT3 scale = { x, y, z };
	//	SetLocalScale(scale);
	//}

	//void Transform::SetLocalScale(PGFLOAT3& sca)
	//{
	//	_scale = sca;
	//}

	//void Transform::SetLocalRotationEuler(float x, float y, float z)
	//{
	//	PGQuaternion result = EulerToQuaternion(x, y, z);
	//	_rotation = result;
	//}

	//void Transform::SetLocalRotationEuler(PGFLOAT3& euler)
	//{
	//	PGQuaternion result = EulerToQuaternion(euler);
	//	_rotation = result;
	//}

	//이 함수에 명시되어 있는 Euler는 degree 기준이다!!!!!
	// 인줄 알았지만.. 라디안이었다..
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

	Transform* Transform::GetParent()
	{
		if (HasParent())
		{
			return _parent;
		}
		// 부모가 없으면 그냥 리턴
		return nullptr;
	}

	Pg::Math::PGFLOAT4X4 Transform::GetWorldTM()
	{
		PGFLOAT4X4 result = GetLocalTM();

		if (_parent)
		{
			PGFLOAT4X4 tParentWorldTM = _parent->GetWorldTM();
			result = result * tParentWorldTM;
		}

		return result;
	}

	Pg::Math::PGFLOAT4X4 Transform::GetLocalTM()
	{
		//Scale: 0값 되는 것 막기.
		bool tValidScale = (_scale.x > std::numeric_limits<float>::epsilon()) ||
			(_scale.y > std::numeric_limits<float>::epsilon()) ||
			(_scale.z > std::numeric_limits<float>::epsilon());

		if (!tValidScale)
		{
			_scale = { 0.1f,0.1f, 0.1f };
		}

		PGFLOAT4X4 result = Pg::Math::PGScaleMatrix(_scale) * Pg::Math::PGRotationMatrix(_rotation) * Pg::Math::PGTranslateMatrix(_position);
		return result;
	}

	Transform* Transform::GetChild(int index)
	{
		if (!_children.empty())
		{
			return _children[index];
		}
		// 자식이 없으면 nullptr을 리턴
		return nullptr;
	}

	//void Transform::SetParent(Transform* parent)
	//{
	//	//_parent = std::make_shared<Transform>(parent->_object);
	//}
	//
	//void Transform::SetParent(GameObject* obj)
	//{
	//	_parent = &(obj->_transform);
	//}

	const std::vector<Transform*>& Transform::GetChildren()
	{
		return _children;
	}

	void Transform::AddChild(Transform* child)
	{
		child->_parent = this;
		_children.emplace_back(child);
	}

	void Transform::AddChild(GameObject* child)
	{
		this->AddChild(&child->_transform);
	}

	bool Transform::Is3D()
	{
		return _is3D;
	}

	void Transform::SetIs3D(bool is3D)
	{
		_is3D = is3D;
	}

	Pg::Math::PGFLOAT4X4 Transform::GetLocalTranslateMatrix()
	{
		PGFLOAT4X4 result =
		{
			1,	0,	0,	0,
			0,	1,	0,	0,
			0,	0,	1,	0,
			_position.x, _position.y, _position.z, 1
		};

		return result;
	}

	Pg::Math::PGFLOAT4X4 Transform::GetLocalScaleMatrix()
	{
		PGFLOAT4X4 result =
		{
			_scale.x,	0,	0,	0,
			0,	_scale.y,	0,	0,
			0,	0,	_scale.z,	0,
			0,	0,			0,	1
		};

		return result;
	}

	Pg::Math::PGFLOAT4X4 Transform::GetLocalRotationMatrix()
	{
		// 1. 쿼터니언 좌표값을 정규화한다
		PGQuaternion q = NormalizeQuaternion(_rotation);

		// 2. 정규화된 쿼터니언 값을 회전행렬에 적용한다
		PGFLOAT4X4 result =
		{
			1.f - 2.f * (q.y * q.y) - 2.f * (q.z * q.z),
			2.f * q.x * q.y - 2.f * q.w * q.z,
			2.f * q.x * q.z + 2.f * q.w * q.y,
			0.f,

			2.f * q.x * q.y + 2.f * q.w * q.z,
			1.f - 2.f * (q.x * q.x) - 2.f * (q.z * q.z),
			2.f * q.y * q.z - 2.f * q.w * q.x,
			0.f,

			2.f * q.x * q.z - 2.f * q.w * q.y,
			2.f * q.y * q.z + 2.f * q.w * q.x,
			1.f - 2.f * (q.x * q.x) - 2.f * (q.y * q.y),
			0.f,

			0.f,	0.f,	0.f,	1.f
		};

		// 3. 값을 반환한다
		return result;
	}

	Pg::Math::PGFLOAT4X4 Transform::GetWorldTranslateMatrix()
	{
		PGFLOAT4X4 result = GetLocalTranslateMatrix();

		if (_parent)
		{
			result *= _parent->GetLocalTranslateMatrix();
		}

		return result;
	}

	Pg::Math::PGFLOAT4X4 Transform::GetWorldScaleMatrix()
	{
		PGFLOAT4X4 result = GetLocalScaleMatrix();

		if (_parent)
		{
			result *= _parent->GetLocalScaleMatrix();
		}

		return result;
	}

	Pg::Math::PGFLOAT4X4 Transform::GetWorldRotationMatrix()
	{
		PGFLOAT4X4 result = GetLocalRotationMatrix();

		if (_parent)
		{
			result *= _parent->GetWorldRotationMatrix();
		}

		return result;
	}

	Pg::Math::PGFLOAT3 Transform::GetForward()
	{
		PGFLOAT4 result = PGFloat4MultiplyMatrix(PGFLOAT4(0.f, 0.f, 1.f, 0.f), GetWorldRotationMatrix());
		_forward = PGFLOAT3(result.x, result.y, result.z);
		return _forward;
	}

	Pg::Math::PGFLOAT3 Transform::GetUp()
	{
		PGFLOAT4 result = PGFloat4MultiplyMatrix(PGFLOAT4(0.f, 1.f, 0.f, 0.f), GetWorldRotationMatrix());
		_up = PGFLOAT3(result.x, result.y, result.z);
		return _up;
	}

	Pg::Math::PGFLOAT3 Transform::GetRight()
	{
		PGFLOAT4 result = PGFloat4MultiplyMatrix(PGFLOAT4(1.f, 0.f, 0.f, 0.f), GetWorldRotationMatrix());
		_right = PGFLOAT3(result.x, result.y, result.z);
		return _right;
	}

	Pg::Math::PGQuaternion Transform::NormalizeQuaternion(PGQuaternion q)
	{
		double length = std::sqrt(q.w * q.w
			+ q.x * q.x
			+ q.y * q.y
			+ q.z * q.z);

		if (length != 0.0)
		{
			q.w /= length;
			q.x /= length;
			q.y /= length;
			q.z /= length;
		}

		return q;
	}

	void Transform::RemoveChild(GameObject* child)
	{
		RemoveChild(child->GetName());
	}

	void Transform::RemoveChild(std::string child)
	{
		auto res = std::find_if(_children.begin(), _children.end(),
			[&](Transform* trans)
			{
				return trans->_object->GetName() == child;
			});

		if (res != _children.end())
		{
			_children.erase(res);
		}
	}
}