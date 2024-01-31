#include "StaticMeshRenderer.h"
#include "../ParagonUtil/ResourceHelper.h"

namespace Pg::Data
{
	StaticMeshRenderer::StaticMeshRenderer(GameObject* obj) : RendererBase3D(obj)
	{
		SetRendererTypeName(typeid(this).name());
	}

	void StaticMeshRenderer::InitializePathsFromName()
	{
		//후에 호출될 예정.
	}

}