#pragma once
#include "Component.h"

#include <Detour/DetourCommon.h>
#include <DetourCrowd/DetourCrowd.h>
#include <Recast/Recast.h>

#include <functional>

/// <summary>
/// RecastDetour를 활용한 NavMesh 컴포넌트.
/// 추가할 시, 길찾기 및 AI가 적용된다.
/// 2024.03.09
/// </summary>

namespace Pg::Engine
{
	class NavigationSystem;
}

namespace Pg::Data
{
	class GameObject;

	class NavMeshAgent : public Component
	{
		friend class Pg::Engine::NavigationSystem;
	public:
		NavMeshAgent(GameObject* owner);

		virtual ~NavMeshAgent();

	public:
		virtual void Start() override;

		virtual void Update() override;

		void SetMaxSpeed(float speed);
		void SetRadius(float rad);
		void SetHeight(float height);
		void SetMaxAcceleration(float accel);
		void SetObstacleAvoidance(bool ob);
		void SetSeperation(bool sep);
		void SetDestination(Pg::Math::PGFLOAT3 des);
		void SetRelocate(Pg::Math::PGFLOAT3 des);

		float GetMaxSpeed();
		float GetRadius();
		float GetHeight();
		float GetMaxAcceleration();
		bool  GetObstacleAvoidance();
		bool GetSeparation();

		Pg::Math::PGFLOAT3 GetDestination();

	private:
		//Agent 설정값들.
		float _radius;

		float _height;

		float _maxSpeed;

		float _maxAcceleration;

		bool _obstacleAvoidance;

		unsigned char _obstacleAvoidanceType;

		bool _separation;

		float _separationWeight;

		bool _anticipateTurns;

		bool _optimizeVisibility;

		bool _optimizeTopology;

	private:
		int _agentidx{ -1 };

		float _targetPos[3];

		dtPolyRef _targetRef;
		const dtCrowdAgent* _crowdAgent;

		///런타임에 설정값 변경 시 필요.
		std::function<void(const Pg::Data::NavMeshAgent*)> _updateSystemFunc;
		std::function<void(Pg::Data::NavMeshAgent*, Pg::Math::PGFLOAT3)> _destinationFunc;
		std::function<void(Pg::Data::NavMeshAgent*, Pg::Math::PGFLOAT3)> _relocateFunc;

	private:
		Pg::Math::PGFLOAT3 _destination;
		Pg::Math::PGFLOAT3 _relocate;
	};
}

