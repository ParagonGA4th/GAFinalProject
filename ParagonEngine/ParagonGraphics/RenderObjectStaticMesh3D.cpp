#include "RenderObjectStaticMesh3D.h"

#include "GraphicsResourceHelper.h"
#include "GraphicsResourceManager.h"
#include "LowDX11Storage.h"
#include "../ParagonData/StaticMeshRenderer.h"
#include "LayoutDefine.h"
#include "AssetBasic3DLoader.h"
#include "MathHelper.h"
#include "RenderTexture2D.h"
#include "AssetModelDataDefine.h"
#include "AssetTextureType.h"
#include "MaterialCluster.h"

#include "dxtk/WICTextureLoader.h"
#include <algorithm>

namespace Pg::Graphics
{
	using Pg::Graphics::Helper::MathHelper;
	using Pg::Graphics::Manager::GraphicsResourceManager;
	using Pg::Data::Enums::eAssetDefine;

	RenderObjectStaticMesh3D::RenderObjectStaticMesh3D(Pg::Data::BaseRenderer* baseRenderer, unsigned int objID) : RenderObject3D(baseRenderer, objID)
	{
		//SkinnedMesh도 똑같은 과정 거쳐야.

		//StaticMeshRenderer 따로 포인터를 받기.
		Pg::Data::StaticMeshRenderer* tStaticMeshRenderer = static_cast<Pg::Data::StaticMeshRenderer*>(GetBaseRenderer());

		//Mesh 데이터를 받기.
		auto tModelData = GraphicsResourceManager::Instance()->GetResource(tStaticMeshRenderer->GetMeshFilePath(), eAssetDefine::_3DMODEL);
		_modelData = static_cast<Asset3DModelData*>(tModelData.get());

		//Constant Buffer Data를 생성.
		_cbFirst = std::make_unique<ConstantBuffer<ConstantBufferDefine::cbPerObjectBase>>();
	}

	RenderObjectStaticMesh3D::~RenderObjectStaticMesh3D()
	{

	}

	void RenderObjectStaticMesh3D::CreateObjMatBuffers()
	{
		//VB 로드. *(Index Buffer는 공유)
		GraphicsResourceManager::Instance()->GetBasic3DLoader()->LoadObjMatBufferStatic(_3rdVB, _modelData, _objectID, GetMaterialID());
		
		//Constant Buffer Data를 생성.
		_cbObjMat = std::make_unique<ConstantBuffer<ConstantBufferDefine::cbPerObjMatBase>>();
	}

	void RenderObjectStaticMesh3D::First_Render(const float* const dt)
	{
		BindMainVertexIndexBuffer();

		int tMeshCount = _modelData->_assetSceneData->_totalMeshCount;

		for (int i = 0; i < tMeshCount; i++)
		{
			//MultiMesh -> Material 적용할 수 있게 여기서도 Vector Clear.
			UINT tToDrawIndexCount = _modelData->_assetSceneData->_meshList[i]._numIndices;

			//업데이트된 다음에 호출된 해당 Mesh만큼 그린다.
			_DXStorage->_deviceContext->DrawIndexed(tToDrawIndexCount,
				_modelData->_assetSceneData->_meshList[i]._indexOffset,
				_modelData->_assetSceneData->_meshList[i]._vertexOffset);
		}
	}

	void RenderObjectStaticMesh3D::First_UpdateConstantBuffers(Pg::Data::CameraData* camData)
	{
		auto _DXStorage = LowDX11Storage::GetInstance();

		// 상수버퍼에 들어갈 값 셋팅
		DirectX::XMFLOAT4X4 tWorldTM = Helper::MathHelper::PG2XM_FLOAT4X4(GetBaseRenderer()->_object->_transform.GetWorldTM());
		DirectX::XMMATRIX tWorldTMMat = DirectX::XMLoadFloat4x4(&tWorldTM);

		//0.01 스케일링 적용.
		tWorldTMMat = DirectX::XMMatrixMultiply(DirectX::XMMatrixScaling(0.01f, 0.01f, 0.01f), tWorldTMMat);
		//스케일링 적용된 다음에 InvTranspose를 구해줘야지!
		DirectX::XMMATRIX tWorldInvTransposeMat = DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(nullptr, tWorldTMMat));

		_cbFirst->GetDataStruct()->gCBuf_World = tWorldTMMat;
		_cbFirst->GetDataStruct()->gCBuf_WorldInvTranspose = tWorldInvTransposeMat;

