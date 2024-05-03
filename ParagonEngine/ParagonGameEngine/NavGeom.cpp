#include "NavGeom.h"
#include <math.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <algorithm>

#include <DebugUtils/DebugDraw.h>
#include <DebugUtils/RecastDebugDraw.h>
#include <Detour/DetourNavMesh.h>
#include <Recast/Recast.h>
#include "Navigation.h"

#pragma warning(disable:4996)

static bool intersectSegmentTriangle(const float* sp, const float* sq,
	const float* a, const float* b, const float* c,
	float& t)
{
	float v, w;
	float ab[3], ac[3], qp[3], ap[3], norm[3], e[3];
	rcVsub(ab, b, a);
	rcVsub(ac, c, a);
	rcVsub(qp, sp, sq);

	// Compute triangle normal. Can be precalculated or cached if
	// intersecting multiple segments against the same triangle
	rcVcross(norm, ab, ac);

	// Compute denominator d. If d <= 0, segment is parallel to or points
	// away from triangle, so exit early
	float d = rcVdot(qp, norm);
	if (d <= 0.0f) return false;

	// Compute intersection t value of pq with plane of triangle. A ray
	// intersects iff 0 <= t. Segment intersects iff 0 <= t <= 1. Delay
	// dividing by d until intersection has been found to pierce triangle
	rcVsub(ap, sp, a);
	t = rcVdot(ap, norm);
	if (t < 0.0f) return false;
	if (t > d) return false; // For segment; exclude this code line for a ray test

	// Compute barycentric coordinate components and test if within bounds
	rcVcross(e, qp, ap);
	v = rcVdot(ac, e);
	if (v < 0.0f || v > d) return false;
	w = -rcVdot(ab, e);
	if (w < 0.0f || v + w > d) return false;

	// Segment/ray intersects triangle. Perform delayed division
	t /= d;

	return true;
}

static char* parseRow(char* buf, char* bufEnd, char* row, int len)
{
	bool start = true;
	bool done = false;
	int n = 0;
	while (!done && buf < bufEnd)
	{
		char c = *buf;
		buf++;
		// multirow
		switch (c)
		{
		case '\n':
			if (start) break;
			done = true;
			break;
		case '\r':
			break;
		case '\t':
		case ' ':
			if (start) break;
			// else falls through
		default:
			start = false;
			row[n++] = c;
			if (n >= len - 1)
				done = true;
			break;
		}
	}
	row[n] = '\0';
	return buf;
}

bool NavGeom::loadMesh(class rcContext* ctx, const std::string& filepath)
{
	if (m_mesh != nullptr)
	{
		delete m_chunkyMesh;
		m_chunkyMesh = 0;
		delete m_mesh;
		m_mesh = 0;
	}
	m_offMeshConCount = 0;
	m_volumeCount = 0;

	m_mesh = new Pg::Engine::MeshLoaderObj;
	if (!m_mesh)
	{
		ctx->log(RC_LOG_ERROR, "loadMesh: Out of memory 'm_mesh'.");
		return false;
	}
	if (!m_mesh->load(filepath))
	{
		ctx->log(RC_LOG_ERROR, "buildTiledNavigation: Could not load '%s'", filepath.c_str());
		return false;
	}

	rcCalcBounds(m_mesh->getVerts(), m_mesh->getVertCount(), m_meshBMin, m_meshBMax);

	m_chunkyMesh = new rcChunkyTriMesh;
	if (!m_chunkyMesh)
	{
		ctx->log(RC_LOG_ERROR, "buildTiledNavigation: Out of memory 'm_chunkyMesh'.");
		return false;
	}
	if (!rcCreateChunkyTriMesh(m_mesh->getVerts(), m_mesh->getTris(), m_mesh->getTriCount(), 256, m_chunkyMesh))
	{
		ctx->log(RC_LOG_ERROR, "buildTiledNavigation: Failed to build chunky mesh.");
		return false;
	}

	return true;
}

bool NavGeom::loadGeomSet(class rcContext* ctx, const std::string& filepath)
{
	return true;
}

NavGeom::NavGeom() :
	m_chunkyMesh(nullptr),
	m_mesh(nullptr),
	m_hasBuildSettings(false),
	m_offMeshConCount(0),
	m_volumeCount(0)
{

}

NavGeom::~NavGeom()
{
	delete m_chunkyMesh;
	delete m_mesh;
}

bool NavGeom::load(class rcContext* ctx, const std::string& filepath)
{
	return true;
}

bool NavGeom::saveGeomSet(const BuildSettings* settings)
{
	return true;
}

bool NavGeom::raycastMesh(float* src, float* dst, float& tmin)
{
	return true;
}

void NavGeom::addOffMeshConnection(const float* spos, const float* epos, const float rad, unsigned char bidir, unsigned char area, unsigned short flags)
{

}

void NavGeom::deleteOffMeshConnection(int i)
{

}

void NavGeom::drawOffMeshConnections(struct duDebugDraw* dd, bool hilight /*= false*/)
{

}

void NavGeom::addConvexVolume(const float* verts, const int nverts, const float minh, const float maxh, unsigned char area)
{

}

void NavGeom::deleteConvexVolume(int i)
{

}

void NavGeom::drawConvexVolumes(struct duDebugDraw* dd, bool hilight /*= false*/)
{

}
