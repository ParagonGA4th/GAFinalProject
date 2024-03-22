#include "NavigationSystem.h"
#include "SceneSystem.h"

#include "../ParagonData/PlaneCollider.h"
#include "../ParagonData/NavigationField.h"
#include "../ParagonData/NavMeshAgent.h"
#include "../ParagonMath/PgMath.h"
#include "../ParagonUtil/Log.h"

#include <singleton-cpp/singleton.h>
#include <functional>

namespace Pg::Engine
{
	NavigationSystem::NavigationSystem() :
		_navMesh(nullptr),
		_navMeshQuery(nullptr)
	{

	}

	void NavigationSystem::Initialize()
	{
		//Crowd와 NavMeshQuery의 구조체 초기화 및 할당
		_crowd = dtAllocCrowd();

		_navMeshQuery = dtAllocNavMeshQuery();

		//에이전트 모두 생성.
		SyncAgents();
	}

	void NavigationSystem::Update(float deltaTime)
	{
		if (_crowd == nullptr || _navMesh == nullptr)
		{
			//PG_TRACE("Crowd 존재하지 않음.");
			return;
		}

		_crowd->update(deltaTime, nullptr);
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
				ap.maxAcceleration = tNavMeshAgent->GetMaxAcceleration();
				ap.collisionQueryRange = ap.radius * 12.0f;
				ap.pathOptimizationRange = ap.radius * 30.0f;

				ap.updateFlags = 0;

				if (tNavMeshAgent->_anticipateTurns)
					ap.updateFlags |= DT_CROWD_ANTICIPATE_TURNS;

				if (tNavMeshAgent->_optimizeVisibility)
					ap.updateFlags |= DT_CROWD_OPTIMIZE_VIS;

				if (tNavMeshAgent->_optimizeTopology)
					ap.updateFlags |= DT_CROWD_OPTIMIZE_TOPO;

				if (tNavMeshAgent->_obstacleAvoidance)
					ap.updateFlags |= DT_CROWD_OBSTACLE_AVOIDANCE;

				if (tNavMeshAgent->_separation)
					ap.updateFlags |= DT_CROWD_SEPARATION;

				ap.obstacleAvoidanceType = static_cast<unsigned char>(tNavMeshAgent->_obstacleAvoidanceType);
				ap.separationWeight = tNavMeshAgent->_separationWeight;

				//agent의 현재 위치는 설정해놓은 포지션으로 정해놓는다.
				Pg::Math::PGFLOAT3 agentPos = tNavMeshAgent->_object->_transform._position;

				_crowd->addAgent(reinterpret_cast<const float*>(&agentPos), &ap);

				_navMeshAgentVec.push_back(tNavMeshAgent);
				
				///런타임에 설정값이 변경될 때 필요함.
				//tNavMeshAgent->_updateSystemFunc = std::bind(&NavigationSystem::UpdateSingleDtParam, this, std::placeholders::_1);
			}

		}
	}

	void NavigationSystem::RemoveAgent(int index)
	{
		_crowd->removeAgent(index);
	}

	void NavigationSystem::CreatePlaneNavMesh()
	{
		float bmin[3]{ std::numeric_limits<float>::max(),
						std::numeric_limits<float>::max(),std::numeric_limits<float>::max() };

		float bmax[3]{ -std::numeric_limits<float>::max(),
						-std::numeric_limits<float>::max(),-std::numeric_limits<float>::max() };
		
		// 바운더리 정보부터 설정
		memset(&_rcConfig, 0, sizeof(_rcConfig));

		_rcConfig.cs = 0.3f; // 셀 사이즈
		_rcConfig.ch = 0.2f; // 셀 높이
		_rcConfig.walkableSlopeAngle = 45.0f;	//경사
		_rcConfig.walkableHeight = 2.0f;
		_rcConfig.walkableClimb = 0.9f;
		_rcConfig.walkableRadius = 0.6f;
		_rcConfig.maxEdgeLen = 12.0f;
		_rcConfig.maxSimplificationError = 1.3f;
		_rcConfig.minRegionArea = 8.0f;
		_rcConfig.mergeRegionArea = 20.0f;
		_rcConfig.maxVertsPerPoly = 6;
		_rcConfig.detailSampleDist = 6.0f;
		_rcConfig.detailSampleMaxError = _rcConfig.ch * 1.0f;

		rcVcopy(_rcConfig.bmin, bmin);
		rcVcopy(_rcConfig.bmax, bmax);
		
		bool processResult{ false };
	
		// 복셀 높이필드 공간 할당
		rcHeightfield* heightField{ rcAllocHeightfield() };
		assert(heightField != nullptr);


		processResult = rcCreateHeightfield(_rcContext, *heightField, _rcConfig.width, _rcConfig.height, _rcConfig.bmin, _rcConfig.bmax, _rcConfig.cs, _rcConfig.ch);
		assert(processResult == true);

		std::vector<unsigned char> triareas;
		//triareas.resize(facesNum);
		//unsigned char * triareas = new unsigned char[facesNum];
		//memset(triareas, 0, facesNum*sizeof(unsigned char));

		//rcMarkWalkableTriangles(_rcContext, _rcConfig.walkableSlopeAngle, worldVertices, verticesNum, faces, facesNum, triareas.data());
		//processResult = rcRasterizeTriangles(_rcContext, worldVertices, verticesNum, faces, triareas.data(), facesNum, *heightField, _rcConfig.walkableClimb);
		//assert(processResult == true);

		// 필요없는 부분 필터링
		rcFilterLowHangingWalkableObstacles(_rcContext, _rcConfig.walkableClimb, *heightField);
		rcFilterLedgeSpans(_rcContext, _rcConfig.walkableHeight, _rcConfig.walkableClimb, *heightField);
		rcFilterWalkableLowHeightSpans(_rcContext, _rcConfig.walkableHeight, *heightField);

		// 밀집 높이 필드 만들기
		rcCompactHeightfield* compactHeightField{ rcAllocCompactHeightfield() };
		assert(compactHeightField != nullptr);

		processResult = rcBuildCompactHeightfield(_rcContext, _rcConfig.walkableHeight, _rcConfig.walkableClimb, *heightField, *compactHeightField);
		//rcFreeHeightField(heightField);
		assert(processResult == true);

		//processResult = rcErodeWalkableArea(context, config.walkableRadius, *compactHeightField);
		//assert(processResult == true);

		processResult = rcBuildDistanceField(_rcContext, *compactHeightField);
		assert(processResult == true);

		rcBuildRegions(_rcContext, *compactHeightField, 0, _rcConfig.minRegionArea, _rcConfig.mergeRegionArea);
		assert(processResult == true);

		// 윤곽선 만들기
		rcContourSet* contourSet{ rcAllocContourSet() };
		assert(contourSet != nullptr);

		processResult = rcBuildContours(_rcContext, *compactHeightField, _rcConfig.maxSimplificationError, _rcConfig.maxEdgeLen, *contourSet);
		assert(processResult == true);

		// 윤곽선으로부터 폴리곤 생성
		//_p = rcAllocPolyMesh();
		//assert(polyMesh != nullptr);
		//
		//processResult = rcBuildPolyMesh(_rcContext, *contourSet, _rcConfig.maxVertsPerPoly, *polyMesh);
		//assert(processResult == true);
		//
		// 디테일 메시 생성
		auto& detailMesh{ _polyMeshDetail = rcAllocPolyMeshDetail() };
		assert(detailMesh != nullptr);
		//
		//processResult = rcBuildPolyMeshDetail(_rcContext, *polyMesh, *compactHeightField, _rcConfig.detailSampleDist, _rcConfig.detailSampleMaxError, *detailMesh);
		//assert(processResult == true);

		//rcFreeCompactHeightfield(compactHeightField);
		//rcFreeContourSet(contourSet);


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

		//processResult = dtCreateNavMeshData(&params, &navData, &navDataSize);
		//assert(processResult == true);

		//dtStatus status;
		//status = _navMesh->init(navData, navDataSize, DT_TILE_FREE_DATA);
		////dtFree(navData);
		//assert(dtStatusFailed(status) == false);

		//status = _navMeshQuery->init(navMesh, 2048);
		//assert(dtStatusFailed(status) == false);

		//_crowd->init(1024, buildSettings.maxAgentRadius, _navMesh);
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