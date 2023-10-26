#pragma once

#include "../ParagonData/BaseRenderer.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/RenderTextData.h"
#include "../ParagonData/CameraData.h"

#include <memory>

/// <summary>
/// Graphics 엔진에서 쓰이는 모든 렌더되는 오브젝트들의 기본 클래스이다.
/// </summary>


namespace Pg::Data
{
	class BaseRenderer;
}

namespace Pg::Graphics
{
	namespace Manager
	{
		class GraphicsResourceManager;
	}
}

namespace Pg::Graphics
{
	class RenderObjectBase 
	{
	public:
		RenderObjectBase(Pg::Data::BaseRenderer* baseRenderer);

		Pg::Data::BaseRenderer* GetBaseRenderer();
	protected:
		Pg::Data::BaseRenderer* _baseRenderer = nullptr;
		Pg::Graphics::Manager::GraphicsResourceManager* _graphicsResourceManager = nullptr;
	};
}


