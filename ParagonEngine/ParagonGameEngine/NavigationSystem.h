#pragma once
#include "../ParagonMath/PgMath.h"

#include <DetourTileCache/DetourTileCache.h>
#include <Detour/DetourNavMesh.h>
#include <Detour/DetourNavMeshBuilder.h>
#include <Detour/DetourNavMeshQuery.h>
#include <Detour/DetourCommon.h>
#include <DetourCrowd/DetourCrowd.h>
#include <Recast/Recast.h>

#include <unordered_map>
#include <vector>

/// <summary>
/// 변지상의 Recast를 적용한 Navigation.
/// AI를 위해서는 pathFinding이 적용되어야 한다.
/// 2024.02.23
/// </summary>
namespace Pg::Data
{
	class NavigationField;
	class NavMeshAgent;
}

namespace Pg::Engine
{
	class SceneSystem;

	class NavigationSystem
	{
	public:
		NavigationSystem();

	public:
		void Initialize();

		void Update(float deltaTime);

		void Finalize();

		//Agent 생성 및 제거
		void SyncAgents();
		void RemoveAgent(int index);

		void CreatePlaneNavMesh();

		dtNavMesh* GetNavMesh() const;

		dtNavMeshQuery* GetNavMeshQuery() const;

	private:

		///런타임에 설정값이 변경될 때 필요함.
		void UpdateSingleDtParam(const Pg::Data::NavMeshAgent* navAgent);

	private:
		//Recast에 필요한 변수들
		dtNavMesh* _navMesh;
		dtNavMeshQuery* _navMeshQuery;
		dtCrowd* _crowd;

		SceneSystem* _sceneSystem = nullptr;

		//Recast.h 관련 클래스(설정)
		rcContext* _rcContext;
		rcConfig _rcConfig;
		rcPolyMesh* _polyMesh;
		rcPolyMeshDetail* _polyMeshDetail;
		

		//컴포넌트 관리할 벡터
		std::vector<Pg::Data::NavigationField*> _navMeshFieldVec;
		std::vector<Pg::Data::NavMeshAgent*> _navMeshAgentVec;
	};
}

