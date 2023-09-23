#include "BaseRenderer.h"
#include "IRenderObject.h"
#include "../ParagonGraphics/RenderObject.h"

#ifdef _DEBUG
#pragma comment(lib,"..\\x64\\Debug\\ParagonGraphics.lib")
#else
#pragma comment(lib,"..\\x64\\Release\\ParagonGraphics.lib")
#endif // _DEBUG

namespace Pg::Core
{
	BaseRenderer::BaseRenderer(GameObject* obj) : Component(obj)
	{
		_renderInfo = std::make_unique<Pg::Graphics::RenderObject>();
	}

	BaseRenderer::~BaseRenderer()
	{

	}

}