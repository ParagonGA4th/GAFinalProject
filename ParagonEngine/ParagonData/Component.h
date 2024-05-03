#pragma once
#include "SerializerHelper.h"
#include "PhysicsCollision.h"

/// <summary>
/// 컴포넌트 클래스
/// overload 엔진 참고해서 만들어갈 예정
/// 2023.09.11
/// </summary>

namespace Pg::Data
{
	class GameObject;
	class Component
	{
	public:
		Component(GameObject* obj);
		//가상 소멸자
		virtual ~Component();

	public:
		//게임 로직 내부에서 실제로 사용될 일은 없다.
		virtual void Internal_EngineAwake() {}
		virtual void Awake() {}
		virtual void Start() {}
		//게임 로직 내부에서 실제로 사용될 일은 없다.
		virtual void Internal_EngineUpdate() {}
		virtual void BeforePhysicsUpdate() {} //무슨 이유에선지 Physics System 실행 전에 Update를 돌리고 싶을 때. Awake()보다도 먼저다.
		virtual void Update() {}
		virtual void FixedUpdate() {}
		virtual void LateUpdate() {}

		//OnEngineStop (Static Scene List 전용)
		virtual void OnEngineStop() {}

		virtual void OnDestroy() {}
		virtual void OnCollisionEnter(PhysicsCollision** _colArr, unsigned int count) {}
		virtual void OnCollisionStay() {}
		virtual void OnCollisionExit(PhysicsCollision** _colArr, unsigned int count) {}

		virtual void OnTriggerEnter(Collider* c) {}
		virtual void OnTriggerStay() {}
		virtual void OnTriggerExit(Collider* c) {}

		virtual void OnSerialize(SerializeVector& sv) {}
		virtual void OnDeserialize(SerializeVector& sv) {}

	public:
		void SetActive(bool active);
		bool GetActive();

	public:
		GameObject* _object;
		bool _isActive;
	};
}