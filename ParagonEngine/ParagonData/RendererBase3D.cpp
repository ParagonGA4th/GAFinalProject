#include "RendererBase3D.h"
#include "StaticMeshRenderer.h"
#include "SkinnedMeshRenderer.h"
#include "RendererBase3D.h"
#include "GameObject.h"


#include "../ParagonHelper/ResourceHelper.h"

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

		if (this->GetRendererTypeName().compare(std::string(typeid(Pg::Data::StaticMeshRenderer*).name())) == 0)
		{
			StaticMeshRenderer* tVal = static_cast<StaticMeshRenderer*>(this);
			tVal->_meshName = ResourceHelper::GetNameFromPath(this->_meshFilePath);
		}
		else
		{
			SkinnedMeshRenderer* tVal = static_cast<SkinnedMeshRenderer*>(this);
			tVal->_meshName = ResourceHelper::GetNameFromPath(this->_meshFilePath);
		}
	}

	std::string RendererBase3D::GetMeshFilePath()
	{
		return _meshFilePath;
	}

	void RendererBase3D::SetMaterialFilePath(const std::string& materialFilePath)
	{
		this->_renderMaterialPath = ResourceHelper::ForcePathUniform(materialFilePath);

		if (this->GetRendererTypeName().compare(std::string(typeid(Pg::Data::StaticMeshRenderer*).name())) == 0)
		{
			StaticMeshRenderer* tVal = static_cast<StaticMeshRenderer*>(this);
			tVal->_materialName = ResourceHelper::GetNameFromPath(this->_renderMaterialPath);
		}
		else
		{
			SkinnedMeshRenderer* tVal = static_cast<SkinnedMeshRenderer*>(this);
			tVal->_materialName = ResourceHelper::GetNameFromPath(this->_renderMaterialPath);
		}
	}

	std::string RendererBase3D::GetMaterialFilePath()
	{
		return _renderMaterialPath;
	}

}
