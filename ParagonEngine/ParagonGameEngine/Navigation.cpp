#include "Navigation.h"
#include "ParagonRecast.h"

#include <singleton-cpp/singleton.h>
#include <functional>
#include <cassert>
namespace Pg::Engine
{
	static int calcLayerBufferSize(const int gridWidth, const int gridHeight)
	{
		const int headerSize = dtAlign4(sizeof(dtTileCacheLayerHeader));
		const int gridSize = gridWidth * gridHeight;
		return headerSize + gridSize * 4;
	}

	Navigation::Navigation() :
		_keepInterResults(false), _totalBuildTimeMs(0.0f)
		// 빌드정보
		, _triareas(nullptr), _solid(nullptr), _chf(nullptr), _cset(nullptr), _pmesh(nullptr)
		, _cfg(), _dmesh(nullptr), _ctx(nullptr)
		//
		, _navMeshDrawFlags('\x3')
		// 빌드세팅
		, _cellSize(0.3), _cellHeight(0.2), _agentHeight(2.0), _agentRadius(0.6), _agentMaxClimb(0.9)
		, _agentMaxSlope(45.0), _regionMinSize(8.0), _regionMergeSize(20.0), _edgeMaxLen(12.0), _edgeMaxError(1.3)
		, _vertsPerPoly(6.0), _detailSampleDist(6.0), _detailSampleMaxError(1), _partitionType(0)
		// 필터 정보
		, _filterLowHangingObstacles(false), _filterLedgeSpans(false), _filterWalkableLowHeightSpans(false)
		// Obstacle에서 추가 된 부분
		, _maxTiles(1024), _maxPolysPerTile(4096)
	{
		_ctx = new rcContext();
		_talloc = new LinearAllocator(32000);
		_tcomp = new FastLZCompressor();
		_tmproc = new MeshProcess();
	}

	Navigation::~Navigation()
	{

	}

	void Navigation::Initialize()
	{

	}

	void Navigation::Release()
	{

	}

	void Navigation::LoadAll(const char* path, int index)
	{
		FILE* fp = fopen(path, "rb");
		if (!fp) return;

		// Read header.
		TileCacheSetHeader header;
		size_t headerReadReturnCode = fread(&header, sizeof(TileCacheSetHeader), 1, fp);
		//if (headerReadReturnCode != 1)
		//{
		//	// Error or early EOF
		//	fclose(fp);
		//	return;
		//}
		//if (header.magic != TILECACHESET_MAGIC)
		//{
		//	fclose(fp);
		//	return;
		//}
		//if (header.version != TILECACHESET_VERSION)
		//{
		//	fclose(fp);
		//	return;
		//}

		_package[index]._navMesh = dtAllocNavMesh();
		if (!_package[index]._navMesh)
		{
			fclose(fp);
			return;
		}
		dtStatus status = _package[index]._navMesh->init(&header.meshParams);
		if (dtStatusFailed(status))
		{
			fclose(fp);
			return;
		}

		_package[index]._tileCache = dtAllocTileCache();
		if (!_package[index]._tileCache)
		{
			fclose(fp);
			return;
		}
		status = _package[index]._tileCache->init(&header.cacheParams, _talloc, _tcomp, _tmproc);
		if (dtStatusFailed(status))
		{
			fclose(fp);
			return;
		}

		// Read tiles.
		for (int i = 0; i < header.numTiles; ++i)
		{
			TileCacheTileHeader tileHeader;
			size_t tileHeaderReadReturnCode = fread(&tileHeader, sizeof(tileHeader), 1, fp);
			if (tileHeaderReadReturnCode != 1)
			{
				// Error or early EOF
				fclose(fp);
				return;
			}
			if (!tileHeader.tileRef || !tileHeader.dataSize)
				break;

			unsigned char* data = (unsigned char*)dtAlloc(tileHeader.dataSize, DT_ALLOC_PERM);
			if (!data) break;
			memset(data, 0, tileHeader.dataSize);
			size_t tileDataReadReturnCode = fread(data, tileHeader.dataSize, 1, fp);
			if (tileDataReadReturnCode != 1)
			{
				// Error or early EOF
				dtFree(data);
				fclose(fp);
				return;
			}

			dtCompressedTileRef tile = 0;
			dtStatus addTileStatus = _package[index]._tileCache->addTile(data, tileHeader.dataSize, DT_COMPRESSEDTILE_FREE_DATA, &tile);
			if (dtStatusFailed(addTileStatus))
			{
				dtFree(data);
			}

			if (tile)
				_package[index]._tileCache->buildNavMeshTile(tile, _package[index]._navMesh);
		}

		fclose(fp);

		_package[index]._navQuery->init(_package[index]._navMesh, 2048);
		return;
	}

