#pragma once
#include "PhysicsCollision.h"

/// <summary>
/// ������Ʈ�� �������̽�.
/// �̸� ������ GameObject �� Scene ����� �ھ�� �����
/// ���ӿ����� �׷��Ƚ� �������� ������ ����...�°���?
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
