#include "RenderObjectStaticMesh3D.h"
#include "LowDX11Storage.h"
#include "MathHelper.h"
#include "GraphicsResourceHelper.h"
#include "GraphicsResourceManager.h"
#include "Asset3DModelData.h"
#include "LayoutDefine.h"

#include "../ParagonData/StaticMeshRenderer.h"

namespace Pg::Graphics
{
	using Pg::Graphics::Helper::MathHelper;
	using Pg::Graphics::Manager::GraphicsResourceManager;
	using Pg::Data::Enums::eAssetDefine;

	RenderObjectStaticMesh3D::RenderObjectStaticMesh3D(Pg::Data::BaseRenderer* baseRenderer) : RenderObject3D(baseRenderer)
	{
		//#ForwardTemp : GeometricPrimitive 인스턴스 만들기.
		//_tempPrimitive = DirectX::GeometricPrimitive::CreateSphere(LowDX11Storage::GetInstance()->_deviceContext, 1.0f);

		_constantBufferStruct = new ConstantBufferDefine::cbPerObjectBase;

		//Device / DevCon 받아오기.
		_device = LowDX11Storage::GetInstance()->_device;
		_devCon = LowDX11Storage::GetInstance()->_deviceContext;

		CreateVertexPixelShader();
		CreateSamplerState();
		CreateRasterizerState();
		CreateConstantBuffer();


		//StaticMeshRenderer 따로 포인터를 받기.
		Pg::Data::StaticMeshRenderer* tStaticMeshRenderer = static_cast<Pg::Data::StaticMeshRenderer*>(GetBaseRenderer());

		//Mesh 데이터를 받기.
		auto tModelData = GraphicsResourceManager::Instance()->GetResource(tStaticMeshRenderer->GetMeshFilePath(), eAssetDefine::_3DMODEL);
		_modelData = static_cast<Asset3DModelData*>(tModelData.get());
		
	}

	RenderObjectStaticMesh3D::~RenderObjectStaticMesh3D()
	{
		delete _constantBufferStruct;
	}

