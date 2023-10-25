#include "StaticMeshRenderer.h"

namespace Pg::Data
{
	StaticMeshRenderer::StaticMeshRenderer(GameObject* obj) : RendererBase3D(obj)
	{
		SetRendererTypeName(typeid(this).name());
	}

	void StaticMeshRenderer::Render()
	{

	}

}