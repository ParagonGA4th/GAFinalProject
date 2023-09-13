#pragma once


/// <summary>
/// 컴포넌트 클래스
/// overload 엔진 참고해서 만들어갈 예정
/// 2023.09.11
/// </summary>

namespace Pg::Core
{
	class GameObject;
}

namespace Pg::Core
{
	class Component
	{
	public:
		Component(GameObject& obj);
		virtual ~Component();

	public:
		virtual void Awake() {}
		virtual void Start() {}
		virtual void Update() {}
		virtual void FixedUpdate() {}
		virtual void LateUpdate() {}

		virtual void OnDestroy() {}
		virtual void OnCollisionEnter() {}
		virtual void OnCollisionStay() {}
		virtual void OnCollisionExit() {}

	public:
		GameObject& _object;
	};

}

