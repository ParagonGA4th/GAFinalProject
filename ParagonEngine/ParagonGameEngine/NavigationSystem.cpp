#include "NavigationSystem.h"
#include "SceneSystem.h"

#include "../ParagonData/PlaneCollider.h"
#include "../ParagonData/NavigationField.h"
#include "../ParagonData/NavMeshAgent.h"
#include "../ParagonMath/PgMath.h"
#include "../ParagonUtil/Log.h"

#include <singleton-cpp/singleton.h>
#include <functional>

namespace Pg::Engine
{
	NavigationSystem::NavigationSystem() :
		_navMesh(nullptr),
		_navMeshQuery(nullptr)
	{

	}

	void NavigationSystem::Initialize()
	{
		memset(&_rcConfig, 0, sizeof(_rcConfig));

		_rcConfig.cs = 0.3f; // 셀 사이즈
		_rcConfig.ch = 0.2f; // 셀 높이
		_rcConfig.walkableSlopeAngle = 45.0f;	//경사
		_rcConfig.walkableHeight = 2.0f;
		_rcConfig.walkableClimb = 0.9f;
		_rcConfig.walkableRadius = 0.6f;
		_rcConfig.maxEdgeLen = 12.0f;
		_rcConfig.maxSimplificationError = 1.3f;
		_rcConfig.minRegionArea = 8.0f;
		_rcConfig.mergeRegionArea = 20.0f;
		_rcConfig.maxVertsPerPoly = 6;
		_rcConfig.detailSampleDist = 6.0f;
		_rcConfig.detailSampleMaxError = 1.0f;

		//Crowd와 NavMeshQuery의 구조체 초기화 및 할당
		_crowd = dtAllocCrowd();

		_navMeshQuery = dtAllocNavMeshQuery();

		//에이전트 모두 생성.
		//SyncAgents();
	}

	void NavigationSystem::Update(float deltaTime)
	{
		if (_crowd == nullptr || _navMesh == nullptr)
		{
			//PG_TRACE("Crowd 존재하지 않음.");
			return;
		}

		_crowd->update(deltaTime, nullptr);
	}

	void NavigationSystem::Finalize()
	{
		//전부 해제.
		_navMeshFieldVec.clear();
		_navMeshAgentVec.clear();

		dtFreeCrowd(_crowd);
		
		dtFreeNavMeshQuery(_navMeshQuery);
	}

	void NavigationSystem::SyncAgents()
	{
		//원래 있던 NavMeshAgentVec();
		if (!_navMeshAgentVec.empty())
		{
			_navMeshAgentVec.clear();
		}

		//싱글턴
		auto& tSceneSystem = singleton<SceneSystem>();
		_sceneSystem = &tSceneSystem;

		for (auto& it : _sceneSystem->GetCurrentScene()->GetObjectList())
		{
			Pg::Data::NavMeshAgent* tNavMeshAgent = it->GetComponent<Pg::Data::NavMeshAgent>();

			if (tNavMeshAgent != nullptr)
			{
				//NavMeshAgent 컴포넌트 들어간 애들만 AddAgent 해줌.
				//Agent의 속성 부여
				dtCrowdAgentParams ap;
				memset(&ap, 0, sizeof(ap));
				
				ap.radius = tNavMeshAgent->GetRadius();
				ap.maxSpeed = tNavMeshAgent->GetMaxSpeed();
				ap.height = tNavMeshAgent->GetHeight();
				ap.maxAcceleration = tNavMeshAgent->GetMaxAcceleration();
				ap.collisionQueryRange = ap.radius * 12.0f;
				ap.pathOptimizationRange = ap.radius * 30.0f;

				ap.updateFlags = 0;

				if (tNavMeshAgent->_anticipateTurns)
					ap.updateFlags |= DT_CROWD_ANTICIPATE_TURNS;

				if (tNavMeshAgent->_optimizeVisibility)
					ap.updateFlags |= DT_CROWD_OPTIMIZE_VIS;

				if (tNavMeshAgent->_optimizeTopology)
					ap.updateFlags |= DT_CROWD_OPTIMIZE_TOPO;

				if (tNavMeshAgent->_obstacleAvoidance)
					ap.updateFlags |= DT_CROWD_OBSTACLE_AVOIDANCE;

				if (tNavMeshAgent->_separation)
					ap.updateFlags |= DT_CROWD_SEPARATION;

				ap.obstacleAvoidanceType = static_cast<unsigned char>(tNavMeshAgent->_obstacleAvoidanceType);
				ap.separationWeight = tNavMeshAgent->_separationWeight;

				//agent의 현재 위치는 설정해놓은 포지션으로 정해놓는다.
				Pg::Math::PGFLOAT3 agentPos = tNavMeshAgent->_object->_transform._position;

				_crowd->addAgent(reinterpret_cast<const float*>(&agentPos), &ap);

				_navMeshAgentVec.push_back(tNavMeshAgent);
				
				///런타임에 설정값이 변경될 때 필요함.
				//tNavMeshAgent->_updateSystemFunc = std::bind(&NavigationSystem::UpdateSingleDtParam, this, std::placeholders::_1);
			}

		}

		assert("");
	}

	void NavigationSystem::CreatePlaneNavMesh()
	{

	}

	dtNavMesh* NavigationSystem::GetNavMesh() const
	{
		return _navMesh;
	}

	dtNavMeshQuery* NavigationSystem::GetNavMeshQuery() const
	{
		return _navMeshQuery;
	}

	void NavigationSystem::UpdateSingleDtParam(const Pg::Data::NavMeshAgent* navAgent)
	{
		//_crowd->updateAgentParameters()
	}
}