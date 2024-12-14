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
/// ������ ������� NavigationSystem�� �� Field�� Agent�� ������Ʈȭ
/// ��ų���� ������ ����. �׷��Ƿ� .obj������ �ε��Ͽ� �����ϴ� ������� ����
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

	// �� PathFIndbox�� �׺�Ž��� �迭�� �����ϱ� ���ϵ��� ���� ��ҵ��� ���� ��
	struct PathFindbox
	{
		class dtNavMesh* _navMesh;
		class dtNavMeshQuery* _navQuery;
		class dtTileCache* _tileCache;

		// PathFind�� ���� �ʿ��� �κе�
		static const int MAX_POLYS = 256;
		static const int MAX_SMOOTH = 2048;

		//Agent������ ���� ����
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

		// Straight-pathfind�� ���� �ʿ��� �κе�
		float _straightPath[MAX_POLYS * 3];
		unsigned char _straightPathFlags[MAX_POLYS];
		dtPolyRef _straightPathPolys[MAX_POLYS];
		int _nstraightPath;

		// Raycast-pathfind�� ���� �ʿ��� �κе�
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
		// �ʱ�ȭ �Լ�.
		void Initialize();
		// ���� �Լ�
		void Release();
		// path ���� �̹� ����� �׺�Ž� ������ �о�ɴϴ�. .bin����
		void LoadAll(const char* path, int index);
		// �׺�Ž��� ���� ���� �ϱ�
		bool HandleBuild(const std::string& path, int index);
		// �׺�Ž��� ������Ʈ �Ѵ�.
		void HandleUpdate(const float dt);

		//�Ž��� ������ ��, Tile������ �ҷ����� �����̴�.
		int rasterizeTileLayers(const int tx, const int ty, const rcConfig& cfg, struct TileCacheData* tiles, const int maxTiles);
		void getTilePos(const float* pos, int& tx, int& ty);
		void renderCachedTile(const int tx, const int ty, const int type);
		void renderCachedTileOverlay(const int tx, const int ty, double* proj, double* model, int* view);

		// ������� Ž�� �Լ�
		PARAGON_ENGINE_DLL std::vector <std::pair<Pg::Math::PGFLOAT3, Pg::Math::PGFLOAT3>> FindStraightPath(int index);
		// �̹� Ž���� ��θ� �������� �Լ�
		PARAGON_ENGINE_DLL std::vector<std::pair<Pg::Math::PGFLOAT3, Pg::Math::PGFLOAT3>> GetPath(int index);
		// Raycast Ž�� �Լ�. (������ο� �ε����°� �ִٸ� �ű������ ��� ǥ��
		PARAGON_ENGINE_DLL Pg::Math::PGFLOAT3 FindRaycastPath(int index);

		//Agent ���� �߰�.
		PARAGON_ENGINE_DLL void SyncAgent(int index, Pg::Math::PGFLOAT3 pos);

		// ��ֹ��� �߰��Ѵ�.
		// pos = ��ֹ� ��ġ / radius = ��ֹ� ũ�� / height = ��ֹ� ������
		PARAGON_ENGINE_DLL void AddTempObstacle(Pg::Math::PGFLOAT3 pos, float radius, float height);
		// pos = ��ֹ� ��ġ / bmin = ��ֹ� �ּ���ǥ / bmax = ��ֹ� �ִ���ǥ
		PARAGON_ENGINE_DLL void AddBoxTempObstacle(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 bmin, DirectX::XMFLOAT3 bmax);
		// Ư�� ��ֹ��� �����Ѵ�.
		PARAGON_ENGINE_DLL void RemoveTempObstacle(DirectX::XMFLOAT3 pos);
		// bpos�� �ִ� ��ֹ��� ��ġ�� npos�� ��ġ�� �ű��.
		PARAGON_ENGINE_DLL void MoveTempObstacle(DirectX::XMFLOAT3 bpos, DirectX::XMFLOAT3 npos);
		// ��� ��ֹ��� �����Ѵ�.
		PARAGON_ENGINE_DLL void ClearAllTempObstacles();

		// startpos �� endpos�� �Է��ϴ� �Լ�. float[3] ����
		PARAGON_ENGINE_DLL void SetSEpos(int index, float sx, float sy, float sz, float ex, float ey, float ez);
		// startpos �� endpos�� �Է��ϴ� �Լ�. XMFLOAT3 ����
		PARAGON_ENGINE_DLL void SetSEpos(int index, Pg::Math::PGFLOAT3 startPosition, Pg::Math::PGFLOAT3 endPosition);
		// startpos�� �Է��ϴ� �Լ�. float[3] ����
		PARAGON_ENGINE_DLL void SetStartpos(int index, float x, float y, float z);
		// startpos�� �Է��ϴ� �Լ�. XMFLOAT3 ����
		PARAGON_ENGINE_DLL void SetStartpos(int index, Pg::Math::PGFLOAT3 position);
		// endpos�� �Է��ϴ� �Լ�. float[3] ����
		PARAGON_ENGINE_DLL void SetEndpos(int index, float x, float y, float z);
		// endpos�� �Է��ϴ� �Լ�. XMFLOAT3 ����
		PARAGON_ENGINE_DLL void SetEndpos(int index, Pg::Math::PGFLOAT3 position);
		// �׺�Ž��� �����ϱ� ���� agent�� �����ϴ� �Լ�. �� �������� ����
		PARAGON_ENGINE_DLL void SetAgent(int index, float agentHeight, float agentMaxSlope, float agentRadius, float agentMaxClimb);

		// �׺�Ž��� �����ϱ� ���� vertex�� index�� �������ִ� �Լ�(����)
		PARAGON_ENGINE_DLL void GetNavmeshRenderInfo(int index, std::vector<Pg::Math::PGFLOAT3>& vertices, std::vector<unsigned int>& indices);
		// ��Ű��(�׺�Ž� �迭) ������ ��������
		int GetPackageSize();
		// ���� ������Ʈ ������ ��ȯ
		void GetAgent(int index, float& agentHeight, float& agentMaxSlope, float& agentRadius, float& agentMaxClimb);
		// ���� �׺�Ž� ���� �� ����Ǿ� �ִ� �׺�Ž����� ������
		std::vector<std::string> GetNavimeshPathList();

		dtObstacleRef hitTestObstacle(const dtTileCache* tc, const float* sq);

		bool isectSegAABB(const float* sq, const float* amin, const float* amax, float& tmin, float& tmax);
	
	private:
		Pg::Math::PGFLOAT3 vertex(const float* pos);
		
		///Navgation�� ���� �ʿ��� ������.
		static const int PACKAGESIZE = 5;

		class NavGeom* _geom;
		PathFindbox _package[PACKAGESIZE];

		bool _keepInterResults;
		float _totalBuildTimeMs;

		unsigned char* _triareas;

		///���� Agent�� ���ؼ��� ����Ʈ ���·� �����ؾ� ���� ������?
		///����ȭ �� ����ؾ� ��
		std::vector<Agent*> _agentVec;


		//Recast�� �ʿ��� ������
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

		// solo_mesh -> temp+obstacle�� �ٲٸ鼭 �߰��� �κ�
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

