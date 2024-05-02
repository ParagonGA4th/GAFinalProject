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
#include "../ParagonProcess/CoreSingleton.h"
#include "../ParagonMath/PgMath.h"
#include "NavGeom.h"

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
	struct Agent
	{
		float _agentHeight = 2.0f;
		float _agentRadius = 1.2f;
		float _agentMaxClimb = 0.9f;
		float _agentMaxSlope = 45.0f;
	};

	// 이 PathFIndbox는 네비매쉬를 배열로 관리하기 편하도록 구성 요소들을 묶은 것
	struct PathFindbox
	{
		class dtNavMesh* _navMesh;
		class dtNavMeshQuery* _navQuery;
		class dtTileCache* _tileCache;

		// PathFind를 위해 필요한 부분들
		static const int MAX_POLYS = 256;
		static const int MAX_SMOOTH = 2048;

		// 에이전트 세팅
		Agent _agentsetting;

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
		bool HandleBuild(int index);
		// 네비매쉬를 업데이트 한다.
		void HandleUpdate(const float dt);

		int rasterizeTileLayers(const int tx, const int ty, const rcConfig& cfg, struct TileCacheData* tiles, const int maxTiles);
		void getTilePos(const float* pos, int& tx, int& ty);
		void renderCachedTile(const int tx, const int ty, const int type);
		void renderCachedTileOverlay(const int tx, const int ty, double* proj, double* model, int* view);

		// 직선경로 탐색 함수
		//std::vector<std::pair<DirectX::XMFLOAT3, DirectX::XMFLOAT3>> FindStraightPath(int index);
		// 이미 탐색된 경로를 가져오는 함수
		//std::vector<std::pair<DirectX::XMFLOAT3, DirectX::XMFLOAT3>> GetPath(int index);
		// Raycast 탐색 함수. (직선경로에 부딧히는게 있다면 거기까지만 경로 표시

		//DirectX::XMFLOAT3 FindRaycastPath(int index);
		// startpos 와 endpos를 입력하는 함수. float[3] 버전
		void SetSEpos(int index, float sx, float sy, float sz, float ex, float ey, float ez);
		// startpos 와 endpos를 입력하는 함수. XMFLOAT3 버전
		void SetSEpos(int index, Pg::Math::PGFLOAT3 startPosition, Pg::Math::PGFLOAT3 endPosition);
		// startpos를 입력하는 함수. float[3] 버전
		void SetStartpos(int index, float x, float y, float z);
		// startpos를 입력하는 함수. XMFLOAT3 버전
		void SetStartpos(int index, Pg::Math::PGFLOAT3 position);
		// endpos를 입력하는 함수. float[3] 버전
		void SetEndpos(int index, float x, float y, float z);
		// endpos를 입력하는 함수. XMFLOAT3 버전
		void SetEndpos(int index, Pg::Math::PGFLOAT3 position);
		// 네비매쉬를 빌드하기 위한 agent를 세팅하는 함수. 각 변수명을 참고
		void SetAgent(int index, float agentHeight, float agentMaxSlope, float agentRadius, float agentMaxClimb);

		dtObstacleRef hitTestObstacle(const dtTileCache* tc, const float* sp, const float* sq);

		bool isectSegAABB(const float* sp, const float* sq, const float* amin, const float* amax, float& tmin, float& tmax);
	private:

		static const int PACKAGESIZE = 5;

		class NavGeom* _geom;
		PathFindbox _package[PACKAGESIZE];

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

		float _cacheBuildTimeMs;
		int _cacheCompressedSize;
		int _cacheRawSize;
		int _cacheLayerCount;
		unsigned int _cacheBuildMemUsage;

		int _maxTiles;
		int _maxPolysPerTile;
		float _tileSize = 48.f;

		enum DrawMode
		{
			DRAWMODE_NAVMESH,
			DRAWMODE_NAVMESH_TRANS,
			DRAWMODE_NAVMESH_BVTREE,
			DRAWMODE_NAVMESH_NODES,
			DRAWMODE_NAVMESH_PORTALS,
			DRAWMODE_NAVMESH_INVIS,
			DRAWMODE_MESH,
			DRAWMODE_CACHE_BOUNDS,
			MAX_DRAWMODE
		};
	};
}

