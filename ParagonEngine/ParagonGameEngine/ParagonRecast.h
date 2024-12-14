#pragma once
#pragma once
#include "../ParagonData/NavigationField.h"
#include "NavGeom.h"

#include <Recast/Recast.h>
#include <Detour/DetourNavMesh.h>
#include <Detour/DetourNavMeshBuilder.h>
#include <Detour/DetourNavMeshQuery.h>
#include <DetourTileCache/DetourTileCache.h>
#include <DetourTileCache/DetourTileCacheBuilder.h>
#include <Detour/DetourCommon.h>
#include <Detour/DetourMath.h>
#include <Detour/DetourAlloc.h>
#include <Detour/DetourAssert.h>
#include <DetourCrowd/DetourCrowd.h>
#include <fastlz/fastlz.h>

namespace Pg::Engine
{
	enum SamplePolyAreas
	{
		SAMPLE_POLYAREA_GROUND,
		SAMPLE_POLYAREA_WATER,
		SAMPLE_POLYAREA_ROAD,
		SAMPLE_POLYAREA_DOOR,
		SAMPLE_POLYAREA_GRASS,
		SAMPLE_POLYAREA_JUMP
	};

	enum SamplePolyFlags
	{
		SAMPLE_POLYFLAGS_WALK = 0x01,		// 걷기 능력 (지상, 풀, 도로)
		SAMPLE_POLYFLAGS_SWIM = 0x02,		// 수영 능력 (물)
		SAMPLE_POLYFLAGS_DOOR = 0x04,		// 문 통과 능력
		SAMPLE_POLYFLAGS_JUMP = 0x08,		// 점프 능력
		SAMPLE_POLYFLAGS_DISABLED = 0x10,	// 비활성화된 폴리곤
		SAMPLE_POLYFLAGS_ALL = 0xffff		// 모든 능력
	};

	static const int NAVMESHSET_MAGIC = 'M' << 24 | 'S' << 16 | 'E' << 8 | 'T'; //'MSET';
	static const int NAVMESHSET_VERSION = 1;

	static const int TILECACHESET_MAGIC = 'T' << 24 | 'S' << 16 | 'E' << 8 | 'T'; //'TSET';
	static const int TILECACHESET_VERSION = 1;
	static const int EXPECTED_LAYERS_PER_TILE = 4;
	static const int MAX_LAYERS = 32;

	struct FastLZCompressor : public dtTileCacheCompressor
	{
		virtual ~FastLZCompressor();

		virtual int maxCompressedSize(const int bufferSize)
		{
			return (int)(bufferSize * 1.05f);
		}

		virtual dtStatus compress(const unsigned char* buffer, const int bufferSize,
			unsigned char* compressed, const int /*maxCompressedSize*/, int* compressedSize)
		{
			*compressedSize = fastlz_compress((const void* const)buffer, bufferSize, compressed);
			return DT_SUCCESS;
		}

		virtual dtStatus decompress(const unsigned char* compressed, const int compressedSize,
			unsigned char* buffer, const int maxBufferSize, int* bufferSize)
		{
			*bufferSize = fastlz_decompress(compressed, compressedSize, buffer, maxBufferSize);
			return *bufferSize < 0 ? DT_FAILURE : DT_SUCCESS;
		}
	};

	FastLZCompressor::~FastLZCompressor()
	{
		// Defined out of line to fix the weak v-tables warning
	}

	struct LinearAllocator : public dtTileCacheAlloc
	{
		unsigned char* buffer;
		size_t capacity;
		size_t top;
		size_t high;

		LinearAllocator(const size_t cap) : buffer(0), capacity(0), top(0), high(0)
		{
			resize(cap);
		}

		virtual ~LinearAllocator();

		void resize(const size_t cap)
		{
			if (buffer) dtFree(buffer);
			buffer = (unsigned char*)dtAlloc(cap, DT_ALLOC_PERM);
			capacity = cap;
		}

