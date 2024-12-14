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
		//SkinnedMesh�� �Ȱ��� ���� ���ľ�.

		//StaticMeshRenderer ���� �����͸� �ޱ�.
		Pg::Data::StaticMeshRenderer* tStaticMeshRenderer = static_cast<Pg::Data::StaticMeshRenderer*>(GetBaseRenderer());

		//Mesh �����͸� �ޱ�.
		auto tModelData = GraphicsResourceManager::Instance()->GetResource(tStaticMeshRenderer->GetMeshFilePath(), eAssetDefine::_3DMODEL);
		_modelData = static_cast<Asset3DModelData*>(tModelData.get());

		//Constant Buffer Data�� ����.
		_cbFirst = std::make_unique<ConstantBuffer<ConstantBufferDefine::cbPerObjectBase>>();

		//Skinned ������ ����, Alpha �뵵.
		this->_staticMeshRenderer = tStaticMeshRenderer;
	}

	RenderObjectStaticMesh3D::~RenderObjectStaticMesh3D()
	{

	}

	void RenderObjectStaticMesh3D::First_Render(const float* const dt)
	{
		int tMeshCount = _modelData->_assetSceneData->_totalMeshCount;

		for (int i = 0; i < tMeshCount; i++)
		{
			//MultiMesh -> Material ������ �� �ְ� ���⼭�� Vector Clear.
			UINT tToDrawIndexCount = _modelData->_assetSceneData->_meshList[i]._numIndices;

			//������Ʈ�� ������ ȣ��� �ش� Mesh��ŭ �׸���.
			_DXStorage->_deviceContext->DrawIndexed(tToDrawIndexCount,
				_modelData->_assetSceneData->_meshList[i]._indexOffset,
				_modelData->_assetSceneData->_meshList[i]._vertexOffset);
		}
	}

	void RenderObjectStaticMesh3D::First_UpdateConstantBuffers(Pg::Data::CameraData* camData)
	{
		auto _DXStorage = LowDX11Storage::GetInstance();

		// ������ۿ� �� �� ����
		
		//�ӽ÷� Translate���� �ٸ��� �����ϰ�, �ٽ� ���� �����Ѵ�.
		Pg::Math::PGFLOAT3 tOriginalPosValue = GetBaseRenderer()->_object->_transform._position;
		GetBaseRenderer()->_object->_transform._position = tOriginalPosValue + _rendererBase3DStorage->GetRendererOffset();

		DirectX::XMMATRIX tWorldTMMat = DirectX::XMMatrixIdentity();
		if (_rendererBase3DStorage->GetIsUseRotationOriginOffset())
		{
			//����, Parent�� ����� �Ѵ�.
			auto& tTrans = GetBaseRenderer()->_object->_transform;
			DirectX::XMMATRIX tScaleMat = DirectX::XMMatrixScaling(tTrans._scale.x, tTrans._scale.y, tTrans._scale.z);
			DirectX::XMMATRIX tOrigRotMat = Pg::Math::PG2XM_MATRIX4X4(Pg::Math::PGRotationMatrix(tTrans._rotation));
			DirectX::XMMATRIX tTransMat = Pg::Math::PG2XM_MATRIX4X4(Pg::Math::PGTranslateMatrix(tTrans._position));

			Pg::Math::PGFLOAT3 pivot = _rendererBase3DStorage->GetRendererRotationOriginOffset();
			DirectX::XMMATRIX toPivot = DirectX::XMMatrixTranslation(-pivot.x, -pivot.y, -pivot.z);
			DirectX::XMMATRIX fromPivot = DirectX::XMMatrixTranslation(pivot.x, pivot.y, pivot.z);

			using namespace DirectX;
			tWorldTMMat = tScaleMat * toPivot * tOrigRotMat * fromPivot * tTransMat;
		}
		else
		{
			DirectX::XMFLOAT4X4 tWorldTM = Helper::MathHelper::PG2XM_FLOAT4X4(GetBaseRenderer()->_object->_transform.GetWorldTM());
			tWorldTMMat = DirectX::XMLoadFloat4x4(&tWorldTM);
		}
		

		//�ٽ� Translate ���󺹱�.
		GetBaseRenderer()->_object->_transform._position = tOriginalPosValue;

		//0.01 �����ϸ� ����.
		tWorldTMMat = DirectX::XMMatrixMultiply(DirectX::XMMatrixScaling(0.01f, 0.01f, 0.01f), tWorldTMMat);
		//�����ϸ� ����� ������ InvTranspose�� ���������!
		DirectX::XMMATRIX tWorldInvTransposeMat = DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(nullptr, tWorldTMMat));

		_cbFirst->GetDataStruct()->gCBuf_World = tWorldTMMat;
		_cbFirst->GetDataStruct()->gCBuf_WorldInvTranspose = tWorldInvTransposeMat;
		_cbFirst->GetDataStruct()->gCBuf_ObjID = GetObjectID();
		_cbFirst->GetDataStruct()->gCBuf_MatID = GetMaterialID();
		//ù��° Constant Buffer���� �길 �־��ָ� �ȴ�.
		_cbFirst->Update();
	}

	void RenderObjectStaticMesh3D::First_BindBuffers()
	{
		_cbFirst->BindVS(0);
		_cbFirst->BindPS(0);

		// Albedo
		_DXStorage->_deviceContext->PSSetShaderResources(8, 1, &(_modelData->_pbrTextureArrays[0]->GetSRV()));
		// Normal
		_DXStorage->_deviceContext->PSSetShaderResources(9, 1, &(_modelData->_pbrTextureArrays[1]->GetSRV()));
		// ARM
		_DXStorage->_deviceContext->PSSetShaderResources(10, 1, &(_modelData->_pbrTextureArrays[2]->GetSRV()));

		// Alpha.
		//_DXStorage->_deviceContext->PSSetShaderResources(11, 1, &(_modelData->_pbrTextureArrays[3]->GetSRV()));

		BindMainVertexIndexBuffer();
	}

	void RenderObjectStaticMesh3D::First_UnbindBuffers()
	{
		_cbFirst->UnbindVS(0);
		_cbFirst->UnbindPS(0);

		ID3D11ShaderResourceView* tNullSRV = nullptr;
		// Albedo
		_DXStorage->_deviceContext->PSSetShaderResources(8, 1, &tNullSRV);
		// Normal
		_DXStorage->_deviceContext->PSSetShaderResources(9, 1, &tNullSRV);
		// ARM
		_DXStorage->_deviceContext->PSSetShaderResources(10, 1, &tNullSRV);
	}

	void RenderObjectStaticMesh3D::BindMainVertexIndexBuffer()
	{
		//Vertex Buffer Setting.
		UINT stride[2] = { sizeof(LayoutDefine::Vin1stStatic_Individual), sizeof(LayoutDefine::Vin2ndAll_Individual) };
		UINT offset[2] = { 0,0 };
		ID3D11Buffer* buffers[2] = { _modelData->_vertexBuffer, _modelData->_secondVertexBuffer };
		_DXStorage->_deviceContext->IASetVertexBuffers(0, 2, buffers, stride, offset);
		//Index Buffer Setting.
		_DXStorage->_deviceContext->IASetIndexBuffer(_modelData->_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	}
}