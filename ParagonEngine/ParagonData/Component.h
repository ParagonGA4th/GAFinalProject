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
	class Scene;

	class Component
	{
	public:
		Component(GameObject* obj);
		//가상 소멸자
		virtual ~Component();

	public:
		//게임 로직 내부에서 실제로 사용될 일은 없다.
		virtual void Internal_EngineAwake() {}
		virtual void BeforePhysicsAwake() {}
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

		virtual void OnTriggerEnter(Collider** _colArr, unsigned int count) {}
		virtual void OnTriggerStay() {}
		virtual void OnTriggerExit(Collider** _colArr, unsigned int count) {}

		virtual void OnAnimationEnd(const std::string& justEndedAnimation) {}

		virtual void OnSerialize(SerializeVector& sv) {}
		virtual void OnDeserialize(SerializeVector& sv) {}

		//일반 오브젝트 / 컴포넌트들에 한해 영향이 없을 것이다.
		//DontDestroyOnLoad로 마킹된 애들만,
		//해당 함수가 Scene이 변경되었을 때 실행된다.
		virtual void OnSceneChange_Global(Pg::Data::Scene* changedScene) {}

		//매니저들이 관리하는 오브젝트를 가져올 수 있게 하기 위해서.
		virtual void GrabManagedObjects() {}

		//Don't Destroy가 아닌 오브젝트들 한정, Effect Object 등 Update와 독립적인 오브젝트들 있을 경우
		//Manual Cleanup 허용하기.
		virtual void CleanOnSceneChange() {}

	public:
		void SetActive(bool active);
		bool GetActive();

	public:
		GameObject* _object;
		bool _isActive;
	};
}