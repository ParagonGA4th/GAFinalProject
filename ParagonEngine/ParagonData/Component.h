#pragma once
#include "SerializerHelper.h"
#include "PhysicsCollision.h"

/// <summary>
/// ������Ʈ Ŭ����
/// overload ���� �����ؼ� ���� ����
/// 2023.09.11
/// </summary>

namespace Pg::Data
{
	class GameObject;
	class Scene;

	class Component
	{
	public:
		Component(GameObject* obj);
		//���� �Ҹ���
		virtual ~Component();

	public:
		//���� ���� ���ο��� ������ ���� ���� ����.
		virtual void Internal_EngineAwake() {}
		virtual void BeforePhysicsAwake() {}
		virtual void Awake() {}
		virtual void Start() {}
		//���� ���� ���ο��� ������ ���� ���� ����.
		virtual void Internal_EngineUpdate() {}
		virtual void BeforePhysicsUpdate() {} //���� ���������� Physics System ���� ���� Update�� ������ ���� ��. Awake()���ٵ� ������.
		virtual void Update() {}
		virtual void FixedUpdate() {}
		virtual void LateUpdate() {}

		//OnEngineStop (Static Scene List ����)
		virtual void OnEngineStop() {}

		virtual void OnDestroy() {}
		virtual void OnCollisionEnter(PhysicsCollision** _colArr, unsigned int count) {}
		virtual void OnCollisionStay() {}
		virtual void OnCollisionExit(PhysicsCollision** _colArr, unsigned int count) {}

		virtual void OnTriggerEnter(Collider** _colArr, unsigned int count) {}
		virtual void OnTriggerStay() {}
		virtual void OnTriggerExit(Collider** _colArr, unsigned int count) {}

		virtual void OnAnimationEnd(const std::string& justEndedAnimation) {}

		virtual void OnSerialize(SerializeVector& sv) {}
		virtual void OnDeserialize(SerializeVector& sv) {}

		//�Ϲ� ������Ʈ / ������Ʈ�鿡 ���� ������ ���� ���̴�.
		//DontDestroyOnLoad�� ��ŷ�� �ֵ鸸,
		//�ش� �Լ��� Scene�� ����Ǿ��� �� ����ȴ�.
		virtual void OnSceneChange_Global(Pg::Data::Scene* changedScene) {}

		//�Ŵ������� �����ϴ� ������Ʈ�� ������ �� �ְ� �ϱ� ���ؼ�.
		virtual void GrabManagedObjects() {}

		//Don't Destroy�� �ƴ� ������Ʈ�� ����, Effect Object �� Update�� �������� ������Ʈ�� ���� ���
		//Manual Cleanup ����ϱ�.
		virtual void CleanOnSceneChange() {}

	public:
		void SetActive(bool active);
		bool GetActive();

	public:
		GameObject* _object;
		bool _isActive;
	};
}