#include "StaticMeshRenderer.h"
#include "../ParagonUtil/ResourceHelper.h"

#include <generic_factory/generic_factory.hpp>

using namespace Pg::Data;
REGISTER_CHILD_INTO_FACTORY(RendererBase3D, StaticMeshRenderer, "class Pg::Data::StaticMeshRenderer", GameObject*);

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