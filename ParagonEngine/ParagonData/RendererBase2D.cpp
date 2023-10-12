#include "RendererBase2D.h"
#include "GameObject.h"
#include "IRenderObject.h"

namespace Pg::Data
{
	RendererBase2D::RendererBase2D(GameObject* obj) : BaseRenderer(obj)
	{
		
	}

	RendererBase2D::~RendererBase2D()
	{
		//
	}
}