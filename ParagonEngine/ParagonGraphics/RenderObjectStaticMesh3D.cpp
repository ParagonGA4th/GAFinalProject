#include "RenderObjectStaticMesh3D.h"

#include "GraphicsResourceHelper.h"
#include "GraphicsResourceManager.h"
#include "LowDX11Storage.h"
#include "../ParagonData/StaticMeshRenderer.h"
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

	RenderObjectStaticMesh3D::RenderObjectStaticMesh3D(Pg::Data::BaseRenderer* baseRenderer) : RenderObject3D(baseRenderer)
	{
		//StaticMeshRenderer 따로 포인터를 받기.
		Pg::Data::StaticMeshRenderer* tStaticMeshRenderer = static_cast<Pg::Data::StaticMeshRenderer*>(GetBaseRenderer());

		//Mesh 데이터를 받기.
		auto tModelData = GraphicsResourceManager::Instance()->GetResource(tStaticMeshRenderer->GetMeshFilePath(), eAssetDefine::_3DMODEL);
		_modelData = static_cast<Asset3DModelData*>(tModelData.get());
	}

	RenderObjectStaticMesh3D::~RenderObjectStaticMesh3D()
	{
		
	}

	void RenderObjectStaticMesh3D::Render()
	{
		BindBuffers();
		_DXStorage->_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		int tMeshCount = _modelData->_assetSceneData->_totalMeshCount;

		for (int i = 0; i < tMeshCount; i++)
		{
			//MultiMesh -> Material 적용할 수 있게 여기서도 Vector Clear.
			ClearTextureArray();

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

			AddTextureToArray(_diffuse);
			AddTextureToArray(_normal);

			BindTextureArray();

			//업데이트된 다음에 호출된 해당 Mesh만큼 그린다.
			_DXStorage->_deviceContext->DrawIndexed(tToDrawIndexCount,
				_modelData->_assetSceneData->_meshList[i]._indexOffset,
				_modelData->_assetSceneData->_meshList[i]._vertexOffset);
		}
	}

	void RenderObjectStaticMesh3D::BindBuffers()
	{
		///
		//Vertex Buffer Setting.
		UINT stride = sizeof(LayoutDefine::Vin1stStatic);
		UINT offset = 0;
		_DXStorage->_deviceContext->IASetVertexBuffers(0, 1, &(_modelData->_vertexBuffer), &stride, &offset);
		//Index Buffer Setting.
		_DXStorage->_deviceContext->IASetIndexBuffer(_modelData->_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	}

	void RenderObjectStaticMesh3D::UpdateConstantBuffers(Pg::Data::CameraData* camData)
	{
		auto _DXStorage = LowDX11Storage::GetInstance();

		//auto& tD3DBuffer = _modelData->_d3dBufferInfo;
		//auto& tMatCluster = _modelData->_materialCluster;

		// 상수버퍼에 들어갈 값 셋팅
		DirectX::XMFLOAT4X4 tWorldTM = Helper::MathHelper::PG2XM_FLOAT4X4(GetBaseRenderer()->_object->_transform.GetWorldTM());
		DirectX::XMMATRIX tWorldTMMat = DirectX::XMLoadFloat4x4(&tWorldTM);

		DirectX::XMMATRIX tWorldInvTransposeMat = DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(nullptr, tWorldTMMat));

		//0.01 스케일링 적용.
		tWorldTMMat = DirectX::XMMatrixMultiply(DirectX::XMMatrixScaling(0.01f, 0.01f, 0.01f), tWorldTMMat);

		DirectX::XMFLOAT4X4 tViewTM = Helper::MathHelper::PG2XM_FLOAT4X4(camData->_viewMatrix);
		DirectX::XMMATRIX tViewTMMat = DirectX::XMLoadFloat4x4(&tViewTM);

		DirectX::XMFLOAT4X4 tProjTM = Helper::MathHelper::PG2XM_FLOAT4X4(camData->_projMatrix);
		DirectX::XMMATRIX tProjTMMat = DirectX::XMLoadFloat4x4(&tProjTM);

		DirectX::XMFLOAT3 tCameraPositionW = Helper::MathHelper::PG2XM_FLOAT3(camData->_position);
		DirectX::XMVECTOR tCameraPositionVec = DirectX::XMLoadFloat3(&tCameraPositionW);
		DirectX::XMMATRIX tCameraPositionMat = DirectX::XMMatrixTranslationFromVector(tCameraPositionVec);

		float tCamDistance = 0.0f;
		DirectX::XMStoreFloat(&tCamDistance, DirectX::XMVector3Length(tCameraPositionVec));

		_constantBufferStruct->gCBuf_World = tWorldTMMat;
		_constantBufferStruct->gCBuf_WorldInvTranspose = tWorldInvTransposeMat;
		_constantBufferStruct->gCBuf_WorldView = tViewTMMat;
		_constantBufferStruct->gCBuf_WorldViewProj = DirectX::XMMatrixMultiply(tWorldTMMat, DirectX::XMMatrixMultiply(tViewTMMat, tProjTMMat));
		_constantBufferStruct->gCBuf_CameraPositionW = tCameraPositionW;

		// Bind Constant Buffers
		for (int i = 0; i < _constantBuffers.size(); ++i)
		{
			_constantBuffers[i]->Update(i);
		}
	}

	void RenderObjectStaticMesh3D::BindBuffers()
	{
		for (int i = 0; i < _constantBuffers.size(); ++i)
		{
			_constantBuffers[i]->Bind(i);
		}
	}

	void RenderObjectStaticMesh3D::UnbindBuffers()
	{
		for (int i = 0; i < _constantBuffers.size(); ++i)
		{
			_constantBuffers[i]->Unbind(i);
		}
	}

}