#include "NavigationSystem.h"
#include "SceneSystem.h"
#include "ParagonRecast.h"

#include "../ParagonData/PlaneCollider.h"
#include "../ParagonData/NavigationField.h"
#include "../ParagonData/NavMeshAgent.h"
#include "../ParagonMath/PgMath.h"
#include "../ParagonUtil/Log.h"

#include <singleton-cpp/singleton.h>
#include <functional>
#include <cassert>

namespace Pg::Engine
{
	NavigationSystem::NavigationSystem() :
		_navMesh(nullptr),
		_navMeshQuery(nullptr),
		_crowd(nullptr)
	{
		_rcContext = std::make_unique<rcContext>(rcContext());
		m_talloc = new LinearAllocator(32000);
		m_tcomp = new FastLZCompressor;
		m_tmproc = new MeshProcess;
	}

	NavigationSystem::~NavigationSystem()
	{
		if (_navMesh)
		{
			dtFreeNavMesh(_navMesh);
			_navMesh = nullptr;
		}
		if (_tileCache)
		{
			dtFreeTileCache(_tileCache);
			_tileCache = nullptr;
		}
		delete m_talloc;
		delete m_tcomp;
		delete m_tmproc;
	}

	void NavigationSystem::Initialize()
	{
		//Crowd와 NavMeshQuery의 구조체 초기화 및 할당
		_crowd = dtAllocCrowd();

		_navMeshQuery = dtAllocNavMeshQuery();

		//NavMesh 생성하고 빌드.
		SyncNavMesh();

		//에이전트 모두 생성.
		SyncAgents();
	}

	void NavigationSystem::Update(float deltaTime)
	{
		if (_crowd == nullptr)
		{
			PG_TRACE("Crowd 존재하지 않음.");
			return;
		}
		if (!_navMeshAgentVec.empty())
		{
			_crowd->update(deltaTime, nullptr);
			//_tileCache->update(deltaTime, _navMesh, nullptr);
		}

		for (auto& it : _navMeshAgentVec)
		{
			const dtCrowdAgent* crowdAgent = _crowd->getAgent(it->_agentidx);
			it->_object->_transform._position = { crowdAgent->npos[0], crowdAgent->npos[1], crowdAgent->npos[2] };
		}

		_crowd->update(deltaTime, nullptr);
		_tileCache->update(deltaTime, _navMesh, nullptr);
	}

	void NavigationSystem::Finalize()
	{
		//전부 해제.
		_navMeshFieldVec.clear();
		_navMeshAgentVec.clear();

		dtFreeCrowd(_crowd);

		dtFreeNavMeshQuery(_navMeshQuery);
	}

	void NavigationSystem::SyncAgents()
	{
		//원래 있던 NavMeshAgentVec();
		if (!_navMeshAgentVec.empty())
		{
			_navMeshAgentVec.clear();
		}

		//싱글턴
		auto& tSceneSystem = singleton<SceneSystem>();
		_sceneSystem = &tSceneSystem;

		for (auto& it : _sceneSystem->GetCurrentScene()->GetObjectList())
		{
			Pg::Data::NavMeshAgent* tNavMeshAgent = it->GetComponent<Pg::Data::NavMeshAgent>();

			if (tNavMeshAgent != nullptr)
			{
				//NavMeshAgent 컴포넌트 들어간 애들만 AddAgent 해줌.
				//Agent의 속성 부여
				dtCrowdAgentParams ap;
				memset(&ap, 0, sizeof(ap));

				ap.radius = tNavMeshAgent->GetRadius();
				ap.maxSpeed = tNavMeshAgent->GetMaxSpeed();
				ap.height = tNavMeshAgent->GetHeight();
				//ap.maxAcceleration = tNavMeshAgent->GetMaxAcceleration();
				ap.maxAcceleration = std::numeric_limits<float>::max();
				ap.collisionQueryRange = ap.radius * 12.0f;
				ap.pathOptimizationRange = ap.radius * 30.0f;
				ap.obstacleAvoidanceType = (unsigned char)3;

				ap.updateFlags = DT_CROWD_OPTIMIZE_TOPO |
					DT_CROWD_OPTIMIZE_VIS |
					DT_CROWD_OBSTACLE_AVOIDANCE;

				/*if (tNavMeshAgent->_anticipateTurns)
					ap.updateFlags |= DT_CROWD_ANTICIPATE_TURNS;

				if (tNavMeshAgent->_optimizeVisibility)
					ap.updateFlags |= DT_CROWD_OPTIMIZE_VIS;

				if (tNavMeshAgent->_optimizeTopology)
					ap.updateFlags |= DT_CROWD_OPTIMIZE_TOPO;

				if (tNavMeshAgent->_obstacleAvoidance)
					ap.updateFlags |= DT_CROWD_OBSTACLE_AVOIDANCE;

				if (tNavMeshAgent->_separation)
					ap.updateFlags |= DT_CROWD_SEPARATION;

				ap.obstacleAvoidanceType = static_cast<unsigned char>(tNavMeshAgent->_obstacleAvoidanceType);*/
				ap.separationWeight = tNavMeshAgent->_separationWeight;

				//agent의 현재 위치는 설정해놓은 포지션으로 정해놓는다.
				Pg::Math::PGFLOAT3 agentPos = tNavMeshAgent->_object->_transform._position;

				tNavMeshAgent->_agentidx = _crowd->addAgent(reinterpret_cast<const float*>(&agentPos), &ap);

				_navMeshAgentVec.push_back(tNavMeshAgent);

				///런타임에 설정값이 변경될 때 필요함.
				tNavMeshAgent->_updateSystemFunc = std::bind(&NavigationSystem::UpdateSingleDtParam, this, std::placeholders::_1);
				tNavMeshAgent->_destinationFunc = std::bind(&NavigationSystem::MoveTo, this, std::placeholders::_1, std::placeholders::_2);
				tNavMeshAgent->_relocateFunc = std::bind(&NavigationSystem::Relocate, this, std::placeholders::_1, std::placeholders::_2);
			}
		}
	}