	bool Navigation::HandleBuild(int index)
	{
		dtStatus status;

		if (!_geom || !_geom->getMesh())
		{
			_ctx->log(RC_LOG_ERROR, "buildTiledNavigation: No vertices and triangles.");
			return false;
		}

		//_tmproc->init(_geom);

		// Init cache
		const float* bmin = _geom->getNavMeshBoundsMin();
		const float* bmax = _geom->getNavMeshBoundsMax();
		int gw = 0, gh = 0;
		rcCalcGridSize(bmin, bmax, _cellSize, &gw, &gh);
		const int ts = (int)_tileSize;
		const int tw = (gw + ts - 1) / ts;
		const int th = (gh + ts - 1) / ts;

		// Generation params.
		rcConfig cfg;
		memset(&cfg, 0, sizeof(cfg));
		cfg.cs = _cellSize;
		cfg.ch = _cellHeight;
		cfg.walkableSlopeAngle = _agentMaxSlope;
		cfg.walkableHeight = (int)ceilf(_agentHeight / cfg.ch);
		cfg.walkableClimb = (int)floorf(_agentMaxClimb / cfg.ch);
		cfg.walkableRadius = (int)ceilf(_agentRadius / cfg.cs);
		cfg.maxEdgeLen = (int)(_edgeMaxLen / _cellSize);
		cfg.maxSimplificationError = _edgeMaxError;
		cfg.minRegionArea = (int)rcSqr(_regionMinSize);		// Note: area = size*size
		cfg.mergeRegionArea = (int)rcSqr(_regionMergeSize);	// Note: area = size*size
		cfg.maxVertsPerPoly = (int)_vertsPerPoly;
		cfg.tileSize = (int)_tileSize;
		cfg.borderSize = cfg.walkableRadius + 3; // Reserve enough padding.
		cfg.width = cfg.tileSize + cfg.borderSize * 2;
		cfg.height = cfg.tileSize + cfg.borderSize * 2;
		cfg.detailSampleDist = _detailSampleDist < 0.9f ? 0 : _cellSize * _detailSampleDist;
		cfg.detailSampleMaxError = _cellHeight * _detailSampleMaxError;
		rcVcopy(cfg.bmin, bmin);
		rcVcopy(cfg.bmax, bmax);

		// Tile cache params.
		dtTileCacheParams tcparams;
		memset(&tcparams, 0, sizeof(tcparams));
		rcVcopy(tcparams.orig, bmin);
		tcparams.cs = _cellSize;
		tcparams.ch = _cellHeight;
		tcparams.width = (int)_tileSize;
		tcparams.height = (int)_tileSize;
		tcparams.walkableHeight = _package[index]._agentsetting._agentHeight;
		tcparams.walkableRadius = _package[index]._agentsetting._agentRadius;
		tcparams.walkableClimb = _package[index]._agentsetting._agentMaxClimb;
		tcparams.maxSimplificationError = _edgeMaxError;
		tcparams.maxTiles = tw * th * 4;
		tcparams.maxObstacles = 128;

		dtFreeTileCache(_package[index]._tileCache);

		_package[index]._tileCache = dtAllocTileCache();
		if (!_package[index]._tileCache)
		{
			_ctx->log(RC_LOG_ERROR, "buildTiledNavigation: Could not allocate tile cache.");
			return false;
		}
		status = _package[index]._tileCache->init(&tcparams, _talloc, _tcomp, _tmproc);
		if (dtStatusFailed(status))
		{
			_ctx->log(RC_LOG_ERROR, "buildTiledNavigation: Could not init tile cache.");
			return false;
		}

		dtFreeNavMesh(_package[0]._navMesh);

		_package[0]._navMesh = dtAllocNavMesh();
		if (!_package[0]._navMesh)
		{
			_ctx->log(RC_LOG_ERROR, "buildTiledNavigation: Could not allocate navmesh.");
			return false;
		}

		dtNavMeshParams params;
		memset(&params, 0, sizeof(params));
		rcVcopy(params.orig, bmin);
		params.tileWidth = _tileSize * _cellSize;
		params.tileHeight = _tileSize * _cellSize;
		params.maxTiles = _maxTiles;
		params.maxPolys = _maxPolysPerTile;

		status = _package[0]._navMesh->init(&params);
		if (dtStatusFailed(status))
		{
			_ctx->log(RC_LOG_ERROR, "buildTiledNavigation: Could not init navmesh.");
			return false;
		}

		status = _package[0]._navQuery->init(_package[0]._navMesh, 2048);
		if (dtStatusFailed(status))
		{
			_ctx->log(RC_LOG_ERROR, "buildTiledNavigation: Could not init Detour navmesh query");
			return false;
		}


		// Preprocess tiles.

		_ctx->resetTimers();

		_cacheLayerCount = 0;
		_cacheCompressedSize = 0;
		_cacheRawSize = 0;

		for (int y = 0; y < th; ++y)
		{
			for (int x = 0; x < tw; ++x)
			{
				TileCacheData tiles[MAX_LAYERS];
				memset(tiles, 0, sizeof(tiles));
				int ntiles = rasterizeTileLayers(x, y, cfg, tiles, MAX_LAYERS);

				for (int i = 0; i < ntiles; ++i)
				{
					TileCacheData* tile = &tiles[i];
					status = _package[index]._tileCache->addTile(tile->data, tile->dataSize, DT_COMPRESSEDTILE_FREE_DATA, 0);
					if (dtStatusFailed(status))
					{
						dtFree(tile->data);
						tile->data = 0;
						continue;
					}

					_cacheLayerCount++;
					_cacheCompressedSize += tile->dataSize;
					_cacheRawSize += calcLayerBufferSize(tcparams.width, tcparams.height);
				}
			}
		}

		// Build initial meshes
		_ctx->startTimer(RC_TIMER_TOTAL);
		for (int y = 0; y < th; ++y)
		{

			for (int x = 0; x < tw; ++x)
			{
				_package[index]._tileCache->buildNavMeshTilesAt(x, y, _package[0]._navMesh);
			}
		}

		const dtNavMesh* nav = _package[0]._navMesh;
		int navmeshMemUsage = 0;
		for (int i = 0; i < nav->getMaxTiles(); ++i)
		{
			const dtMeshTile* tile = nav->getTile(i);
			if (tile->header)
				navmeshMemUsage += tile->dataSize;
		}
		return true;
	}