	void RenderObjectStaticMesh3D::Render(Pg::Data::CameraData* camData)
	{
		////렌더.
		//DirectX::XMFLOAT4X4 tWorldTM = MathHelper::PG2XM_FLOAT4X4(_baseRenderer->_object->_transform.GetWorldTM());
		//DirectX::XMMATRIX tWorldTMMat = DirectX::XMLoadFloat4x4(&tWorldTM);
		//
		//DirectX::XMFLOAT4X4 tViewTM = MathHelper::PG2XM_FLOAT4X4(camData->_viewMatrix);
		//DirectX::XMMATRIX tViewTMMat = DirectX::XMLoadFloat4x4(&tViewTM);
		//
		//DirectX::XMFLOAT4X4 tProjTM = MathHelper::PG2XM_FLOAT4X4(camData->_projMatrix);
		//DirectX::XMMATRIX tProjTMMat = DirectX::XMLoadFloat4x4(&tProjTM);
		//
		//_tempPrimitive->Draw(tWorldTMMat, tViewTMMat, tProjTMMat, DirectX::Colors::Crimson);

		//렌더하는 방식 : 한꺼번에 GPU에 많은 양의 메모리를 넣어놓는게 아니라,
		//DrawIndexed를 나눠서 호출하는 중에 SetResource를 다르게 사용.

		auto& tD3DBuffer = _modelData->_d3dBufferInfo;
		auto& tMatCluster = _modelData->_materialCluster;

		auto _DXStorage = LowDX11Storage::GetInstance();
		_DXStorage->_deviceContext->OMSetRenderTargets(1, &(_DXStorage->_mainRTV), (_DXStorage->_depthStencilView));

		//Input Layout 호출 / Primitive Topology 세팅.
		_devCon->IASetInputLayout(LayoutDefine::GetStatic1stLayout());
		_devCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		_devCon->RSSetState(_rasterizerState);
		//VS Bind.
		_devCon->VSSetShader(_vertexShader, nullptr, 0);

		//PS Bind.
		_devCon->PSSetShader(_pixelShader, nullptr, 0);
		//Sampler State Binding (PS)
		_devCon->PSSetSamplers(0, 1, &_samplerState);

		//Vertex Buffer Setting.
		UINT stride = sizeof(LayoutDefine::Vin1stStatic);
		UINT offset = 0;
		_devCon->IASetVertexBuffers(0, 1, &(_modelData->_d3dBufferInfo._vertexBuffer), &stride, &offset);
		//Index Buffer Setting.
		_devCon->IASetIndexBuffer(_modelData->_d3dBufferInfo._indexBuffer, DXGI_FORMAT_R32_UINT, 0);

		//Constant Buffer Binding (VS)
		UpdateConstantBuffer(camData);
		_devCon->VSSetConstantBuffers(0, 1, &_constantBuffer);
		//_devCon->DrawIndexed(_modelData->_d3dBufferInfo._totalIndexCount, 0, 0);
		//_devCon->DrawIndexed(36, 0, 0);

		//Update Shared Constant Buffer
		//UpdateConstantBuffer(camData);
		//_devCon->DrawIndexed(12, 0, 0);


		//_devCon->PSSetShaderResources(0, 1, &(tATS.texture));

		int tMeshCount = _modelData->_d3dBufferInfo._meshCount;
		for (int i = 0; i < tMeshCount; i++)
		{
			UINT tToDrawIndexCount = 0;

			if (i >= tMeshCount - 1)
			{
				//마지막.
				tToDrawIndexCount =
					_modelData->_d3dBufferInfo._totalIndexCount -
					_modelData->_d3dBufferInfo._indexOffsetVector[i];
			}
			else
			{
				tToDrawIndexCount =
					_modelData->_d3dBufferInfo._indexOffsetVector[i + 1] -
					_modelData->_d3dBufferInfo._indexOffsetVector[i];
			}

			UINT tMatID = _modelData->_d3dBufferInfo._materialIDVector[i];
			AssetTextureSRV tATS = _modelData->_materialCluster.GetMaterialATSByIndex(tMatID)[0];
			ID3D11ShaderResourceView* tTempDiffuseTexture = tATS.texture;
			assert(tTempDiffuseTexture != nullptr);

			//_devCon->PSSetShaderResources(0, 1, &_testSRV);
			_devCon->PSSetShaderResources(0, 1, &tTempDiffuseTexture);

			//업데이트된 다음에 호출된 해당 Mesh만큼 그린다.
			_devCon->DrawIndexed(tToDrawIndexCount,
				_modelData->_d3dBufferInfo._indexOffsetVector[i],
				_modelData->_d3dBufferInfo._vertexOffsetVector[i]);
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

		//VS/PS Unbind.
		_devCon->VSSetShader(nullptr, nullptr, 0);
		_devCon->PSSetShader(nullptr, nullptr, 0);

		//Constant Buffer 설정.

		//현재 Mesh가 어떤 Material을 가지고 있는지 확인해야. 
		//이에 따라 판단을 내려야 하기에.
		//나중에는 결국 Mesh Sorting 등등이 어느 정도 이루어져 있어야 할것. 
	}

	void RenderObjectStaticMesh3D::UpdateConstantBuffer(Pg::Data::CameraData* camData)
	{
		Pg::Math::PGFLOAT4X4 tWorldMat = GetBaseRenderer()->_object->_transform.GetWorldTM();
		DirectX::XMFLOAT4X4 tWorldMatFF = MathHelper::PG2XM_FLOAT4X4(tWorldMat);
		DirectX::XMMATRIX tWorldMatFFMat = DirectX::XMLoadFloat4x4(&tWorldMatFF);

		//0.01 스케일링 적용.
		tWorldMatFFMat = DirectX::XMMatrixMultiply(DirectX::XMMatrixScaling(0.01f, 0.01f, 0.01f), tWorldMatFFMat);

		DirectX::XMMATRIX tWorldInvTransposeMat = DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(nullptr, tWorldMatFFMat));

		//기반 Struct채우기.
		_constantBufferStruct->gCBuf_World = tWorldMatFFMat;
		_constantBufferStruct->gCBuf_WorldInvTranspose = tWorldInvTransposeMat;

		DirectX::XMFLOAT4X4 tView = MathHelper::PG2XM_FLOAT4X4(camData->_viewMatrix);
		DirectX::XMMATRIX tViewMat = DirectX::XMLoadFloat4x4(&tView);

		DirectX::XMFLOAT4X4 tProj = MathHelper::PG2XM_FLOAT4X4(camData->_projMatrix);
		DirectX::XMMATRIX tProjMat = DirectX::XMLoadFloat4x4(&tProj);

		DirectX::XMMATRIX tWVP = DirectX::XMMatrixMultiply(tWorldMatFFMat, DirectX::XMMatrixMultiply(tViewMat, tProjMat));
		_constantBufferStruct->gCBuf_WorldViewProj = tWVP;
		_constantBufferStruct->gCBuf_CameraPositionW = MathHelper::PG2XM_FLOAT3(camData->_position);

		//Updating Subresource
		_devCon->UpdateSubresource(_constantBuffer, 0, NULL, _constantBufferStruct, 0, 0);
	}

