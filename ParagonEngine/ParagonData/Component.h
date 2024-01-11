#pragma once
#include "SerializerHelper.h"

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
		virtual void Awake() {}
		virtual void Start() {}
		virtual void Update() {}
		virtual void FixedUpdate() {}
		virtual void LateUpdate() {}

		virtual void OnDestroy() {}
		virtual void OnCollisionEnter() {}
		virtual void OnCollisionStay() {}
		virtual void OnCollisionExit() {}

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

