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
		
		//_normal = new RenderTexture2D(Pg::Data::Enums::eAssetDefine::_2DTEXTURE, "../Resources/Textures/tw_normal.png");
		//HRESULT hr = DirectX::CreateWICTextureFromFile(_DXStorage->_device, _normal->GetFilePath().c_str(), &_normal->GetResource(), &_normal->GetSRV());
		//
		//_diffuse = new RenderTexture2D(Pg::Data::Enums::eAssetDefine::_2DTEXTURE, "../Resources/Textures/tw_diffuse.png");
		//hr = DirectX::CreateWICTextureFromFile(_DXStorage->_device, _normal->GetFilePath().c_str(), &_normal->GetResource(), &_normal->GetSRV());
	}

	RenderObjectStaticMesh3D::~RenderObjectStaticMesh3D()
	{
		delete _constantBufferStruct;
	}

	void RenderObjectStaticMesh3D::Render()
	{
		_textures.clear();

		BindBuffers();
		_DXStorage->_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		int tMeshCount = _modelData->_assetSceneData->_totalMeshCount;

		for (int i = 0; i < tMeshCount; i++)
		{
			UINT tToDrawIndexCount = _modelData->_assetSceneData->_meshList[i]._numIndices;
			UINT tMatID = _modelData->_assetSceneData->_meshList[i]._materialID;

			//이거 한번만 받아도 되겠지만, 일단은 통일성을 위해서.
			//아니면 업데이트되는 로직을 여기랑 연관? 후의 일.
			this->_diffuse = _modelData->GetMaterialByIndex(tMatID)->GetTextureByType(PG_TextureType_DIFFUSE);
			this->_normal =  _modelData->GetMaterialByIndex(tMatID)->GetTextureByType(PG_TextureType_NORMALS);
			assert(this->_diffuse != nullptr);
			assert(this->_normal != nullptr);

			_textures.emplace_back(_diffuse);
			_textures.emplace_back(_normal);

			BindTextures();

			//업데이트된 다음에 호출된 해당 Mesh만큼 그린다.
			_devCon->DrawIndexed(tToDrawIndexCount,
				_modelData->_assetSceneData->_meshList[i]._indexOffset,
				_modelData->_assetSceneData->_meshList[i]._vertexOffset);
		}

		/*
		분석도 분석인데, 지금은 Node별로 Mesh의 Local Transformation이 반영되지 않기 때문에, 당연히 버텍스 버퍼가 한 공간에 겹쳐서 출력된다. 이를 고쳐야 한다..
		이와 더불어, 쓸데없는 데이터는 통합하는 것도 좋다!
		지금 오버헤드를 줄여서, 값을 처리하는 것이 중요.
		또한, → 현재 부모 노드의 행렬을 요상하게 처리하고 있기 때문에 (Transpose 등등.. Tofu/ModelViewer, 이 역시 손을 봐줘야 한다!
		*/

		//이제 실제로 그리고 / Texture를 바꿔끼는 방식이 들어가야 한다.
		//바뀌는 SRV를 반영해야 한다. -> MaterialCluster와 D3DBufferInfo를 활용해야 한다.
		// SRV + 바뀌는 인덱스 올바르게 반영해야 한다.

		//Multi-Material으로 렌더. 목표해서 되어야 하는 방식.
		//MultiMaterialDraw();
		//SingleMaterialDraw();
		//SingleMaterialMultiMeshDraw();

		//현재 Mesh가 어떤 Material을 가지고 있는지 확인해야. 
		//이에 따라 판단을 내려야 하기에.
		//나중에는 결국 Mesh Sorting 등등이 어느 정도 이루어져 있어야 할것. 
	}

	void RenderObjectStaticMesh3D::BindBuffers()
	{
		///
		//Vertex Buffer Setting.
		UINT stride = sizeof(LayoutDefine::Vin1stStatic);
		UINT offset = 0;
		_devCon->IASetVertexBuffers(0, 1, &(_modelData->_vertexBuffer), &stride, &offset);
		//Index Buffer Setting.
		_devCon->IASetIndexBuffer(_modelData->_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
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

	void RenderObjectStaticMesh3D::BindConstantBuffers()
	{
		for (int i = 0; i < _constantBuffers.size(); ++i)
		{
			_constantBuffers[i]->Bind(i);
		}
	}

	void RenderObjectStaticMesh3D::UnbindConstantBuffers()
	{
		for (int i = 0; i < _constantBuffers.size(); ++i)
		{
			_constantBuffers[i]->Unbind(i);
		}
	}

}