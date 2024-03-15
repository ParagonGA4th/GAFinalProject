#include "NavigationSystem.h"
#include "SceneSystem.h"

#include "../ParagonData/PlaneCollider.h"
#include "../ParagonData/NavigationField.h"
#include "../ParagonData/NavMeshAgent.h"

#include <singleton-cpp/singleton.h>

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

		//_navMeshAgent->GetCrowd() = dtAllocCrowd();

		//_navMeshQuery = dtAllocNavMeshQuery();
	}

	void NavigationSystem::Update(float deltaTime)
	{
		if (_navMesh == nullptr || _navMeshAgent->GetCrowd() == nullptr)
		{
			return;
		}

		_navMeshAgent->GetCrowd()->update(deltaTime, nullptr);
	}

	void NavigationSystem::Finalize()
	{
		//전부 해제.
		_navMeshFieldVec.clear();

		dtFreeCrowd(_navMeshAgent->GetCrowd());
		
		dtFreeNavMeshQuery(_navMeshQuery);
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

	void NavigationSystem::AddAgent()
	{

	}
}