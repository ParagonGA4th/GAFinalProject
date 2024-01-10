#pragma once
#include <functional>
#include <PxPhysicsAPI.h>

namespace Pg::Engine
{
	class PhysicsCollision;
}

/// <summary>
/// 모든 콜라이더들의 부모클래스가 되어야 한다!
/// 아마 Component들을 받게 되어, GameObject의 함수를 호출하는 방식 등으로 사용되어야 할 것.
/// 
/// Collider로 대체되어야.
/// </summary>

namespace Pg::Engine
{
	class PhysicsColliderActor
	{
	public:
		virtual void OnCollisionEnter(const PhysicsCollision& c);
		virtual void OnCollisionExit(const PhysicsCollision& c);
		virtual void OnTriggerEnter(PhysicsColliderActor* c);
		virtual void OnTriggerExit(PhysicsColliderActor* c);


	public:
		//해당 콜라이더에 대해 Collision Start가 등록되었을 때 호출 (막 충돌했을 때)
		std::function<void(const PhysicsCollision&)> _collisionEnter;

		//해당 콜라이더에 대해 Collision Exit가 등록되었을 때 호출. (막 충돌을 끝냈을 때)
		std::function<void(const PhysicsCollision&)> _collisionExit;

		//해당 콜라이더에 대한 Trigger Start가 등록되었을 때 (막 콜라이더가 Event Trigger를 했을 때)
		std::function<void(PhysicsColliderActor*)> _triggerEnter;

		//해당 콜라이더에 대한 Trigger End가 등록되었을 때 (막 콜라이더가 Event Trigger를 했을 때)
		std::function<void(PhysicsColliderActor*)> _triggerExit;
	};
}

