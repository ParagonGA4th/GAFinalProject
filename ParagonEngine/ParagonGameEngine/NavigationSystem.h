#pragma once
#include "../ParagonMath/PgMath.h"

#include <Detour/DetourNavMesh.h>
#include <Detour/DetourNavMeshBuilder.h>
#include <Detour/DetourNavMeshQuery.h>
#include <Detour/DetourCommon.h>
#include <DetourCrowd/DetourCrowd.h>
#include <DetourTileCache/DetourTileCache.h>
#include <Recast/Recast.h>

#include <unordered_map>
#include <vector>

/// <summary>
/// 변지상의 Recast를 적용한 Navigation.
/// AI를 위해서는 pathFinding이 적용되어야 한다.
/// 2024.02.23
/// </summary>
namespace Pg::Engine
{
	class NavigationField;

	class NavigationSystem
	{
	public:
		NavigationSystem();

		void Initialize();

		void Update(float deltaTime);

		void Finalize();

		void AddAgent();

		void CreatePlaneNavMesh();

		dtNavMesh* GetNavMesh() const;

		dtCrowd* GetCrowd() const;

		dtNavMeshQuery* GetNavMeshQuery() const;

	private:
		//Recast에 필요한 변수들
		dtNavMesh* _navMesh;
		dtCrowd* _crowd;
		dtNavMeshQuery* _navMeshQuery;

		//Recast.h 관련 클래스(설정)
		rcContext* _rcContext;
		rcConfig _rcConfig;

		//컴포넌트 관리할 벡터
		std::vector<NavigationField*> _navMeshFieldVec;

		//NavMesh를 생성할 컴포넌트들
		NavigationField* _navMeshField;
	};
}

