#pragma once

#include "../ParagonData/BaseRenderer.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/RenderTextData.h"
#include "../ParagonData/CameraData.h"

#include <memory>

/// <summary>
/// Graphics �������� ���̴� ��� �����Ǵ� ������Ʈ���� �⺻ Ŭ�����̴�.
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