	void RenderObjectStaticMesh3D::CreateSamplerState()
	{
		D3D11_SAMPLER_DESC sd;
		sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sd.Filter = D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;
		sd.MipLODBias = 0.0f;
		sd.MaxAnisotropy = 1;

		_device->CreateSamplerState(&sd, &_samplerState);
	}

	void RenderObjectStaticMesh3D::CreateVertexPixelShader()
	{
		//TempForwardVS/PS 경로를 셋업한다.
		std::wstring tTempForwardVSPath = L"../Builds/x64/Debug/TempForwardVS.cso";
		std::wstring tTempForwardPSPath = L"../Builds/x64/Debug/TempForwardPS.cso";

		//TempForwardVS -> Vertex Shader 셋업.
		ID3DBlob* tVertexShaderByteCode = nullptr;
		D3DReadFileToBlob(tTempForwardVSPath.c_str(), &(tVertexShaderByteCode));
		_device->CreateVertexShader(tVertexShaderByteCode->GetBufferPointer(),
			tVertexShaderByteCode->GetBufferSize(), NULL, &_vertexShader);

		//TempForwardPS -> Vertex Shader 셋업.
		ID3DBlob* tPixelShaderByteCode = nullptr;
		D3DReadFileToBlob(tTempForwardPSPath.c_str(), &(tPixelShaderByteCode));
		_device->CreatePixelShader(tPixelShaderByteCode->GetBufferPointer(),
			tPixelShaderByteCode->GetBufferSize(), NULL, &_pixelShader);
	}

	void RenderObjectStaticMesh3D::CreateRasterizerState()
	{
		D3D11_RASTERIZER_DESC rd;
		//rd.FillMode = D3D11_FILL_WIREFRAME;
		rd.FillMode = D3D11_FILL_SOLID;
		rd.CullMode = D3D11_CULL_BACK;
		rd.FrontCounterClockwise = false;
		rd.DepthBias = 0;
		rd.SlopeScaledDepthBias = 0.0f;
		rd.DepthBiasClamp = 0.0f;
		rd.DepthClipEnable = true;
		rd.ScissorEnable = false;
		rd.MultisampleEnable = false;
		rd.AntialiasedLineEnable = false;

		_device->CreateRasterizerState(&rd, &_rasterizerState);
	}

	void RenderObjectStaticMesh3D::CreateConstantBuffer()
	{
		int sizeCB = (((sizeof(ConstantBufferDefine::cbPerObjectBase) - 1) / 16) + 1) * 16;	// declspec 으로 16바이트 정렬할 수 있다?

		D3D11_BUFFER_DESC tCBufferDesc;
		tCBufferDesc.ByteWidth = sizeCB; // 상수버퍼는 16바이트 정렬
		tCBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		tCBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		tCBufferDesc.CPUAccessFlags = 0;
		tCBufferDesc.MiscFlags = 0;

		_cbufferSubresourceData.pSysMem = _constantBufferStruct;

		_device->CreateBuffer(&tCBufferDesc, &_cbufferSubresourceData, &(_constantBuffer));
	}



}