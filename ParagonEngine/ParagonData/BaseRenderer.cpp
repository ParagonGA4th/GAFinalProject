#include "BaseRenderer.h"

#include "../ParagonGraphics/GraphicsDLLExporter.h"


namespace Pg::Data
{
	BaseRenderer::BaseRenderer(GameObject* obj) : Component(obj)
	{
		//
	}

	BaseRenderer::~BaseRenderer()
	{

	}

	void BaseRenderer::SetRendererTypeName(const std::string& typeName)
	{
		_rendererTypeName = typeName;
	}

	const std::string& BaseRenderer::GetRendererTypeName() const
	{
		return _rendererTypeName;
	}

	

}