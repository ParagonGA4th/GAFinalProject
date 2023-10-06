#include "BaseRenderer.h"
#include "IRenderObject.h"
#include "../ParagonGraphics/GraphicsDLLExporter.h"


namespace Pg::Data
{
	BaseRenderer::BaseRenderer(GameObject* obj) : Component(obj)
	{
		//
	}

	BaseRenderer::~BaseRenderer()
	{
		if (_renderObject != nullptr)
		{
			delete _renderObject;
		}
	}

	Pg::Data::IRenderObject* BaseRenderer::GetRenderObject()
	{
		return _renderObject;
	}

}