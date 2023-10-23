#pragma once
#include "IComponent.h"
#include "ISerializable.h"

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
	class Component : public IComponent, public ISerializable
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

		// Scene Data -> XML
		virtual void OnSerialize() override {}
		// XML -> Scene Data
		virtual void OnDeserialize() override {}
		// Scene Data -> Editor Data
		virtual void OnDataStructure() override {}

	public:
		void SetActive(bool active);
		bool GetActive();

	public:
		GameObject* _object;
		bool _isActive;
	};

}

