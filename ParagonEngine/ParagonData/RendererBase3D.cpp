#include "RendererBase3D.h"
#include "GameObject.h"

#include "../ParagonUtil/ResourceHelper.h"

#include <cassert>

namespace Pg::Data
{
	using Pg::Util::Helper::ResourceHelper;

	RendererBase3D::RendererBase3D(GameObject* obj) : BaseRenderer(obj)
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


}
