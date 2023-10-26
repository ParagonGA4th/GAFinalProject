#include "StaticMeshRenderer.h"
#include "../ParagonUtil/ResourceHelper.h"

namespace Pg::Data
{
	using Pg::Util::Helper::ResourceHelper;

	StaticMeshRenderer::StaticMeshRenderer(GameObject* obj) : RendererBase3D(obj)
	{
		SetRendererTypeName(typeid(this).name());
	}

	void StaticMeshRenderer::SetMeshFilePath(const std::string& meshFilePath)
	{
		this->_meshFilePath = ResourceHelper::ForcePathUniform(meshFilePath);
	}

	std::string StaticMeshRenderer::GetMeshFilePath()
	{
		return _meshFilePath;
	}

}