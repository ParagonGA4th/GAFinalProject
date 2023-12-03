#include "RenderObjectSkinnedMesh3D.h"

#include "GraphicsResourceHelper.h"
#include "GraphicsResourceManager.h"
#include "LowDX11Storage.h"
#include "../ParagonData/SkinnedMeshRenderer.h"
#include "LayoutDefine.h"
#include "MathHelper.h"
#include "RenderTexture2D.h"
#include "AssetModelDataDefine.h"
#include "AssetTextureType.h"
#include "MaterialCluster.h"

#include "dxtk/WICTextureLoader.h"

namespace Pg::Graphics
{
	using Pg::Graphics::Helper::MathHelper;
	using Pg::Graphics::Manager::GraphicsResourceManager;
	using Pg::Data::Enums::eAssetDefine;


	RenderObjectSkinnedMesh3D::RenderObjectSkinnedMesh3D(Pg::Data::BaseRenderer* baseRenderer) : RenderObject3D(baseRenderer)
	{
		//StaticMeshRenderer 따로 포인터를 받기.
		Pg::Data::SkinnedMeshRenderer* tSkinnedMeshRenderer = static_cast<Pg::Data::SkinnedMeshRenderer*>(GetBaseRenderer());

		//Mesh 데이터를 받기.
		auto tModelData = GraphicsResourceManager::Instance()->GetResource(tSkinnedMeshRenderer->GetMeshFilePath(), eAssetDefine::_3DMODEL);
		_modelData = static_cast<Asset3DModelData*>(tModelData.get());
	}

	RenderObjectSkinnedMesh3D::~RenderObjectSkinnedMesh3D()
	{
		delete _constantBufferStruct;
	}

	void RenderObjectSkinnedMesh3D::Render()
	{

	}

	void RenderObjectSkinnedMesh3D::BindBuffers()
	{

	}

	void RenderObjectSkinnedMesh3D::UpdateConstantBuffers(Pg::Data::CameraData* camData)
	{

	}

	void RenderObjectSkinnedMesh3D::BindConstantBuffers()
	{

	}

	void RenderObjectSkinnedMesh3D::UnbindConstantBuffers()
	{

	}

}