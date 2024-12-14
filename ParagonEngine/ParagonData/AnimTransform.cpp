#include "AnimTransform.h"

namespace Pg::Data
{
	using namespace DirectX::SimpleMath;

	AnimTransform::AnimTransform()
		: _position(Vector3::Zero)
		, _rotation(Quaternion::Identity)
		, _scale(Vector3::One)
		, _parent(nullptr)
		, _children()
		, _isDirty(false)
		, _worldTM(Matrix::Identity)
	{

	}

	void AnimTransform::SetPosition(const Vector3& position)
	{
		Vector3 result = position;

		if (_parent)
		{
			result = Vector3::Transform(result, _parent->GetWorldTM().Invert());
		}

		_position = result;
		SetDirtyRecur(this);
	}

	void AnimTransform::SetRotation(const Quaternion& rotation)
	{
		Quaternion result = rotation;

		if (_parent)
		{
			Quaternion parentRot = _parent->GetRotation();
			parentRot.Conjugate();

			result = Quaternion::Concatenate(result, parentRot);
		}

		_rotation = result;
		SetDirtyRecur(this);
	}

	void AnimTransform::SetRotationEuler(const Vector3& euler)
	{
		SetRotation(Quaternion::CreateFromYawPitchRoll(euler));
	}

	void AnimTransform::SetScale(const Vector3& scale)
	{
		Vector3 result = scale;

		if (_parent)
		{
			Vector3 parentScale = _parent->GetScale();
			result.x /= parentScale.x;
			result.y /= parentScale.y;
			result.z /= parentScale.z;
		}

		_scale = result;
		SetDirtyRecur(this);
	}

	Vector3 AnimTransform::GetPosition() const
	{
		Vector3 result = { _position.x, _position.y, _position.z };

		if (_parent)
		{
			result = Vector3::Transform(result, _parent->GetWorldTM());
		}

		return result;
	}

	Quaternion AnimTransform::GetRotation() const
	{
		Quaternion result = _rotation;

		if (_parent)
		{
			result = Quaternion::Concatenate(result, _parent->GetRotation());
		}

		return result;
	}

	Vector3 AnimTransform::GetEuler() const
	{
		return GetRotation().ToEuler();
	}

	Vector3 AnimTransform::GetScale() const
	{
		Vector3 result = _scale;

		if (_parent)
		{
			Vector3 parentScale = _parent->GetScale();
			result.x *= parentScale.x;
			result.y *= parentScale.y;
			result.z *= parentScale.z;
		}

		return result;
	}

	void AnimTransform::SetLocalPosition(const Vector3& position)
	{
		_position = position;
		SetDirtyRecur(this);
	}

	void AnimTransform::SetLocalRotation(const Quaternion& quaternion)
	{
		_rotation = quaternion;
		_rotation.Normalize();
		SetDirtyRecur(this);
	}

	void AnimTransform::SetLocalRotationEuler(const Vector3& euler)
	{
		_rotation = Quaternion::CreateFromYawPitchRoll({ euler.x, euler.y , euler.z });
		SetDirtyRecur(this);
	}

	void AnimTransform::SetLocalScale(const Vector3& scale)
	{
		_scale = scale;
		SetDirtyRecur(this);
	}

	Vector3 AnimTransform::GetLocalPosition() const
	{
		return _position;
	}

	Quaternion AnimTransform::GetLocalRotation() const
	{
		return _rotation;
	}

	Vector3 AnimTransform::GetLocalEuler() const
	{

		Vector3 radian = _rotation.ToEuler();

		float degreeX = DirectX::XMConvertToDegrees(radian.x);
		float degreeY = DirectX::XMConvertToDegrees(radian.y);
		float degreeZ = DirectX::XMConvertToDegrees(radian.z);

		return { degreeX,degreeY,degreeZ };
	}

	Vector3 AnimTransform::GetLocalScale() const
	{
		return _scale;
	}

	Vector3 AnimTransform::GetForward() const
	{
		return Vector3::Transform(Vector3::Backward, GetRotation());
	}

	Vector3 AnimTransform::GetUp() const
	{
		return Vector3::Transform(Vector3::Up, GetRotation());
	}

	Vector3 AnimTransform::GetRight() const
	{
		return Vector3::Transform(Vector3::Right, GetRotation());
	}

	Matrix AnimTransform::GetScaleMatrix() const
	{
		Matrix result = GetLocalScaleMatrix();

		if (_parent)
		{
			result *= _parent->GetScaleMatrix();
		}

		return result;
	}

	Matrix AnimTransform::GetRotationMatrix() const
	{
		Matrix result = GetLocalRotationMatrix();

		if (_parent)
		{
			result *= _parent->GetRotationMatrix();
		}

		return result;
	}

	Matrix AnimTransform::GetTranslationMatrix() const
	{
		Matrix result = GetLocalTranslationMatrix();

		if (_parent)
		{
			result *= _parent->GetTranslationMatrix();
		}

		return result;
	}

	Matrix AnimTransform::GetWorldTM()
	{
		// 이거 한번 써봐야 할듯?
		// return Matrix::CreateWorld(_position, GetForward(), GetUp());

		if (!_isDirty)
		{
			return _worldTM;
		}

		Matrix result = GetLocalTM();

		if (_parent)
		{
			result *= _parent->GetWorldTM();
		}

		_worldTM = result;
		_isDirty = false;

		return result;
	}

