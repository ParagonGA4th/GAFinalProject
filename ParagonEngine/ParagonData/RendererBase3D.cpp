#include "RendererBase3D.h"
#include "GameObject.h"

#include "../ParagonUtil/ResourceHelper.h"

#include <cassert>

namespace Pg::Data
{
	using Pg::Util::Helper::ResourceHelper;

	RendererBase3D::RendererBase3D(GameObject* obj) : BaseRenderer(obj), _meshFilePath(""), _renderMaterialPath("")
	{
		
	}

	RendererBase3D::~RendererBase3D()
	{
		//
	}

	void RendererBase3D::SetMeshFilePath(const std::string& meshFilePath)
	{
		this->_meshFilePath = ResourceHelper::ForcePathUniform(meshFilePath);
	}

	std::string RendererBase3D::GetMeshFilePath()
	{
		return _meshFilePath;
	}

	void RendererBase3D::SetMaterialFilePath(const std::string& materialFilePath)
	{
		this->_renderMaterialPath = ResourceHelper::ForcePathUniform(materialFilePath);
	}

	std::string RendererBase3D::GetMaterialFilePath()
	{
		return _renderMaterialPath;
	}

}