	int Navigation::rasterizeTileLayers(const int tx, const int ty, const rcConfig& cfg, struct TileCacheData* tiles, const int maxTiles)
	{
		if (!_geom || !_geom->getMesh() || !_geom->getChunkyMesh())
		{
			_ctx->log(RC_LOG_ERROR, "buildTile: Input mesh is not specified.");
			return 0;
		}

		FastLZCompressor comp;
		RasterizationContext rc;

		const float* verts = _geom->getMesh()->getVerts();
		const int nverts = _geom->getMesh()->getVertCount();
		const rcChunkyTriMesh* chunkyMesh = _geom->getChunkyMesh();

		// Tile bounds.
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
			_ctx->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'solid'.");
			return 0;
		}
		if (!rcCreateHeightfield(_ctx, *rc.solid, tcfg.width, tcfg.height, tcfg.bmin, tcfg.bmax, tcfg.cs, tcfg.ch))
		{
			_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not create solid heightfield.");
			return 0;
		}

		// Allocate array that can hold triangle flags.
		// If you have multiple meshes you need to process, allocate
		// and array which can hold the max number of triangles you need to process.
		rc.triareas = new unsigned char[chunkyMesh->maxTrisPerChunk];
		if (!rc.triareas)
		{
			_ctx->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'm_triareas' (%d).", chunkyMesh->maxTrisPerChunk);
			return 0;
		}