	Matrix AnimTransform::GetLocalScaleMatrix() const
	{
		return Matrix::CreateScale(_scale);
	}

	Matrix AnimTransform::GetLocalRotationMatrix() const
	{
		return Matrix::CreateFromQuaternion(_rotation);
	}

	Matrix AnimTransform::GetLocalTranslationMatrix() const
	{
		return Matrix::CreateTranslation(_position);
	}

	Matrix AnimTransform::GetLocalTM() const
	{
		Matrix result;
		result *= GetLocalScaleMatrix();
		result *= GetLocalRotationMatrix();
		result *= GetLocalTranslationMatrix();

		return result;
	}

	void AnimTransform::Translate(const Vector3& translation)
	{
		_position += translation;
		SetDirtyRecur(this);
	}

	void AnimTransform::Rotate(const Quaternion& quaternion)
	{
		_rotation = Quaternion::Concatenate(_rotation, quaternion);
		SetDirtyRecur(this);
	}

	void AnimTransform::LookAt(const Vector3& target, const Vector3& up /*= Vector3::Up*/)
	{
		Vector3 forward = target - GetPosition();
		forward.Normalize();
		Vector3 right = up.Cross(forward);
		right.Normalize();
		Vector3 upVec = forward.Cross(right);
		upVec.Normalize();

		_rotation = Quaternion::CreateFromRotationMatrix(Matrix::CreateWorld(GetPosition(), forward, upVec));
		SetDirtyRecur(this);
	}

	void AnimTransform::LookAt(const AnimTransform* target, const Vector3& up /*= Vector3::Up*/)
	{
		Vector3 targetPosition = target->GetPosition();

		Vector3 forward = targetPosition - GetPosition();
		forward.Normalize();
		Vector3 right = up.Cross(forward);
		right.Normalize();
		Vector3 upVec = forward.Cross(right);
		upVec.Normalize();

		_rotation = Quaternion::CreateFromRotationMatrix(Matrix::CreateWorld(GetPosition(), forward, upVec));
		SetDirtyRecur(this);
	}

	void AnimTransform::RotateAround(const Vector3& point, const Vector3& axis, float angle)
	{
		Vector3 position = GetPosition();
		Vector3 direction = position - point;
		Matrix rotation = Matrix::CreateFromAxisAngle(axis, angle);
		direction = Vector3::Transform(direction, rotation);
		SetPosition(point + direction);
		SetDirtyRecur(this);
	}

	void AnimTransform::Scale(const Vector3& scale)
	{
		_scale *= scale;
		SetDirtyRecur(this);
	}

	void AnimTransform::SetParent(AnimTransform* parent, bool keepWorldPosition)
	{
		if (parent == nullptr)
		{
			ReleaseParent();
			return;
		}

		if (keepWorldPosition)
		{
			_position = Vector3::Transform(_position, parent->GetWorldTM().Invert());
			Quaternion parentRot = parent->GetRotation();
			parentRot.Conjugate();
			_rotation = Quaternion::Concatenate(_rotation, parentRot);
			_scale = Vector3::Transform(_scale, parent->GetScaleMatrix().Invert());
		}

		_parent = parent;
		parent->AddChild(this);
		SetDirtyRecur(this);
	}

	AnimTransform* AnimTransform::GetParent() const
	{
		return _parent;
	}

	AnimTransform* AnimTransform::GetChild(int index) const
	{
		if (_children.size() <= index)
		{
			return nullptr;
		}

		return _children[index];
	}

	int AnimTransform::GetChildCount() const
	{
		return (int)_children.size();
	}

	void AnimTransform::AddChild(AnimTransform* child)
	{
		_children.emplace_back(child);
	}

	void AnimTransform::ReleaseChild(AnimTransform* child)
	{
		auto iter = std::find(_children.begin(), _children.end(), child);
		if (iter != _children.end())
		{
			_children.erase(iter);
		}
		SetDirtyRecur(this);
	}

	void AnimTransform::ReleaseParent()
	{
		if (!_parent)
		{
			return;
		}

		SetLocalPosition(Vector3::Transform(_position, _parent->GetWorldTM()));
		SetLocalRotation(Quaternion::Concatenate(_rotation, _parent->GetRotation()));
		SetLocalScale(Vector3::Transform(_scale, _parent->GetScaleMatrix()));

		_parent->ReleaseChild(this);
		_parent = nullptr;
		SetDirtyRecur(this);
	}

	std::vector<AnimTransform*>& AnimTransform::GetChildren()
	{
		return _children;
	}

	Vector3* AnimTransform::GetLocalPositionPtr()
	{
		return &_position;
	}

	Quaternion* AnimTransform::GetLocalRotationPtr()
	{
		return &_rotation;
	}

	Vector3* AnimTransform::GetLocalScalePtr()
	{
		return &_scale;
	}

	void AnimTransform::SetDirtyRecur(AnimTransform* transform)
	{
		_isDirty = true;

		for (auto& child : _children)
		{
			child->SetDirtyRecur(child);
		}
	}
}
