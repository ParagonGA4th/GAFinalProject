#pragma once
#include "../ParagonData/IRenderObject.h"

/// <summary>
/// Graphics 엔진에서 쓰이는 모든 렌더되는 오브젝트들의 기본 클래스이다.
/// </summary>
/// 

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
	class RenderObjectBase : public Pg::Data::IRenderObject
	{
	public:
		RenderObjectBase(Pg::Data::BaseRenderer* baseRenderer);

		Pg::Data::BaseRenderer* _baseRenderer = nullptr;
	protected:
		Pg::Graphics::Manager::GraphicsResourceManager* _graphicsResourceManager = nullptr;
	};
}