	void NavigationSystem::RemoveAgent(int index)
	{
		_crowd->removeAgent(index);
	}

	void NavigationSystem::SyncNavMesh()
	{
		//원래 있던 NavMeshAgentVec();
		if (!_navMeshFieldVec.empty())
		{
			_navMeshFieldVec.clear();
		}
		if (!worldVertices.empty())
		{
			worldVertices.clear();
		}
		if (!worldIndices.empty())
		{
			worldIndices.clear();
		}

		//싱글턴
		auto& tSceneSystem = singleton<SceneSystem>();
		_sceneSystem = &tSceneSystem;

		for (auto& it : _sceneSystem->GetCurrentScene()->GetObjectList())
		{
			Pg::Data::NavigationField* tNavigationField = it->GetComponent<Pg::Data::NavigationField>();

			if (tNavigationField != nullptr)
			{
				Pg::Data::PlaneCollider* planeCollider = it->GetComponent<Pg::Data::PlaneCollider>();
				assert(planeCollider != nullptr);

				CreatePlaneNavMesh(planeCollider, worldVertices, worldIndices);

				_navMeshFieldVec.push_back(tNavigationField);
			}
		}

		BuildPlaneNavMesh(worldVertices, worldIndices);
	}

	void NavigationSystem::BuildPlaneNavMesh(const float* worldVertices, size_t verticesNum, const int* faces, size_t facesNum, const Pg::Data::BuildSettings& buildSettings)
	{
		static constexpr int EXPECTED_LAYERS_PER_TILE = 4;
		dtStatus status;

		//auto success = handleBuild(worldVertices, verticesNum, faces, facesNum, buildSettings);

		float bmin[3]{ std::numeric_limits<float>::max(),
						std::numeric_limits<float>::max(),std::numeric_limits<float>::max() };

		float bmax[3]{ -std::numeric_limits<float>::max(),
						-std::numeric_limits<float>::max(),-std::numeric_limits<float>::max() };

		// 바운더리 정보부터 설정
		for (auto i = 0; i < verticesNum; i++)
		{
			if (bmin[0] > worldVertices[i * 3])
				bmin[0] = worldVertices[i * 3];
			if (bmin[1] > worldVertices[i * 3 + 1])
				bmin[1] = worldVertices[i * 3 + 1];
			if (bmin[2] > worldVertices[i * 3 + 2])
				bmin[2] = worldVertices[i * 3 + 2];

			if (bmax[0] < worldVertices[i * 3])
				bmax[0] = worldVertices[i * 3];
			if (bmax[1] < worldVertices[i * 3 + 1])
				bmax[1] = worldVertices[i * 3 + 1];
			if (bmax[2] < worldVertices[i * 3 + 2])
				bmax[2] = worldVertices[i * 3 + 2];
		}

		//TileCache 초기화
		int gw = 0, gh = 0;
		rcCalcGridSize(bmin, bmax, buildSettings.divisionSizeXZ, &gw, &gh);
		const int ts = (int)buildSettings.tileSize;
		const int tw = (gw + ts - 1) / ts;
		const int th = (gh + ts - 1) / ts;

		memset(&_rcConfig, 0, sizeof(_rcConfig));

		_rcConfig.cs = buildSettings.divisionSizeXZ;
		_rcConfig.ch = buildSettings.divisionSizeY;
		_rcConfig.walkableSlopeAngle = buildSettings.walkableSlopeAngle;
		_rcConfig.walkableHeight = (int)ceilf(2.0 / _rcConfig.ch);
		_rcConfig.walkableClimb = (int)floorf(0.9 / _rcConfig.ch);
		_rcConfig.walkableRadius = (int)ceilf(buildSettings.agentRadius / _rcConfig.cs);
		_rcConfig.maxEdgeLen = (int)(12.0 / buildSettings.divisionSizeXZ);
		_rcConfig.maxSimplificationError = buildSettings.edgeMaxError;
		_rcConfig.minRegionArea = (int)rcSqr(8);		// Note: area = size*size
		_rcConfig.mergeRegionArea = (int)rcSqr(20);	// Note: area = size*size
		_rcConfig.maxVertsPerPoly = (int)6;
		_rcConfig.tileSize = (int)buildSettings.tileSize;
		_rcConfig.borderSize = _rcConfig.walkableRadius + 3; // Reserve enough padding.
		_rcConfig.width = _rcConfig.tileSize + _rcConfig.borderSize * 2;
		_rcConfig.height = _rcConfig.tileSize + _rcConfig.borderSize * 2;
		_rcConfig.detailSampleDist = 6.0 < 0.9f ? 0 : buildSettings.divisionSizeXZ * 6.0;
		_rcConfig.detailSampleMaxError = _rcConfig.ch * 1.0;
		rcVcopy(_rcConfig.bmin, bmin);
		rcVcopy(_rcConfig.bmax, bmax);
		rcCalcGridSize(_rcConfig.bmin, _rcConfig.bmax, _rcConfig.cs, &_rcConfig.width, &_rcConfig.height);

		// Tile cache params.
		dtTileCacheParams tcparams;
		memset(&tcparams, 0, sizeof(tcparams));
		rcVcopy(tcparams.orig, bmin);
		tcparams.cs = buildSettings.divisionSizeXZ;
		tcparams.ch = buildSettings.divisionSizeY;
		tcparams.width = (int)buildSettings.tileSize;
		tcparams.height = (int)buildSettings.tileSize;
		tcparams.walkableHeight = buildSettings.walkableHeight;
		tcparams.walkableRadius = buildSettings.agentRadius;
		tcparams.walkableClimb = buildSettings.walkableClimb;
		tcparams.maxSimplificationError = buildSettings.edgeMaxError;
		tcparams.maxTiles = tw * th * EXPECTED_LAYERS_PER_TILE;
		tcparams.maxObstacles = 512;

		dtFreeTileCache(_tileCache);

		_tileCache = dtAllocTileCache();
		if (!_tileCache)
		{
			_rcContext->log(RC_LOG_ERROR, "buildTiledNavigation: Could not allocate tile cache.");
		}
		status = _tileCache->init(&tcparams, m_talloc, m_tcomp, m_tmproc);
		if (dtStatusFailed(status))
		{
			_rcContext->log(RC_LOG_ERROR, "buildTiledNavigation: Could not init tile cache.");
		}
		
		bool processResult{ false };

		// 복셀 높이필드 공간 할당
		rcHeightfield* heightField{ rcAllocHeightfield() };
		assert(heightField != nullptr);

		processResult = rcCreateHeightfield(_rcContext.get(), *heightField, _rcConfig.width, _rcConfig.height, _rcConfig.bmin, _rcConfig.bmax, _rcConfig.cs, _rcConfig.ch);
		assert(processResult == true);

		std::vector<unsigned char> triareas;
		triareas.resize(facesNum);

		//unsigned char* triareas = new unsigned char[facesNum];
		//memset(triareas, 0, facesNum * sizeof(unsigned char));

		rcMarkWalkableTriangles(_rcContext.get(), _rcConfig.walkableSlopeAngle, worldVertices, verticesNum, faces, facesNum, triareas.data());
		processResult = rcRasterizeTriangles(_rcContext.get(), worldVertices, verticesNum, faces, triareas.data(), facesNum, *heightField, _rcConfig.walkableClimb);
		assert(processResult == true);

		// 필요없는 부분 필터링
		rcFilterLowHangingWalkableObstacles(_rcContext.get(), _rcConfig.walkableClimb, *heightField);
		rcFilterLedgeSpans(_rcContext.get(), _rcConfig.walkableHeight, _rcConfig.walkableClimb, *heightField);
		rcFilterWalkableLowHeightSpans(_rcContext.get(), _rcConfig.walkableHeight, *heightField);

		// 밀집 높이 필드 만들기
		rcCompactHeightfield* compactHeightField{ rcAllocCompactHeightfield() };
		assert(compactHeightField != nullptr);

		processResult = rcBuildCompactHeightfield(_rcContext.get(), _rcConfig.walkableHeight, _rcConfig.walkableClimb, *heightField, *compactHeightField);
		//rcFreeHeightField(heightField);
		assert(processResult == true);

		//processResult = rcErodeWalkableArea(_rcContext, _rcConfig.walkableRadius, *compactHeightField);
		//assert(processResult == true);

		processResult = rcBuildDistanceField(_rcContext.get(), *compactHeightField);
		assert(processResult == true);

		rcBuildRegions(_rcContext.get(), *compactHeightField, 0, _rcConfig.minRegionArea, _rcConfig.mergeRegionArea);
		assert(processResult == true);

		// 윤곽선 만들기
		rcContourSet* contourSet{ rcAllocContourSet() };
		assert(contourSet != nullptr);

		processResult = rcBuildContours(_rcContext.get(), *compactHeightField, _rcConfig.maxSimplificationError, _rcConfig.maxEdgeLen, *contourSet);
		assert(processResult == true);

		// 윤곽선으로부터 폴리곤 생성
		_polyMesh = rcAllocPolyMesh();
		assert(_polyMesh != nullptr);

		processResult = rcBuildPolyMesh(_rcContext.get(), *contourSet, _rcConfig.maxVertsPerPoly, *_polyMesh);
		assert(processResult == true);

		// 디테일 메시 생성
		auto& detailMesh{ _polyMeshDetail = rcAllocPolyMeshDetail() };
		assert(detailMesh != nullptr);

		processResult = rcBuildPolyMeshDetail(_rcContext.get(), *_polyMesh, *compactHeightField, _rcConfig.detailSampleDist, _rcConfig.detailSampleMaxError, *detailMesh);
		assert(processResult == true);

		//rcFreeCompactHeightfield(compactHeightField);
		//rcFreeContourSet(contourSet);

		 // detour 데이터 생성
		unsigned char* navData{ nullptr };
		int navDataSize{ 0 };

		assert(_rcConfig.maxVertsPerPoly <= DT_VERTS_PER_POLYGON);

		// Update poly flags from areas.
		for (int i = 0; i < _polyMesh->npolys; ++i)
		{
			if (_polyMesh->areas[i] == RC_WALKABLE_AREA)
			{
				_polyMesh->areas[i] = 0;
				_polyMesh->flags[i] = 1;
			}
		}

		dtFreeNavMesh(_navMesh);

		_navMesh = dtAllocNavMesh();
		if (!_navMesh)
		{
			_rcContext->log(RC_LOG_ERROR, "buildTiledNavigation: Could not allocate navmesh.");
		}

		//NavMesh 설정
		dtNavMeshCreateParams params;
		memset(&params, 0, sizeof(params));

		params.verts = _polyMesh->verts;
		params.vertCount = _polyMesh->nverts;
		params.polys = _polyMesh->polys;
		params.polyAreas = _polyMesh->areas;
		params.polyFlags = _polyMesh->flags;
		params.polyCount = _polyMesh->npolys;
		params.nvp = _polyMesh->nvp;
		params.detailMeshes = detailMesh->meshes;
		params.detailVerts = detailMesh->verts;
		params.detailVertsCount = detailMesh->nverts;
		params.detailTris = detailMesh->tris;
		params.detailTriCount = detailMesh->ntris;
		params.offMeshConVerts = 0;
		params.offMeshConRad = 0;
		params.offMeshConDir = 0;
		params.offMeshConAreas = 0;
		params.offMeshConFlags = 0;
		params.offMeshConUserID = 0;
		params.offMeshConCount = 0;
		params.walkableHeight = _rcConfig.walkableHeight;
		params.walkableRadius = _rcConfig.walkableRadius;
		params.walkableClimb = _rcConfig.walkableClimb;
		rcVcopy(params.bmin, _polyMesh->bmin);
		rcVcopy(params.bmax, _polyMesh->bmax);
		params.cs = _rcConfig.cs;
		params.ch = _rcConfig.ch;
		params.buildBvTree = true;

		processResult = dtCreateNavMeshData(&params, &navData, &navDataSize);
		assert(processResult == true);

		_navMesh = dtAllocNavMesh();

		status = _navMesh->init(navData, navDataSize, DT_TILE_FREE_DATA);
		dtFree(navData);
		assert(dtStatusFailed(status) == false);

		status = _navMeshQuery->init(_navMesh, 2048);
		assert(dtStatusFailed(status) == false);

		_rcContext->resetTimers();

		m_cacheLayerCount = 0;
		m_cacheCompressedSize = 0;
		m_cacheRawSize = 0;

		for (int y = 0; y < th; ++y)
		{
			for (int x = 0; x < tw; ++x)
			{
				TileCacheData tiles[MAX_LAYERS];
				memset(tiles, 0, sizeof(tiles));
				int ntiles = rasterizeTileLayers(worldVertices, verticesNum, faces, facesNum, x, y, _rcConfig, tiles, MAX_LAYERS);

				for (int i = 0; i < ntiles; ++i)
				{
					TileCacheData* tile = &tiles[i];
					status = _tileCache->addTile(tile->data, tile->dataSize, DT_COMPRESSEDTILE_FREE_DATA, 0);
					if (dtStatusFailed(status))
					{
						dtFree(tile->data);
						tile->data = 0;
						continue;
					}

					m_cacheLayerCount++;
					m_cacheCompressedSize += tile->dataSize;
					m_cacheRawSize += calcLayerBufferSize(tcparams.width, tcparams.height);
				}
			}
		}

		// Build initial meshes
		_rcContext->startTimer(RC_TIMER_TOTAL);
		for (int y = 0; y < th; ++y)
			for (int x = 0; x < tw; ++x)
				_tileCache->buildNavMeshTilesAt(x, y, _navMesh);
		_rcContext->stopTimer(RC_TIMER_TOTAL);

		m_cacheBuildTimeMs = _rcContext->getAccumulatedTime(RC_TIMER_TOTAL) / 1000.0f;
		m_cacheBuildMemUsage = static_cast<unsigned int>(m_talloc->high);


		const dtNavMesh* nav = _navMesh;
		int navmeshMemUsage = 0;
		for (int i = 0; i < nav->getMaxTiles(); ++i)
		{
			const dtMeshTile* tile = nav->getTile(i);
			if (tile->header)
				navmeshMemUsage += tile->dataSize;
		}

		_crowd->init(1024, buildSettings.maxAgentRadius, _navMesh);
	}


