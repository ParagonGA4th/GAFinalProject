#include "RenderObject2D.h"
#include "GraphicsResourceManager.h"
#include "../ParagonData/BaseRenderer.h"
#include "../ParagonUtil/ResourceHelper.h"

#include <cassert>

namespace Pg::Graphics
{
	using Pg::Graphics::Manager::GraphicsResourceManager;
	using Pg::Util::Helper::ResourceHelper;

	RenderObject2D::RenderObject2D(Pg::Data::BaseRenderer* baseRenderer) : RenderObjectBase(baseRenderer)
	{
		//
	}

	RenderObject2D::~RenderObject2D()
	{

	}

	void RenderObject2D::Render(Pg::Data::CameraData* camData)
	{
		assert(false);
	}



}