		float tbmin[2], tbmax[2];
		tbmin[0] = tcfg.bmin[0];
		tbmin[1] = tcfg.bmin[2];
		tbmax[0] = tcfg.bmax[0];
		tbmax[1] = tcfg.bmax[2];
		int cid[512];// TODO: Make grow when returning too many items.
		const int ncid = rcGetChunksOverlappingRect(chunkyMesh, tbmin, tbmax, cid, 512);
		if (!ncid)
		{
			return 0; // empty
		}

		for (int i = 0; i < ncid; ++i)
		{
			const rcChunkyTriMeshNode& node = chunkyMesh->nodes[cid[i]];
			const int* tris = &chunkyMesh->tris[node.i * 3];
			const int ntris = node.n;

			memset(rc.triareas, 0, ntris * sizeof(unsigned char));
			rcMarkWalkableTriangles(_ctx, tcfg.walkableSlopeAngle,
				verts, nverts, tris, ntris, rc.triareas);

			if (!rcRasterizeTriangles(_ctx, verts, nverts, tris, rc.triareas, ntris, *rc.solid, tcfg.walkableClimb))
				return 0;
		}

		// Once all geometry is rasterized, we do initial pass of filtering to
		// remove unwanted overhangs caused by the conservative rasterization
		// as well as filter spans where the character cannot possibly stand.
		if (_filterLowHangingObstacles)
			rcFilterLowHangingWalkableObstacles(_ctx, tcfg.walkableClimb, *rc.solid);
		if (_filterLedgeSpans)
			rcFilterLedgeSpans(_ctx, tcfg.walkableHeight, tcfg.walkableClimb, *rc.solid);
		if (_filterWalkableLowHeightSpans)
			rcFilterWalkableLowHeightSpans(_ctx, tcfg.walkableHeight, *rc.solid);


		rc.chf = rcAllocCompactHeightfield();
		if (!rc.chf)
		{
			_ctx->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'chf'.");
			return 0;
		}
		if (!rcBuildCompactHeightfield(_ctx, tcfg.walkableHeight, tcfg.walkableClimb, *rc.solid, *rc.chf))
		{
			_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not build compact data.");
			return 0;
		}

		// Erode the walkable area by agent radius.
		if (!rcErodeWalkableArea(_ctx, tcfg.walkableRadius, *rc.chf))
		{
			_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not erode.");
			return 0;
		}

		// (Optional) Mark areas.
		const ConvexVolume* vols = _geom->getConvexVolumes();
		for (int i = 0; i < _geom->getConvexVolumeCount(); ++i)
		{
			rcMarkConvexPolyArea(_ctx, vols[i].verts, vols[i].nverts,
				vols[i].hmin, vols[i].hmax,
				(unsigned char)vols[i].area, *rc.chf);
		}

