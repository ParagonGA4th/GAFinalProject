#include "RenderObjectBase.h"
#include "GraphicsResourceManager.h"
#include "../ParagonData/BaseRenderer.h"

#include <cassert>

namespace Pg::Graphics
{
	using Pg::Graphics::Manager::GraphicsResourceManager;

	RenderObjectBase::RenderObjectBase(Pg::Data::BaseRenderer* baseRenderer) 
		: _baseRenderer(baseRenderer), _graphicsResourceManager(GraphicsResourceManager::Instance())
	{
		//만들 때, 렌더할 때 필요한 정보를 받아와야 한다.
	}

	Pg::Data::BaseRenderer* RenderObjectBase::GetBaseRenderer()
	{
		return _baseRenderer;
	}

}
