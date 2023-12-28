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
		//버퍼 바인딩.
		BindBuffers();
		_DXStorage->_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		int tMeshCount = _modelData->_assetSceneData->_totalMeshCount;

		for (int i = 0; i < tMeshCount; i++)
		{
			//MultiMesh -> Material 적용할 수 있게 여기서도 Vector Clear.
			_textures.clear();

			UINT tToDrawIndexCount = _modelData->_assetSceneData->_meshList[i]._numIndices;
			UINT tMatID = _modelData->_assetSceneData->_meshList[i]._materialID;

			//이거 한번만 받아도 되겠지만, 일단은 통일성을 위해서.
			//아니면 업데이트되는 로직을 여기랑 연관? 후의 일.
			this->_diffuse = _modelData->GetMaterialByIndex(tMatID)->GetTextureByType(PG_TextureType_DIFFUSE);
			this->_normal = _modelData->GetMaterialByIndex(tMatID)->GetTextureByType(PG_TextureType_NORMALS);

			if (this->_diffuse == nullptr)
			{
				this->_diffuse = GraphicsResourceManager::Instance()->GetDefaultTexture(PG_TextureType_DIFFUSE);
			}
			if (this->_normal == nullptr)
			{
				this->_normal = GraphicsResourceManager::Instance()->GetDefaultTexture(PG_TextureType_NORMALS);
			}

			_textures.emplace_back(_diffuse);
			_textures.emplace_back(_normal);

			BindTextures();

			//업데이트된 다음에 호출된 해당 Mesh만큼 그린다.
			_devCon->DrawIndexed(tToDrawIndexCount,
				_modelData->_assetSceneData->_meshList[i]._indexOffset,
				_modelData->_assetSceneData->_meshList[i]._vertexOffset);
		}
	}

	void RenderObjectSkinnedMesh3D::BindBuffers()
	{
		//Vertex Buffer Setting.
		UINT stride = sizeof(LayoutDefine::Vin1stSkinned);
		UINT offset = 0;
		_devCon->IASetVertexBuffers(0, 1, &(_modelData->_vertexBuffer), &stride, &offset);

		//Index Buffer Setting.
		_devCon->IASetIndexBuffer(_modelData->_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
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

	void RenderObjectSkinnedMesh3D::UnbindBuffers()
	{

	}

}