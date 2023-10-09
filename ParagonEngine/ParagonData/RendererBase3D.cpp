#include "RendererBase3D.h"
#include "GameObject.h"
#include "IRenderObject.h"

namespace Pg::Data
{
	RendererBase3D::RendererBase3D(GameObject* obj) : BaseRenderer(obj)
	{
		
	}

	RendererBase3D::~RendererBase3D()
	{
		//
	}

}
