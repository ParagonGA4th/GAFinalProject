#include "SkinnedMeshRenderer.h"

#include "../ParagonUtil/ResourceHelper.h"

namespace Pg::Data
{
	using Pg::Util::Helper::ResourceHelper;

	SkinnedMeshRenderer::SkinnedMeshRenderer(GameObject* obj) : RendererBase3D(obj)
	{
		SetRendererTypeName(typeid(this).name());
	}



}