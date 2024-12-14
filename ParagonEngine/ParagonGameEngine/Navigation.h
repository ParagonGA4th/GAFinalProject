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
#include "../ParagonData/DebugData.h"
#include "NavGeom.h"

#include "EngineDLLExporter.h"

#include <unordered_map>
#include <vector>
#include <memory>

/// <summary>
/// 기존에 만들었던 NavigationSystem은 각 Field와 Agent를 컴포넌트화
/// 시킬려고 했으나 실패. 그러므로 .obj파일을 로드하여 빌드하는 방식으로 변경
/// 2024.04.20
/// </summary>
namespace Pg::Data
{
	struct NavCylinderInfo;
}

namespace Pg::Engine
{
	class NavMeshAgent;
	class SceneSystem;

	struct Agent
	{
		float _agentHeight = 2.0f;
		float _agentRadius = 1.2f;
		float _agentMaxClimb = 0.9f;
		float _agentMaxSlope = 45.0f;
		float _agentMaxSpeed = 10.0f;
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

		//Agent관리를 위한 변수
		dtCrowd* _crowd;
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

		PathFindbox();
		~PathFindbox();
	};

	class Navigation
	{
	public:
		PARAGON_ENGINE_DLL Navigation();
		PARAGON_ENGINE_DLL ~Navigation();

	public:
		// 초기화 함수.
		void Initialize();
		// 제거 함수
		void Release();
		// path 에서 이미 빌드된 네비매쉬 파일을 읽어옵니다. .bin파일
		void LoadAll(const char* path, int index);
		// 네비매쉬를 직접 빌드 하기
		bool HandleBuild(const std::string& path, int index);
		// 네비매쉬를 업데이트 한다.
		void HandleUpdate(const float dt);

		//매쉬를 빌드할 때, Tile단위로 불러오기 위함이다.
		int rasterizeTileLayers(const int tx, const int ty, const rcConfig& cfg, struct TileCacheData* tiles, const int maxTiles);
		void getTilePos(const float* pos, int& tx, int& ty);
		void renderCachedTile(const int tx, const int ty, const int type);
		void renderCachedTileOverlay(const int tx, const int ty, double* proj, double* model, int* view);

		// 직선경로 탐색 함수
		PARAGON_ENGINE_DLL std::vector <std::pair<Pg::Math::PGFLOAT3, Pg::Math::PGFLOAT3>> FindStraightPath(int index);
		// 이미 탐색된 경로를 가져오는 함수
		PARAGON_ENGINE_DLL std::vector<std::pair<Pg::Math::PGFLOAT3, Pg::Math::PGFLOAT3>> GetPath(int index);
		// Raycast 탐색 함수. (직선경로에 부딧히는게 있다면 거기까지만 경로 표시
		PARAGON_ENGINE_DLL Pg::Math::PGFLOAT3 FindRaycastPath(int index);

		//Agent 전부 추가.
		PARAGON_ENGINE_DLL void SyncAgent(int index, Pg::Math::PGFLOAT3 pos);

		// 장애물을 추가한다.
		// pos = 장애물 위치 / radius = 장애물 크기 / height = 장애물 사이즈
		PARAGON_ENGINE_DLL void AddTempObstacle(Pg::Math::PGFLOAT3 pos, float radius, float height);
		// pos = 장애물 위치 / bmin = 장애물 최소좌표 / bmax = 장애물 최대좌표
		PARAGON_ENGINE_DLL void AddBoxTempObstacle(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 bmin, DirectX::XMFLOAT3 bmax);
		// 특정 장애물을 제거한다.
		PARAGON_ENGINE_DLL void RemoveTempObstacle(DirectX::XMFLOAT3 pos);
		// bpos에 있는 장애물의 위치를 npos의 위치로 옮긴다.
		PARAGON_ENGINE_DLL void MoveTempObstacle(DirectX::XMFLOAT3 bpos, DirectX::XMFLOAT3 npos);
		// 모든 장애물을 제거한다.
		PARAGON_ENGINE_DLL void ClearAllTempObstacles();

		// startpos 와 endpos를 입력하는 함수. float[3] 버전
		PARAGON_ENGINE_DLL void SetSEpos(int index, float sx, float sy, float sz, float ex, float ey, float ez);
		// startpos 와 endpos를 입력하는 함수. XMFLOAT3 버전
		PARAGON_ENGINE_DLL void SetSEpos(int index, Pg::Math::PGFLOAT3 startPosition, Pg::Math::PGFLOAT3 endPosition);
		// startpos를 입력하는 함수. float[3] 버전
		PARAGON_ENGINE_DLL void SetStartpos(int index, float x, float y, float z);
		// startpos를 입력하는 함수. XMFLOAT3 버전
		PARAGON_ENGINE_DLL void SetStartpos(int index, Pg::Math::PGFLOAT3 position);
		// endpos를 입력하는 함수. float[3] 버전
		PARAGON_ENGINE_DLL void SetEndpos(int index, float x, float y, float z);
		// endpos를 입력하는 함수. XMFLOAT3 버전
		PARAGON_ENGINE_DLL void SetEndpos(int index, Pg::Math::PGFLOAT3 position);
		// 네비매쉬를 빌드하기 위한 agent를 세팅하는 함수. 각 변수명을 참고
		PARAGON_ENGINE_DLL void SetAgent(int index, float agentHeight, float agentMaxSlope, float agentRadius, float agentMaxClimb);

		// 네비매쉬를 빌드하기 위해 vertex와 index를 제공해주는 함수(예정)
		PARAGON_ENGINE_DLL void GetNavmeshRenderInfo(int index, std::vector<Pg::Math::PGFLOAT3>& vertices, std::vector<unsigned int>& indices);
		// 패키지(네비매쉬 배열) 사이즈 가져오기
		int GetPackageSize();
		// 현재 에이전트 세팅을 반환
		void GetAgent(int index, float& agentHeight, float& agentMaxSlope, float& agentRadius, float& agentMaxClimb);
		// 현재 네비매쉬 폴더 내 저장되어 있는 네비매쉬들을 가져옴
		std::vector<std::string> GetNavimeshPathList();

		dtObstacleRef hitTestObstacle(const dtTileCache* tc, const float* sq);

		bool isectSegAABB(const float* sq, const float* amin, const float* amax, float& tmin, float& tmax);
	
	private:
		Pg::Math::PGFLOAT3 vertex(const float* pos);
		
		///Navgation을 위해 필요한 변수들.
		static const int PACKAGESIZE = 5;

		class NavGeom* _geom;
		PathFindbox _package[PACKAGESIZE];

		bool _keepInterResults;
		float _totalBuildTimeMs;

		unsigned char* _triareas;

		///다중 Agent를 위해서면 리스트 형태로 관리해야 하지 않을까?
		///구조화 시 고민해야 함
		std::vector<Agent*> _agentVec;


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

	private:
		SceneSystem* _sceneSystem = nullptr;
		//Pg:Data::NavCylinderInfo* _navCyilnderInfo = nullptr ;
	};
}

