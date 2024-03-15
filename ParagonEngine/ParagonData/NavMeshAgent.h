#pragma once
#include "Component.h"

#include <Detour/DetourCommon.h>
#include <DetourCrowd/DetourCrowd.h>
#include <Recast/Recast.h>

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

	public:
		virtual void Update() override;

		//Navigation 기능 적용될 오브젝트 만든다.
		void AddAgent();

		void SetMaxSpeed(float speed);
		void SetRadius(float rad);
		void SetHeight(float height);
		void SetMaxAcceleration(float accel);

		float GetMaxSpeed();
		float GetRadius();
		float GetHeight();
		float GetMaxAcceleration();

		dtCrowd*& GetCrowd();

	private:
		dtCrowd* _crowd;

	private:
		float _radius;
		float _height;
		float _maxSpeed;
		float _maxAcceleration;
	};
}