	void NavigationSystem::CreatePlaneNavMesh(Pg::Data::PlaneCollider* planeCollider, std::vector<Pg::Math::PGFLOAT3>& worldVertices, std::vector<int>& worldIndices)
	{
		// PlaneCollider의 위치, 너비, 높이 및 회전 정보를 가져옵니다.
		Pg::Math::PGFLOAT3 planePos = planeCollider->_object->_transform._position;

		float width = planeCollider->GetWidth();
		float depth = planeCollider->GetDepth();

		Pg::Math::PGQuaternion rotationAxis = planeCollider->_object->_transform._rotation; // 회전 축
		DirectX::XMFLOAT4 tPre = { rotationAxis.x,rotationAxis.y, rotationAxis.z, rotationAxis.w };
		DirectX::XMVECTOR quaternion = DirectX::XMLoadFloat4(&tPre);

		DirectX::XMFLOAT3 botleft = { -width / 2.0f, 0.0f, -depth / 2.0f };
		DirectX::XMFLOAT3 topleft = { -width / 2.0f, 0.0f, depth / 2.0f };
		DirectX::XMFLOAT3 botright = { width / 2.0f, 0.0f, -depth / 2.0f };
		DirectX::XMFLOAT3 topright = { width / 2.0f, 0.0f, depth / 2.0f };

		//DirectX::XMFLOAT3 botleft{ -10.f, 0.f, -10.f };
		//DirectX::XMFLOAT3 topright{ 10.f, 0.f, 10.f };

		int startingIdx = worldVertices.size();
		worldVertices.push_back({ botleft.x,0,topright.z });
		worldVertices.push_back({ botleft.x,0,botleft.z });
		worldVertices.push_back({ topright.x,0,botleft.z });
		worldVertices.push_back({ topright.x,0,topright.z });

		worldIndices.push_back(startingIdx + 2);
		worldIndices.push_back(startingIdx + 1);
		worldIndices.push_back(startingIdx + 0);
		worldIndices.push_back(startingIdx + 3);
		worldIndices.push_back(startingIdx + 2);
		worldIndices.push_back(startingIdx + 0);

		// 회전을 적용합니다.
		//DirectX::XMVECTOR botleftRotated = DirectX::XMVector3Rotate(DirectX::XMLoadFloat3(&botleft), quaternion);
		//DirectX::XMVECTOR botrightRotated = DirectX::XMVector3Rotate(DirectX::XMLoadFloat3(&botright), quaternion);
		//DirectX::XMVECTOR topleftRotated = DirectX::XMVector3Rotate(DirectX::XMLoadFloat3(&topleft), quaternion);
		//DirectX::XMVECTOR toprightRotated = DirectX::XMVector3Rotate(DirectX::XMLoadFloat3(&topright), quaternion);
		//
		//// DirectX 좌표계에 맞게 좌표를 조정합니다.
		//DirectX::XMFLOAT3 result;
		//DirectX::XMStoreFloat3(&result, botleftRotated);
		//botleft.x = result.x + planePos.x;
		//botleft.y = result.y + planePos.y;
		//botleft.z = result.z + planePos.z;
		//
		//DirectX::XMStoreFloat3(&result, botrightRotated);
		//botright.x = result.x + planePos.x;
		//botright.y = result.y + planePos.y;
		//botright.z = result.z + planePos.z;
		//
		//DirectX::XMStoreFloat3(&result, topleftRotated);
		//topleft.x = result.x + planePos.x;
		//topleft.y = result.y + planePos.y;
		//topleft.z = result.z + planePos.z;
		//
		//DirectX::XMStoreFloat3(&result, toprightRotated);
		//topright.x = result.x + planePos.x;
		//topright.y = result.y + planePos.y;
		//topright.z = result.z + planePos.z;

		// 꼭짓점을 worldVertices에 추가합니다.
		//int startingIdx = worldVertices.size();
		//worldVertices.push_back(Pg::Math::XM2PG_FLOAT3(botleft));
		//worldVertices.push_back(Pg::Math::XM2PG_FLOAT3(botright));
		//worldVertices.push_back(Pg::Math::XM2PG_FLOAT3(topleft));
		//worldVertices.push_back(Pg::Math::XM2PG_FLOAT3(topright));
		//
		//// 인덱스를 worldIndices에 추가합니다.
		//worldIndices.push_back(startingIdx + 2);
		//worldIndices.push_back(startingIdx + 1);
		//worldIndices.push_back(startingIdx + 0);
		//worldIndices.push_back(startingIdx + 3);
		//worldIndices.push_back(startingIdx + 2);
		//worldIndices.push_back(startingIdx + 0);
	}