		virtual void reset()
		{
			high = dtMax(high, top);
			top = 0;
		}

		virtual void* alloc(const size_t size)
		{
			if (!buffer)
				return 0;
			if (top + size > capacity)
				return 0;
			unsigned char* mem = &buffer[top];
			top += size;
			return mem;
		}

		virtual void free(void* /*ptr*/)
		{
			// Empty
		}
	};

	LinearAllocator::~LinearAllocator()
	{
		// Defined out of line to fix the weak v-tables warning
		dtFree(buffer);
	}

	struct MeshProcess : public dtTileCacheMeshProcess
	{
		NavGeom* m_geom;

		MeshProcess();

		virtual ~MeshProcess();

		void init(NavGeom* geom) { m_geom = geom; };

		virtual void process(struct dtNavMeshCreateParams* params, unsigned char* polyAreas,
			unsigned short* polyFlags)
		{
			// Update poly flags from areas.
			for (int i = 0; i < params->polyCount; ++i)
			{
				if (polyAreas[i] == DT_TILECACHE_WALKABLE_AREA)
					polyAreas[i] = SAMPLE_POLYAREA_GROUND;

				if (polyAreas[i] == SAMPLE_POLYAREA_GROUND ||
					polyAreas[i] == SAMPLE_POLYAREA_GRASS ||
					polyAreas[i] == SAMPLE_POLYAREA_ROAD)
				{
					polyFlags[i] = SAMPLE_POLYFLAGS_WALK;
				}
				else if (polyAreas[i] == SAMPLE_POLYAREA_WATER)
				{
					polyFlags[i] = SAMPLE_POLYFLAGS_SWIM;
				}
				else if (polyAreas[i] == SAMPLE_POLYAREA_DOOR)
				{
					polyFlags[i] = SAMPLE_POLYFLAGS_WALK | SAMPLE_POLYFLAGS_DOOR;
				}
			}

			// Pass in off-mesh connections.
			if (m_geom)
			{
				params->offMeshConVerts = m_geom->getOffMeshConnectionVerts();
				params->offMeshConRad = m_geom->getOffMeshConnectionRads();
				params->offMeshConDir = m_geom->getOffMeshConnectionDirs();
				params->offMeshConAreas = m_geom->getOffMeshConnectionAreas();
				params->offMeshConFlags = m_geom->getOffMeshConnectionFlags();
				params->offMeshConUserID = m_geom->getOffMeshConnectionId();
				params->offMeshConCount = m_geom->getOffMeshConnectionCount();
			}
		};
	};

	MeshProcess::MeshProcess()
	{

	}

	MeshProcess::~MeshProcess()
	{
		// Defined out of line to fix the weak v-tables warning
	}

	struct TileCacheData
	{
		unsigned char* data;
		int dataSize;
	};

	struct RasterizationContext
	{
		RasterizationContext() :
			solid(0),
			triareas(0),
			lset(0),
			chf(0),
			ntiles(0)
		{
			memset(tiles, 0, sizeof(TileCacheData) * MAX_LAYERS);
		}

		~RasterizationContext()
		{
			rcFreeHeightField(solid);
			delete[] triareas;
			rcFreeHeightfieldLayerSet(lset);
			rcFreeCompactHeightfield(chf);
			for (int i = 0; i < MAX_LAYERS; ++i)
			{
				dtFree(tiles[i].data);
				tiles[i].data = 0;
			}
		}

		rcHeightfield* solid;
		unsigned char* triareas;
		rcHeightfieldLayerSet* lset;
		rcCompactHeightfield* chf;
		TileCacheData tiles[MAX_LAYERS];
		int ntiles;
	};

	struct TileCacheSetHeader
	{
		int magic;
		int version;
		int numTiles;
		dtNavMeshParams meshParams;
		dtTileCacheParams cacheParams;
	};

	struct TileCacheTileHeader
	{
		dtCompressedTileRef tileRef;
		int dataSize;
	};
}
