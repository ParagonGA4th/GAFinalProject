#pragma once
#include "RenderObject3D.h"

/// <summary>
/// InstancedStaticMesh.
/// </summary>

namespace Pg::Graphics
{
	class RenderObjectInstancedMesh3D : public RenderObject3D
	{
	public:
		RenderObjectInstancedMesh3D(Pg::Data::BaseRenderer* baseRenderer, unsigned int objID);
		~RenderObjectInstancedMesh3D();

	};
}

