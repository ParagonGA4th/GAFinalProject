#pragma once
#include <vector>
#include <dxtk/SimpleMath.h>

/// <summary>
/// RocketEngine ���, Animation �� Ȱ���� ���� ������ Transform.
/// </summary>
	
namespace Pg::Data
{
	using namespace DirectX::SimpleMath;

	class AnimTransform
	{
	public:
		AnimTransform();
	
		/// World ���� Getter Setter
		void SetPosition(const Vector3& position);
		void SetRotation(const Quaternion& rotation);
		void SetRotationEuler(const Vector3& euler);		// x,y,z ���� ȸ�� �� (radian)
		void SetScale(const Vector3& scale);

		Vector3 GetPosition() const;
		Quaternion GetRotation() const;
		Vector3 GetEuler() const;
		Vector3 GetScale() const;

		/// Local ���� Getter Setter
		void SetLocalPosition(const Vector3& position);
		void SetLocalRotation(const Quaternion& quaternion);
		void SetLocalRotationEuler(const Vector3& euler);
		void SetLocalScale(const Vector3& scale);

		Vector3 GetLocalPosition() const;
		Quaternion GetLocalRotation() const;
		Vector3 GetLocalEuler() const;
		Vector3 GetLocalScale() const;

		/// Local ���� ������ ��ȯ (Tween Animation ���)
		Vector3* GetLocalPositionPtr();
		Quaternion* GetLocalRotationPtr();
		Vector3* GetLocalScalePtr();

		/// World ���� ���� ����
		Vector3 GetForward() const;
		Vector3 GetUp() const;
		Vector3 GetRight() const;

		/// World ���� ���
		Matrix GetScaleMatrix() const;
		Matrix GetRotationMatrix() const;
		Matrix GetTranslationMatrix() const;
		Matrix GetWorldTM();

		/// Local ���� ���
		Matrix GetLocalScaleMatrix() const;
		Matrix GetLocalRotationMatrix() const;
		Matrix GetLocalTranslationMatrix() const;
		Matrix GetLocalTM() const;

		/// Transform ��ȯ �Լ�
		void Translate(const Vector3& translation);
		void Rotate(const Quaternion& quaternion);
		void LookAt(const Vector3& target, const Vector3& up = Vector3::Up);
		void LookAt(const AnimTransform* target, const Vector3& up = Vector3::Up);
		void RotateAround(const Vector3& point, const Vector3& axis, float angle);
		void Scale(const Vector3& scale);

		/// �������� �Լ�
		void SetParent(AnimTransform* parent, bool keepWorldPosition);
		AnimTransform* GetParent() const;
		AnimTransform* GetChild(int index) const;
		int GetChildCount() const;
		void ReleaseParent();
		std::vector<AnimTransform*>& GetChildren();

	protected:
		void AddChild(AnimTransform* child);
		void ReleaseChild(AnimTransform* child);
		void SetDirtyRecur(AnimTransform* transform);

	private:
		Vector3 _position;
		Quaternion _rotation;		// ���ʹϾ�
		Vector3 _scale;

		AnimTransform* _parent;
		std::vector<AnimTransform*> _children;
		bool _isDirty;
		Matrix _worldTM;
	};
}