	void NavigationSystem::MoveTo(Pg::Data::NavMeshAgent* agent, Pg::Math::PGFLOAT3 des)
	{
		if (_navMeshFieldVec.empty())
		{
			return;
		}

		_filter = _crowd->getFilter(0);
		agent->_crowdAgent = _crowd->getAgent(agent->_agentidx);
		//_halfExtents = _crowd->getQueryExtents();
		static constexpr float halfExtents[]{ 100,100,100 };

		_navMeshQuery->findNearestPoly(reinterpret_cast<const float*>(&des), halfExtents,
					_filter, &(agent->_targetRef), agent->_targetPos);

		_crowd->requestMoveTarget(agent->_agentidx, agent->_targetRef, agent->_targetPos);

		//const int maxAttempts = 10;
		//
		//for (int attempt = 0; attempt < maxAttempts; ++attempt)
		//{
		//	dtStatus status = _navMeshQuery->findNearestPoly(reinterpret_cast<const float*>(&des), halfExtents,
		//		_filter, &(agent->_targetRef), agent->_targetPos);

		//	if (status == DT_SUCCESS && agent->_targetRef != 0)
		//	{
		//		// 유효한 다각형을 찾은 경우 이동 명령을 요청하고 함수를 종료합니다.
		//		_crowd->requestMoveTarget(agent->_agentidx, agent->_targetRef, agent->_targetPos);
		//		//return;
		//	}
		//	else
		//	{
		//		des.x += 1.0f;
		//		des.z += 1.0f;
		//	}
		//}

	}

