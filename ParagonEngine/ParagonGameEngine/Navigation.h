#pragma once

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

#include "../ParagonData/BuildSettings.h"

#include <unordered_map>
#include <vector>
#include <memory>

/// <summary>
/// 기존에 만들었던 NavigationSystem은 각 Field와 Agent를 컴포넌트화
/// 시킬려고 했으나 실패. 그러므로 .obj파일을 로드하여 빌드하는 방식으로 변경
/// 2024.04.20
/// </summary>
namespace Pg::Engine
{
	// 이 PathFIndbox는 네비매쉬를 배열로 관리하기 편하도록 구성 요소들을 묶은 것
	struct PathFindbox
	{
		class dtNavMesh* _navMesh;
		class dtNavMeshQuery* _navQuery;

		// PathFind를 위해 필요한 부분들
		static const int MAX_POLYS = 256;
		static const int MAX_SMOOTH = 2048;

		dtPolyRef _startRef;
		dtPolyRef _endRef;
		dtPolyRef _parent[MAX_POLYS];
		float _startPos[3];
		float _endPos[3];
		dtQueryFilter _filter;
		dtPolyRef _path[MAX_POLYS];
		int _pathCount;
		float _polyPickExt[3];

		// Straight-pathfind를 위해 필요한 부분들
		float _straightPath[MAX_POLYS * 3];
		unsigned char _straightPathFlags[MAX_POLYS];
		dtPolyRef _straightPathPolys[MAX_POLYS];
		int _nstraightPath;

		// Raycast-pathfind를 위해 필요한 부분들
		dtRaycastHit _hit;
		dtPolyRef _RaycastPathPolys;
		float _hitPos[3];

		//PathFindbox();
		//~PathFindbox();
	};

	class Navigation
	{
	public:
		Navigation();
		~Navigation();

	public:
		// 초기화 함수.
		void Initialize();
		// 제거 함수
		void Release();
		// path 에서 이미 빌드된 네비매쉬 파일을 읽어옵니다. .bin파일
		void LoadAll(const char* path, int index);
		// 네비매쉬를 직접 빌드 하기
		bool HandleBuild(const Pg::Data::BuildSettings& buildSettings, const float* worldVertices, size_t verticesNum, const int* faces, size_t facesNum);
		// 네비매쉬를 업데이트 한다.
		void HandleUpdate(const float dt);

		int rasterizeTileLayers(const float* worldVertices, size_t verticesNum, const int* faces, size_t facesNum, const int tx, const int ty, const rcConfig& cfg, struct TileCacheData* tiles, const int maxTiles);
		void getTilePos(const float* pos, int& tx, int& ty);
		void renderCachedTile(const int tx, const int ty, const int type);
		void renderCachedTileOverlay(const int tx, const int ty, double* proj, double* model, int* view);
		// sp, sq를 기반으로 장애물의 ref 값을 반환하는 함수. sp,sq가 뭔진 아직 나도 모르겠음
		dtObstacleRef hitTestObstacle(const dtTileCache* tc, const float* sp, const float* sq);

		bool isectSegAABB(const float* sp, const float* sq, const float* amin, const float* amax, float& tmin, float& tmax);
	private:
		bool _keepInterResults;
		float _totalBuildTimeMs;

		unsigned char* _triareas;

		//Recast에 필요한 변수들
		rcHeightfield* _solid;
		rcCompactHeightfield* _chf;
		rcContourSet* _cset;
		rcPolyMesh* _pmesh;
		rcConfig _cfg;
		rcPolyMeshDetail* _dmesh;
		rcContext* _ctx;

		unsigned char _navMeshDrawFlags;

		float _cellSize;
		float _cellHeight;
		float _agentHeight;
		float _agentRadius;
		float _agentMaxClimb;
		float _agentMaxSlope;
		float _regionMinSize;
		float _regionMergeSize;
		float _edgeMaxLen;
		float _edgeMaxError;
		float _vertsPerPoly;
		float _detailSampleDist;
		float _detailSampleMaxError;
		int _partitionType;

		bool _filterLowHangingObstacles;
		bool _filterLedgeSpans;
		bool _filterWalkableLowHeightSpans;

		// solo_mesh -> temp+obstacle로 바꾸면서 추가된 부분
		struct LinearAllocator* _talloc;
		struct FastLZCompressor* _tcomp;
		struct MeshProcess* _tmproc;

		dtTileCache* _tileCache;

		float _cacheBuildTimeMs;
		int _cacheCompressedSize;
		int _cacheRawSize;
		int _cacheLayerCount;
		unsigned int _cacheBuildMemUsage;

		PathFindbox _package[5];
		int _maxTiles;
		int _maxPolysPerTile;
	};
}