		//첫번째 Constant Buffer에는 얘만 넣어주면 된다.
		_cbFirst->Update();
	}

	void RenderObjectStaticMesh3D::First_BindBuffers()
	{
		_cbFirst->BindVS(0);
	}

	void RenderObjectStaticMesh3D::First_UnbindBuffers()
	{
		_cbFirst->UnbindVS(0);
	}

	void RenderObjectStaticMesh3D::ObjMat_UpdateConstantBuffers(Pg::Data::CameraData* camData)
	{
		auto _DXStorage = LowDX11Storage::GetInstance();

		// 상수버퍼에 들어갈 값 셋팅
		DirectX::XMFLOAT4X4 tWorldTM = Helper::MathHelper::PG2XM_FLOAT4X4(GetBaseRenderer()->_object->_transform.GetWorldTM());
		DirectX::XMMATRIX tWorldTMMat = DirectX::XMLoadFloat4x4(&tWorldTM);

		//0.01 스케일링 적용.
		tWorldTMMat = DirectX::XMMatrixMultiply(DirectX::XMMatrixScaling(0.01f, 0.01f, 0.01f), tWorldTMMat);

		_cbObjMat->GetDataStruct()->gCBuf_World = tWorldTMMat;	
		// _cbObjMat 업데이트.
		_cbObjMat->Update();
	}

	void RenderObjectStaticMesh3D::ObjMat_BindBuffers()
	{
		_cbObjMat->BindVS(0);

		// PixelShader : 이제 Albedo / Normal / Specular / Arm 데이터를 넣어줘야 한다.
		// 디폴트 매터리얼 상관하지 않고, 모든 오브젝트가 값 자체는 이제 필요하게 될 것이라는 말이다. Texture 투입.
		// 그냥 예전방식대로, Texture2DArray 자체를 투입할 것.
		// 나중에는 같은 오브젝트 + 인스턴싱의 영향을 받는다면 해당 스텝을 누락하던가, 

		// Albedo
		_DXStorage->_deviceContext->PSSetShaderResources(8, 1, &(_modelData->_pbrTextureArrays[0]->GetSRV()));
		// Normal
		_DXStorage->_deviceContext->PSSetShaderResources(9, 1, &(_modelData->_pbrTextureArrays[1]->GetSRV()));
		// Specular
		_DXStorage->_deviceContext->PSSetShaderResources(10, 1, &(_modelData->_pbrTextureArrays[2]->GetSRV()));
		// ARM
		_DXStorage->_deviceContext->PSSetShaderResources(11, 1, &(_modelData->_pbrTextureArrays[3]->GetSRV()));
	}

	void RenderObjectStaticMesh3D::ObjMat_Render(const float* const dt)
	{
		BindObjMatVertexIndexBuffer();

		int tMeshCount = _modelData->_assetSceneData->_totalMeshCount;

		for (int i = 0; i < tMeshCount; i++)
		{
			//MultiMesh -> Material 적용할 수 있게 여기서도 Vector Clear.
			UINT tToDrawIndexCount = _modelData->_assetSceneData->_meshList[i]._numIndices;

			//업데이트된 다음에 호출된 해당 Mesh만큼 그린다.
			_DXStorage->_deviceContext->DrawIndexed(tToDrawIndexCount,
				_modelData->_assetSceneData->_meshList[i]._indexOffset,
				_modelData->_assetSceneData->_meshList[i]._vertexOffset);
		}
	}

	void RenderObjectStaticMesh3D::ObjMat_UnbindBuffers()
	{
		_cbObjMat->UnbindVS(0);

		//PBR Texture를 다 썼으니, 이제 할당 해제!
		ID3D11ShaderResourceView* tNullSRV = nullptr;
		// PBR Texture Arrays To NULL
		_DXStorage->_deviceContext->PSSetShaderResources(8, 1, &(tNullSRV));
		_DXStorage->_deviceContext->PSSetShaderResources(9, 1, &(tNullSRV));
		_DXStorage->_deviceContext->PSSetShaderResources(10, 1, &(tNullSRV));
		_DXStorage->_deviceContext->PSSetShaderResources(11, 1, &(tNullSRV));

	}

	void RenderObjectStaticMesh3D::BindMainVertexIndexBuffer()
	{
		///
		//Vertex Buffer Setting.
		UINT stride = sizeof(LayoutDefine::Vin1stStatic_Individual);
		UINT offset = 0;
		_DXStorage->_deviceContext->IASetVertexBuffers(0, 1, &(_modelData->_vertexBuffer), &stride, &offset);
		//Index Buffer Setting.
		_DXStorage->_deviceContext->IASetIndexBuffer(_modelData->_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	}

	void RenderObjectStaticMesh3D::BindObjMatVertexIndexBuffer()
	{
		//Vertex Buffer Setting.
		UINT stride = sizeof(LayoutDefine::Vin3rdStaticSkinned_Individual);
		UINT offset = 0;
		_DXStorage->_deviceContext->IASetVertexBuffers(0, 1, &(_3rdVB), &stride, &offset);
		//Index Buffer Setting. (Model Data와 공유)
		_DXStorage->_deviceContext->IASetIndexBuffer(_modelData->_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	}

	

}