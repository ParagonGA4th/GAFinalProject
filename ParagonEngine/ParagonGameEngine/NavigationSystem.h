#pragma once
#include <Detour/DetourNavMesh.h>
#include <Detour/DetourNavMeshQuery.h>
#include <Detour/DetourCommon.h>
#include <DetourCrowd/DetourCrowd.h>

#include <unordered_map>

/// <summary>
/// 변지상의 Recast를 적용한 Navigation.
/// AI를 위해서는 pathFinding이 적용되어야 한다.
/// 2024.02.23
/// </summary>
namespace Pg::Engine
{
	class NavigationSystem
	{
	public:
		NavigationSystem();

		void Initialize();

		void Update(float deltaTime);

		void Finalize();

		void AddAgent();

		dtNavMesh* GetNavMesh() const;

		dtCrowd* GetCrowd() const;

		dtNavMeshQuery* GetNavMeshQuery() const;

	private:
		//Recast에 필요한 변수들
		dtNavMesh* _navMesh;
		dtCrowd* _crowd;
		dtNavMeshQuery* _navMeshQuery;
	};
}