		rc.lset = rcAllocHeightfieldLayerSet();
		if (!rc.lset)
		{
			_ctx->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'lset'.");
			return 0;
		}
		if (!rcBuildHeightfieldLayers(_ctx, *rc.chf, tcfg.borderSize, tcfg.walkableHeight, *rc.lset))
		{
			_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not build heighfield layers.");
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

	void Navigation::getTilePos(const float* pos, int& tx, int& ty)
	{
		if (!_geom) return;

		const float* bmin = _geom->getNavMeshBoundsMin();

		const float ts = _tileSize * _cellSize;
		tx = (int)((pos[0] - bmin[0]) / ts);
		ty = (int)((pos[2] - bmin[2]) / ts);
	}

	void Navigation::renderCachedTile(const int tx, const int ty, const int type)
	{

	}

	void Navigation::renderCachedTileOverlay(const int tx, const int ty, double* proj, double* model, int* view)
	{

	}

	void Navigation::HandleUpdate(const float dt)
	{
		for (int i = 0; i < PACKAGESIZE; i++)
		{
			if (!_package[i]._navMesh)
				return;
				return;
			if (!_package[i]._tileCache)
				return;
			_package[i]._tileCache->update(dt, _package[i]._navMesh);
		}
	}

	dtObstacleRef Navigation::hitTestObstacle(const dtTileCache* tc, const float* sp, const float* sq)
	{
		float tmin = FLT_MAX;
		const dtTileCacheObstacle* obmin = 0;
		for (int i = 0; i < tc->getObstacleCount(); ++i)
		{
			const dtTileCacheObstacle* ob = tc->getObstacle(i);
			if (ob->state == DT_OBSTACLE_EMPTY)
				continue;

			float bmin[3], bmax[3], t0, t1;
			tc->getObstacleBounds(ob, bmin, bmax);

			if (isectSegAABB(sp, sq, bmin, bmax, t0, t1))
			{
				if (t0 < tmin)
				{
					tmin = t0;
					obmin = ob;
				}
			}
		}
		return tc->getObstacleRef(obmin);
	}

	bool Navigation::isectSegAABB(const float* sp, const float* sq, const float* amin, const float* amax, float& tmin, float& tmax)
	{
		static const float EPS = 1e-6f;

		float d[3];
		rcVsub(d, sq, sp);
		tmin = 0;  // set to -FLT_MAX to get first hit on line
		tmax = FLT_MAX;		// set to max distance ray can travel (for segment)

		// For all three slabs
		for (int i = 0; i < 3; i++)
		{
			if (fabsf(d[i]) < EPS)
			{
				// Ray is parallel to slab. No hit if origin not within slab
				if (sp[i] < amin[i] || sp[i] > amax[i])
					return false;
			}
			else
			{
				// Compute intersection t value of ray with near and far plane of slab
				const float ood = 1.0f / d[i];
				float t1 = (amin[i] - sp[i]) * ood;
				float t2 = (amax[i] - sp[i]) * ood;
				// Make t1 be intersection with near plane, t2 with far plane
				if (t1 > t2) rcSwap(t1, t2);
				// Compute the intersection of slab intersections intervals
				if (t1 > tmin) tmin = t1;
				if (t2 < tmax) tmax = t2;
				// Exit with no collision as soon as slab intersection becomes empty
				if (tmin > tmax) return false;
			}
		}

		return true;
	}

	void Navigation::SetSEpos(int index, float sx, float sy, float sz, float ex, float ey, float ez)
	{
		_package[index]._startPos[0] = sx;
		_package[index]._startPos[1] = sy;
		_package[index]._startPos[2] = sz;
		_package[index]._navQuery->findNearestPoly(_package[index]._startPos, _package[index]._polyPickExt, &(_package[index]._filter), &(_package[index]._startRef), 0);

		_package[index]._endPos[0] = ex;
		_package[index]._endPos[1] = ey;
		_package[index]._endPos[2] = ez;
		_package[index]._navQuery->findNearestPoly(_package[index]._endPos, _package[index]._polyPickExt, &(_package[index]._filter), &(_package[index]._endRef), 0);
	}

	void Navigation::SetSEpos(int index, Pg::Math::PGFLOAT3 startPosition, Pg::Math::PGFLOAT3 endPosition)
	{

	}

	void Navigation::SetStartpos(int index, float x, float y, float z)
	{

	}

	void Navigation::SetStartpos(int index, Pg::Math::PGFLOAT3 position)
	{

	}

	void Navigation::SetEndpos(int index, float x, float y, float z)
	{

	}

	void Navigation::SetEndpos(int index, Pg::Math::PGFLOAT3 position)
	{

	}

	void Navigation::SetAgent(int index, float agentHeight, float agentMaxSlope, float agentRadius, float agentMaxClimb)
	{

	}
}