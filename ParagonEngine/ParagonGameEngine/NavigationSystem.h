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
#include <RecastDemo/ChunkyTriMesh.h>


#include <unordered_map>
#include <vector>
#include <memory>

/// <summary>
/// �������� Recast�� ������ Navigation.
/// AI�� ���ؼ��� pathFinding�� ����Ǿ�� �Ѵ�.
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
		~NavigationSystem();

	public:
		void Initialize();

		void Update(float deltaTime);

		void Finalize();

		//Agent ���� �� ����
		void SyncAgents();
		void RemoveAgent(int index);

		//NavMesh ����
		void SyncNavMesh();

		void BuildPlaneNavMesh(const float* worldVertices, size_t verticesNum, 
			const int* faces, size_t facesNum, const Pg::Data::BuildSettings& buildSettings);
		
		//�Լ� �����ε�
		void BuildPlaneNavMesh(std::vector<Pg::Math::PGFLOAT3> worldVertices, std::vector<int> faces,
			const Pg::Data::BuildSettings& buildSettings = Pg::Data::BuildSettings{})
		{
			static_assert(sizeof(Pg::Math::PGFLOAT3) == sizeof(float) * 3);
			if(worldVertices.empty() && faces.empty()) return;

			assert(!worldVertices.empty() && !faces.empty());
			assert(faces.size() % 3 == 0);
			BuildPlaneNavMesh(reinterpret_cast<float*>(&(worldVertices[0].x)), worldVertices.size(), &faces[0], faces.size() / 3, buildSettings);
		}

		void CreatePlaneNavMesh(Pg::Data::PlaneCollider* planeCollider, std::vector<Pg::Math::PGFLOAT3>& worldVertices, std::vector<int>& worldFaces);

		void MoveTo(Pg::Data::NavMeshAgent* agent, Pg::Math::PGFLOAT3 des);

		void Relocate(Pg::Data::NavMeshAgent* agent, Pg::Math::PGFLOAT3 des);

		int rasterizeTileLayers(const float* worldVertices, size_t verticesNum, const int* faces, size_t facesNum, const int tx, const int ty, const rcConfig& cfg, struct TileCacheData* tiles, const int maxTiles);

		int calcLayerBufferSize(const int gridWidth, const int gridHeight);

		dtNavMesh* GetNavMesh() const;

		dtNavMeshQuery* GetNavMeshQuery() const;

	private:

		///��Ÿ�ӿ� �������� ����� �� �ʿ���.
		void UpdateSingleDtParam(const Pg::Data::NavMeshAgent* navAgent);

	private:
		//Recast�� �ʿ��� ������
		dtNavMesh* _navMesh;
		dtNavMeshQuery* _navMeshQuery;
		dtCrowd* _crowd;
		dtTileCache* _tileCache;

		const dtQueryFilter* _filter;
		const float* _halfExtents;

		SceneSystem* _sceneSystem = nullptr;

		//Recast.h ���� Ŭ����(����)
		std::unique_ptr<rcContext> _rcContext;
		rcConfig _rcConfig;
		rcPolyMesh* _polyMesh;
		rcPolyMeshDetail* _polyMeshDetail;

		//Ŀ���� ����ü
		struct LinearAllocator* m_talloc;
		struct FastLZCompressor* m_tcomp;
		struct MeshProcess* m_tmproc;

		//������Ʈ ������ ����
		std::vector<Pg::Data::NavigationField*> _navMeshFieldVec;
		std::vector<Pg::Data::NavMeshAgent*> _navMeshAgentVec;

		//navMeshField�� ���ؽ� �� �ε���
		std::vector<Pg::Math::PGFLOAT3> worldVertices;
		std::vector<int> worldIndices;

		//TileCache�� ����
		float m_cacheBuildTimeMs;
		int m_cacheCompressedSize;
		int m_cacheRawSize;
		int m_cacheLayerCount;
		unsigned int m_cacheBuildMemUsage;
		int m_maxTiles;
		int m_maxPolysPerTile;
		float m_tileSize;

		bool m_filterLowHangingObstacles;
		bool m_filterLedgeSpans;
		bool m_filterWalkableLowHeightSpans;
	};
}

