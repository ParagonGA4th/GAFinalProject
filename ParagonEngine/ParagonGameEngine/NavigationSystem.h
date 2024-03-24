#pragma once
#include "../ParagonMath/PgMath.h"
#include "../ParagonData/BuildSettings.h"

#include <Detour/DetourNavMesh.h>
#include <Detour/DetourNavMeshBuilder.h>
#include <Recast/Recast.h>
#include <DetourTileCache/DetourTileCache.h>
#include <DetourTileCache/DetourTileCacheBuilder.h>
#include <Detour/DetourNavMeshQuery.h>
#include <Detour/DetourCommon.h>
#include <Detour/DetourAssert.h>
#include <Detour/DetourMath.h>
#include <Detour/DetourAlloc.h>
#include <DetourCrowd/DetourCrowd.h>

#include <unordered_map>
#include <vector>
#include <memory>

/// <summary>
/// 변지상의 Recast를 적용한 Navigation.
/// AI를 위해서는 pathFinding이 적용되어야 한다.
/// 2024.02.23
/// </summary>
namespace Pg::Data
{
	class NavigationField;
	class NavMeshAgent;
	class PlaneCollider;
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

		//NavMesh 생성
		void SyncNavMesh();

		void BuildPlaneNavMesh(const float* worldVertices, size_t verticesNum, 
			const int* faces, size_t facesNum, const Pg::Data::BuildSettings& buildSettings);
		
		//함수 오버로드
		void BuildPlaneNavMesh(std::vector<Pg::Math::PGFLOAT3> worldVertices, std::vector<int> faces,
			const Pg::Data::BuildSettings& buildSettings = Pg::Data::BuildSettings{})
		{
			static_assert(sizeof(Pg::Math::PGFLOAT3) == sizeof(float) * 3);
			assert(!worldVertices.empty() && !faces.empty());
			assert(faces.size() % 3 == 0);
			BuildPlaneNavMesh(reinterpret_cast<float*>(&(worldVertices[0].x)), worldVertices.size(), &faces[0], faces.size() / 3, buildSettings);
		}

		void CreatePlaneNavMesh(Pg::Data::PlaneCollider* planeCollider, std::vector<Pg::Math::PGFLOAT3>& worldVertices, std::vector<int>& worldFaces);

		void MoveTo(Pg::Data::NavMeshAgent* agent, Pg::Math::PGFLOAT3 des);

		int rasterizeTileLayers(const float* worldVertices, size_t verticesNum, const int* faces, size_t facesNum, const int tx, const int ty, const rcConfig& cfg, struct TileCacheData* tiles, const int maxTiles);

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
		dtTileCache* _tileCache;

		const dtQueryFilter* _filter;
		const float* _halfExtents;

		SceneSystem* _sceneSystem = nullptr;

		//Recast.h 관련 클래스(설정)
		std::unique_ptr<rcContext> _rcContext;
		rcConfig _rcConfig;
		rcPolyMesh* _polyMesh;
		rcPolyMeshDetail* _polyMeshDetail;
		

		//컴포넌트 관리할 벡터
		std::vector<Pg::Data::NavigationField*> _navMeshFieldVec;
		std::vector<Pg::Data::NavMeshAgent*> _navMeshAgentVec;

		std::vector<Pg::Math::PGFLOAT3> worldVertices;
		std::vector<int> worldIndices;
	};
}