	void NavigationSystem::Relocate(Pg::Data::NavMeshAgent* agent, Pg::Math::PGFLOAT3 des)
	{
		const float* pos = &des.x;
		constexpr float agentPlacementHalfExtents[3]{ 1000,1000,1000 };
		float nearest[3];
		dtPolyRef ref = 0;
		dtVcopy(nearest, pos);

		dtStatus status = _navMeshQuery->findNearestPoly(pos, agentPlacementHalfExtents, _crowd->getFilter(0), &ref, nearest);
		if (dtStatusFailed(status))
		{
			dtVcopy(nearest, pos);
			ref = 0;
		}
		dtCrowdAgent* ag = _crowd->getEditableAgent(agent->_agentidx);
		ag->corridor.reset(ref, nearest);
		ag->boundary.reset();
		ag->partial = false;

		ag->topologyOptTime = 0;
		ag->targetReplanTime = 0;
		ag->nneis = 0;

		dtVset(ag->dvel, 0, 0, 0);
		dtVset(ag->nvel, 0, 0, 0);
		dtVset(ag->vel, 0, 0, 0);
		dtVcopy(ag->npos, nearest);

		ag->desiredSpeed = 0;

		if (ref)
			ag->state = DT_CROWDAGENT_STATE_WALKING;
		else
			ag->state = DT_CROWDAGENT_STATE_INVALID;

		ag->targetState = DT_CROWDAGENT_TARGET_NONE;
	}

