#pragma once
#include "IComponent.h"

/// <summary>
/// 컴포넌트 클래스
/// overload 엔진 참고해서 만들어갈 예정
/// 2023.09.11
/// </summary>

namespace Pg::Data
{
	class GameObject;
}

namespace Pg::Data
{
	class Component : public IComponent
	{
	public:
		Component(GameObject* obj);
		//가상 소멸자
		virtual ~Component();

	public:
		virtual void Awake() override {}
		virtual void Start() override {}
		virtual void Update() override {}
		virtual void FixedUpdate() override {}
		virtual void LateUpdate() override {}

		virtual void OnDestroy() override {}
		virtual void OnCollisionEnter() override {}
		virtual void OnCollisionStay() override {}
		virtual void OnCollisionExit() override {}

		virtual void OnTriggerEnter() override {}
		virtual void OnTriggerStay() override {}
		virtual void OnTriggerExit() override {}

	public:
		void SetActive(bool active);
		bool GetActive();

	public:
		GameObject* _object;
		bool _isActive;
	};

}

