#pragma once
#include "Component.h"
#include "../ParagonMath/PgMath.h"
#include "data_factory.h" // Auto Register�� ���� �ʼ����.
#include <memory>

/// <summary>
/// 23.10.6 ������ && ������
/// �������� �������� ¥�� �������� ������ Trasnform Ŭ����
/// ������Ʈ�� �ڽİ� �θ�, transform ���Ӽ� ���� ��� ���⼭ �����Ѵ�
/// ���� ������Ʈ�� ���Ϸ�, ���ʹϾ� ��ȯ�� ���� �Լ��� ����.
/// 
/// 24.01.03 �ֹμ�
/// ���� Get, Set ��Ŀ��� DataSerializer�� ���� visit_struct ���� ������� ����.
/// </summary>

namespace Pg::Editor::Window { class Hierarchy; }

namespace Pg::Data
{
	using namespace Pg::Math;

	class GameObject;

	class Transform : public Component, Pg::Factory::Data::RegisteredInFactory<Component, Transform, GameObject*>
	{
		friend class Camera;
		friend class Pg::Editor::Window::Hierarchy;
		friend class StaticMeshRenderer;
		friend class SkinnedMeshRenderer;
	public:
		// �ӽ� �⺻������
		Transform() = default;
		Transform(GameObject* obj);

		//�ڵ�ȭ�� Auto-Registration �۵� ���� �ʼ�.
		static Component* CreateInstance(GameObject* go) { return new Transform(go); }
		static const char* GetFactoryKey() { return "class Pg::Data::Transform"; }

		virtual void OnDeserialize(SerializeVector& sv) override;
		virtual void OnSerialize(SerializeVector& sv) override;

		//// Get ���� �Լ���
		//PGFLOAT3 GetPosition() const;
		//PGQuaternion GetRotation() const;
		//PGFLOAT3 GetScale() const;

		//// Get ���� �Լ���
		//PGFLOAT3 GetLocalScale();
		//PGFLOAT3 GetLocalPosition();
		//PGQuaternion GetLocalRotation();

		//// Set ���� �Լ���
		//void SetPosition(const PGFLOAT3& pos);
		//void SetPosition(float x, float y, float z);
		//void SetRotation(const PGQuaternion& rot);
		//void SetRotation(float w, float x, float y, float z);
		//void SetScale(const PGFLOAT3& scale);
		//void SetScale(float x, float y, float z);

		//// Set ���� �Լ���
		//void SetLocalPosition(float x, float y, float z);
		//void SetLocalPosition(PGFLOAT3& pos);

		//void SetLocalRotation(float w, float x, float y, float z);
		//void SetLocalRotation(PGQuaternion& rot);
		//void SetLocalScale(float x, float y, float z);
		//void SetLocalScale(PGFLOAT3& sca);
		//void SetLocalRotationEuler(float x, float y, float z);
		//void SetLocalRotationEuler(PGFLOAT3& euler);

		// �� ����, ���� transform�� ���� ���
		PGQuaternion NormalizeQuaternion(PGQuaternion q);

		PGFLOAT4X4 GetLocalTranslateMatrix();
		PGFLOAT4X4 GetLocalScaleMatrix();
		PGFLOAT4X4 GetLocalRotationMatrix();

		PGFLOAT4X4 GetWorldTranslateMatrix();
		PGFLOAT4X4 GetWorldScaleMatrix();
		PGFLOAT4X4 GetWorldRotationMatrix();

		// ������Ʈ�� ����, ���, ������ ���� (����� �׸��ų�... �� �� Ȱ��)
		PGFLOAT3 GetForward();
		PGFLOAT3 GetUp();
		PGFLOAT3 GetRight();

		// ������ ������ ����... ���Ϸ� && ���ʹϾ� ��ȯ �Լ���
		PGQuaternion EulerToQuaternion(float x, float y, float z);
		PGQuaternion EulerToQuaternion(const PGFLOAT3& euler);
		PGFLOAT3 QuaternionToEuler(float w, float x, float y, float z);
		PGFLOAT3 QuaternionToEuler(const PGQuaternion& quaternion);

		// ���� ��� (�θ� ������ ���)
		PGFLOAT4X4 GetWorldTM();
		PGFLOAT3 GetWorldPosition();
		PGQuaternion GetWorldRotation();
		PGFLOAT4X4 GetLocalTM();

		// ���Ӽ� ���� �Լ�
		Transform* GetParent();
		const std::vector<Transform*>& GetChildren();
		Transform* GetChild(int index);
		Transform* GetChild(std::string name);

		bool HasParent();
		void AddChild(Transform* child);
		void AddChild(GameObject* child);

		void RemoveChild(GameObject* child);
		void RemoveChild(std::string child);


		/// ������Ʈ�� 3D ���θ� ����... Transform�� �ִ°� ������?
		void SetIs3D(bool is3D);
		bool Is3D();

		//Culling�� ����, Graphics�� ����ϴ� ���� �Լ�.
		//�������� x,y,z �� Ȧ������ �����ΰ�?
		bool IsScaleOddMinus();

		//�̵��� �������� �� ��ȯ. (�ν��Ͻ� ����)
		bool GetCanMove();

	public:
		/// visit_struct ���
		BEGIN_VISITABLES(Transform);
		VISITABLE(PGFLOAT3, _position);
		VISITABLE(PGQuaternion, _rotation);
		VISITABLE(PGFLOAT3, _scale);
		END_VISITABLES;

	private:
		// ī�޶� ���� transform �Լ�
		// ī�޶󿡼� rotation�� �Ͼ�� ������Ʈ�� ���� ���͸� ��ȯ���ش�
		//PGFLOAT3 SetForwardUpRight(PGFLOAT3 up, PGFLOAT3 right, PGFLOAT3 forward);
		
		// �θ�, �ڽ� ��ü�� ����Ű�� transform
		Transform* _parent{ nullptr };
		std::vector<Transform*> _children;

		// Forward, Right, Up
		PGFLOAT3 _forward;
		PGFLOAT3 _right;
		PGFLOAT3 _up;

		// bool 
		bool _is3D;

		//�ʱⰪ True.
		bool _isCanMove;
	};
}