	int NavigationSystem::rasterizeTileLayers(const float* worldVertices, size_t verticesNum, const int* faces, size_t facesNum, const int tx, const int ty, const rcConfig& cfg, struct TileCacheData* tiles, const int maxTiles)
	{
		FastLZCompressor comp;
		RasterizationContext rc;

		const float* verts = worldVertices;
		const int nverts = verticesNum;
		rcChunkyTriMesh chunkyMesh;
		rcCreateChunkyTriMesh(verts, faces, facesNum, 256, &chunkyMesh);

		//// Tile bounds.
		const float tcs = cfg.tileSize * cfg.cs;

		rcConfig tcfg;
		memcpy(&tcfg, &cfg, sizeof(tcfg));

		tcfg.bmin[0] = cfg.bmin[0] + tx * tcs;
		tcfg.bmin[1] = cfg.bmin[1];
		tcfg.bmin[2] = cfg.bmin[2] + ty * tcs;
		tcfg.bmax[0] = cfg.bmin[0] + (tx + 1) * tcs;
		tcfg.bmax[1] = cfg.bmax[1];
		tcfg.bmax[2] = cfg.bmin[2] + (ty + 1) * tcs;
		tcfg.bmin[0] -= tcfg.borderSize * tcfg.cs;
		tcfg.bmin[2] -= tcfg.borderSize * tcfg.cs;
		tcfg.bmax[0] += tcfg.borderSize * tcfg.cs;
		tcfg.bmax[2] += tcfg.borderSize * tcfg.cs;

		// Allocate voxel heightfield where we rasterize our input data to.
		rc.solid = rcAllocHeightfield();
		if (!rc.solid)
		{
			_rcContext->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'solid'.");
			return 0;
		}
		if (!rcCreateHeightfield(_rcContext.get(), *rc.solid, tcfg.width, tcfg.height, tcfg.bmin, tcfg.bmax, tcfg.cs, tcfg.ch))
		{
			_rcContext->log(RC_LOG_ERROR, "buildNavigation: Could not create solid heightfield.");
			return 0;
		}

		// Allocate array that can hold triangle flags.
		// If you have multiple meshes you need to process, allocate
		// and array which can hold the max number of triangles you need to process.
		rc.triareas = new unsigned char[chunkyMesh.maxTrisPerChunk];
		if (!rc.triareas)
		{
			_rcContext->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'm_triareas' (%d).", chunkyMesh.maxTrisPerChunk);
			return 0;
		}

		float tbmin[2], tbmax[2];
		tbmin[0] = tcfg.bmin[0];
		tbmin[1] = tcfg.bmin[2];
		tbmax[0] = tcfg.bmax[0];
		tbmax[1] = tcfg.bmax[2];
		
		int cid[512];// TODO: Make grow when returning too many items.
		const int ncid = rcGetChunksOverlappingRect(&chunkyMesh, tbmin, tbmax, cid, 512);
		if (!ncid)
		{
			return 0; // empty
		}

		for (int i = 0; i < ncid; ++i)
		{
			const rcChunkyTriMeshNode& node = chunkyMesh.nodes[cid[i]];
			const int* tris = &chunkyMesh.tris[node.i * 3];
			const int ntris = node.n;

			memset(rc.triareas, 0, ntris * sizeof(unsigned char));
			rcMarkWalkableTriangles(_rcContext.get(), tcfg.walkableSlopeAngle,
				verts, nverts, tris, ntris, rc.triareas);

			if (!rcRasterizeTriangles(_rcContext.get(), verts, nverts, tris, rc.triareas, ntris, *rc.solid, tcfg.walkableClimb))
				return 0;
		}

		// Once all geometry is rasterized, we do initial pass of filtering to
		// remove unwanted overhangs caused by the conservative rasterization
		// as well as filter spans where the character cannot possibly stand.
		if (m_filterLowHangingObstacles)
			rcFilterLowHangingWalkableObstacles(_rcContext.get(), tcfg.walkableClimb, *rc.solid);
		if (m_filterLedgeSpans)
			rcFilterLedgeSpans(_rcContext.get(), tcfg.walkableHeight, tcfg.walkableClimb, *rc.solid);
		if (m_filterWalkableLowHeightSpans)
			rcFilterWalkableLowHeightSpans(_rcContext.get(), tcfg.walkableHeight, *rc.solid);


		rc.chf = rcAllocCompactHeightfield();
		if (!rc.chf)
		{
			_rcContext->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'chf'.");
			return 0;
		}
		if (!rcBuildCompactHeightfield(_rcContext.get(), tcfg.walkableHeight, tcfg.walkableClimb, *rc.solid, *rc.chf))
		{
			_rcContext->log(RC_LOG_ERROR, "buildNavigation: Could not build compact data.");
			return 0;
		}

		// Erode the walkable area by agent radius.
		if (!rcErodeWalkableArea(_rcContext.get(), tcfg.walkableRadius, *rc.chf))
		{
			_rcContext->log(RC_LOG_ERROR, "buildNavigation: Could not erode.");
			return 0;
		}

		rc.lset = rcAllocHeightfieldLayerSet();
		if (!rc.lset)
		{
			_rcContext->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'lset'.");
			return 0;
		}
		if (!rcBuildHeightfieldLayers(_rcContext.get(), *rc.chf, tcfg.borderSize, tcfg.walkableHeight, *rc.lset))
		{
			_rcContext->log(RC_LOG_ERROR, "buildNavigation: Could not build heighfield layers.");
			return 0;
		}

		rc.ntiles = 0;
		for (int i = 0; i < rcMin(rc.lset->nlayers, MAX_LAYERS); ++i)
		{
			TileCacheData* tile = &rc.tiles[rc.ntiles++];
			const rcHeightfieldLayer* layer = &rc.lset->layers[i];

			// Store header
			dtTileCacheLayerHeader header;
			header.magic = DT_TILECACHE_MAGIC;
			header.version = DT_TILECACHE_VERSION;

			// Tile layer location in the navmesh.
			header.tx = tx;
			header.ty = ty;
			header.tlayer = i;
			dtVcopy(header.bmin, layer->bmin);
			dtVcopy(header.bmax, layer->bmax);

			// Tile info.
			header.width = (unsigned char)layer->width;
			header.height = (unsigned char)layer->height;
			header.minx = (unsigned char)layer->minx;
			header.maxx = (unsigned char)layer->maxx;
			header.miny = (unsigned char)layer->miny;
			header.maxy = (unsigned char)layer->maxy;
			header.hmin = (unsigned short)layer->hmin;
			header.hmax = (unsigned short)layer->hmax;

			dtStatus status = dtBuildTileCacheLayer(&comp, &header, layer->heights, layer->areas, layer->cons,
				&tile->data, &tile->dataSize);
			if (dtStatusFailed(status))
			{
				return 0;
			}
		}

		// Transfer ownsership of tile data from build context to the caller.
		int n = 0;
		for (int i = 0; i < rcMin(rc.ntiles, maxTiles); ++i)
		{
			tiles[n++] = rc.tiles[i];
			rc.tiles[i].data = 0;
			rc.tiles[i].dataSize = 0;
		}

		return n;
	}

	int NavigationSystem::calcLayerBufferSize(const int gridWidth, const int gridHeight)
	{
		const int headerSize = dtAlign4(sizeof(dtTileCacheLayerHeader));
		const int gridSize = gridWidth * gridHeight;
		return headerSize + gridSize * 4;
	}

	dtNavMesh* NavigationSystem::GetNavMesh() const
	{
		return _navMesh;
	}

	dtNavMeshQuery* NavigationSystem::GetNavMeshQuery() const
	{
		return _navMeshQuery;
	}

	void NavigationSystem::UpdateSingleDtParam(const Pg::Data::NavMeshAgent* navAgent)
	{
		//_crowd->updateAgentParameters()
	}
}