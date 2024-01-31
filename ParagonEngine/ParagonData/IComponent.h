#pragma once
#include "PhysicsCollision.h"

/// <summary>
/// 컴포넌트의 인터페이스.
/// 이를 포함한 GameObject 나 Scene 기능을 코어에서 만들어
/// 게임엔진과 그래픽스 엔진에서 가져다 쓴다...맞겠지?
/// 2023.09.11
/// </summary>

namespace Pg::Data
{
	class IComponent
	{
	public:
		virtual void Awake() abstract;
		virtual void Start() abstract;
		virtual void Update() abstract;
		virtual void FixedUpdate() abstract;
		virtual void LateUpdate() abstract;

		virtual void OnDestroy() abstract;
		virtual void OnCollisionEnter(PhysicsCollision** _colArr, unsigned int count) abstract;
		virtual void OnCollisionStay() abstract;
		virtual void OnCollisionExit(PhysicsCollision** _colArr, unsigned int count) abstract;

		virtual void OnTriggerEnter(Collider* c) abstract;
		virtual void OnTriggerStay() abstract;
		virtual void OnTriggerExit(Collider* c) abstract;
	};
}
