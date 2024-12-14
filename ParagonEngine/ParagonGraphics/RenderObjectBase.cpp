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
		//���� ��, ������ �� �ʿ��� ������ �޾ƿ;� �Ѵ�.
	}

	Pg::Data::BaseRenderer* RenderObjectBase::GetBaseRenderer()
	{
		return _baseRenderer;
	}

}
