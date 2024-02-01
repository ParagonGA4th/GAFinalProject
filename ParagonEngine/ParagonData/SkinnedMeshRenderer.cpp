#include "SkinnedMeshRenderer.h"

namespace Pg::Data
{
	SkinnedMeshRenderer::SkinnedMeshRenderer(GameObject* obj) : RendererBase3D(obj)
	{
		SetRendererTypeName(typeid(this).name());
	}

	void SkinnedMeshRenderer::ConvertPotentialUnrealValues()
	{
		//
	}

}