#pragma once
#include "Component.h"

/// <summary>
/// RecastDetour를 활용한 NavMesh 컴포넌트.
/// 추가할 시, 길찾기 및 AI가 적용된다.
/// 2024.03.09
/// </summary>
namespace Pg::Data
{
	class GameObject;

	class NavMeshAgent : public Component
	{
	public:
		NavMeshAgent(GameObject* owner);

		virtual void Update() override;

		void SetMaxSpeed(float speed);
		void SetRadius(float rad);
		void SetHeight(float height);

		float GetMaxSpeed();
		float GetRadius();
		float GetHeight();

	private:
		float _radius;
		float _height;
		float _maxSpeed;
	};
}

