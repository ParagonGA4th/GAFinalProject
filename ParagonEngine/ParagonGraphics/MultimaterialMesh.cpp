#include "MultimaterialMesh.h"
#include "Asset3DModelData.h"
#include "GraphicsResourceManager.h"
#include "LayoutDefine.h"
#include "LowDX11Storage.h"
#include "MathHelper.h"
#include "AssetTextureType.h"

#include "../ParagonData/GraphicsResource.h"
#include "../ParagonData/AssetDefines.h"
#include "../ParagonData/CameraData.h"

#include <dxtk/DDSTextureLoader.h>

namespace Pg::Graphics
{
	using Pg::Graphics::Manager::GraphicsResourceManager;
	using Pg::Data::Enums::eAssetDefine;

	MultimaterialMesh::MultimaterialMesh()
	{
		_constantBufferStruct = new ConstantBufferDefine::cbPerObjectBase;

		//Device / DevCon 받아오기.
		_device = LowDX11Storage::GetInstance()->_device;
		_devCon = LowDX11Storage::GetInstance()->_deviceContext;

		//고정된 File Path ( == AssetManager에서 이미 로딩된 경로가 있어야 작동하므로, 하드코딩했음.)
		_filePath = "../Resources/3DModels/TexturedMultiCubes/TexturedMultiCubeMultiMesh.fbx";
		//_filePath = "../Resources/3DModels/Banana.fbx";

		auto tModelData = GraphicsResourceManager::Instance()->GetResource(_filePath, eAssetDefine::_3DMODEL);
		_modelData = static_cast<Asset3DModelData*>(tModelData.get());

		Initialize();

		//테스팅을 위해서, SRV를 명시적으로 만들어서 테스트.
		HRESULT hr = DirectX::CreateDDSTextureFromFile(_device, L"../Resources/Textures/DummyData/EditorCamDummy.dds",
			&_testResource, &_testSRV);
	}

	MultimaterialMesh::~MultimaterialMesh()
	{
		delete _constantBufferStruct;
	}

	void MultimaterialMesh::Initialize()
	{
		CreateVertexPixelShader();
		CreateSamplerState();
		CreateRasterizerState();
		CreateConstantBuffer();
		SetupMatrices();
	}

	void MultimaterialMesh::Draw(Pg::Data::CameraData* camData)
	{
		//렌더하는 방식 : 한꺼번에 GPU에 많은 양의 메모리를 넣어놓는게 아니라,
		//DrawIndexed를 나눠서 호출하는 중에 SetResource를 다르게 사용.

		auto& tD3DBuffer = _modelData->_d3dBufferInfo;
		auto& tMatCluster = _modelData->_materialCluster;

		//Input Layout 호출 / Primitive Topology 세팅.
		_devCon->IASetInputLayout(LayoutDefine::GetStatic1stLayout());
		_devCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//VS Bind.
		_devCon->VSSetShader(_vertexShader, nullptr, 0);
		//Update Shared Constant Buffer
		UpdateConstantBuffer(camData);
		//Constant Buffer Binding (VS)
		_devCon->VSSetConstantBuffers(0, 1, &_constantBuffer);
		//PS Bind.
		_devCon->PSSetShader(_pixelShader, nullptr, 0);
		//Constant Buffer Binding (PS)
		_devCon->PSSetConstantBuffers(0, 1, &_constantBuffer);
		//Sampler State Binding (PS)
		_devCon->PSSetSamplers(0, 1, &_samplerState);

		//Vertex Buffer Setting.
		UINT stride = sizeof(LayoutDefine::Vin1stStatic);
		UINT offset = 0;
		_devCon->IASetVertexBuffers(0, 1, &(_modelData->_d3dBufferInfo._vertexBuffer), &stride, &offset);
		//Index Buffer Setting.
		_devCon->IASetIndexBuffer(_modelData->_d3dBufferInfo._indexBuffer, DXGI_FORMAT_R16_UINT, 0);

		//이제 실제로 그리고 / Texture를 바꿔끼는 방식이 들어가야 한다.
		//바뀌는 SRV를 반영해야 한다. -> MaterialCluster와 D3DBufferInfo를 활용해야 한다.
		// SRV + 바뀌는 인덱스 올바르게 반영해야 한다.

		//Multi-Material으로 렌더. 목표해서 되어야 하는 방식.
		MultiMaterialDraw();
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

	void MultimaterialMesh::CreateSamplerState()
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

	void MultimaterialMesh::CreateVertexPixelShader()
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

	void MultimaterialMesh::CreateRasterizerState()
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

	void MultimaterialMesh::UpdateConstantBuffer(Pg::Data::CameraData* camData)
	{
		using Pg::Graphics::Helper::MathHelper;

		//기반 Struct채우기.
		_constantBufferStruct->gCBuf_World = _worldMat;
		_constantBufferStruct->gCBuf_WorldInvTranspose = _worldInvTransposeMat;

		DirectX::XMFLOAT4X4 tView = MathHelper::PG2XM_FLOAT4X4(camData->_viewMatrix);
		DirectX::XMMATRIX tViewMat = DirectX::XMLoadFloat4x4(&tView);

		DirectX::XMFLOAT4X4 tProj = MathHelper::PG2XM_FLOAT4X4(camData->_projMatrix);
		DirectX::XMMATRIX tProjMat = DirectX::XMLoadFloat4x4(&tProj);

		DirectX::XMMATRIX tWVP = DirectX::XMMatrixMultiply(DirectX::XMMatrixMultiply(_worldMat, tViewMat), tProjMat);
		_constantBufferStruct->gCBuf_WorldViewProj = tWVP;
		_constantBufferStruct->gCBuf_CameraPositionW = MathHelper::PG2XM_FLOAT3(camData->_position);

		//Updating Subresource
		_devCon->UpdateSubresource(_constantBuffer, 0, NULL, _constantBufferStruct, 0, 0);
	}

	void MultimaterialMesh::SetupMatrices()
	{
		DirectX::XMFLOAT3 tPosition = { 0.0f, 3.0f, 0.0f };
		DirectX::XMVECTOR tPosVec = DirectX::XMLoadFloat3(&tPosition);

		DirectX::XMFLOAT4 tRotQuat = { 0.0f, 0.0f, 0.0f, 0.0f };
		DirectX::XMVECTOR tRotQuatVec = DirectX::XMLoadFloat4(&tRotQuat);

		DirectX::XMFLOAT3 tScale = { 1.0f, 1.0f, 1.0f };
		DirectX::XMVECTOR tScaleVec = DirectX::XMLoadFloat3(&tScale);

		_worldMat = DirectX::XMMatrixAffineTransformation(tScaleVec, tPosVec, tRotQuatVec, tPosVec);
		_worldInvTransposeMat = DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(nullptr, _worldMat));
	}

	void MultimaterialMesh::CreateConstantBuffer()
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

	void MultimaterialMesh::MultiMaterialDraw()
	{
		int tMeshCount = _modelData->_d3dBufferInfo._meshCount;
		for (int i = 0; i < tMeshCount; i++)
		{
			UINT tToDrawIndexCount = 0;

			if (i >= tMeshCount - 1)
			{
				//마지막.
				tToDrawIndexCount =
					_modelData->_d3dBufferInfo._indexCount -
					_modelData->_d3dBufferInfo._indexOffsetVector[i];
			}
			else
			{
				tToDrawIndexCount =
					_modelData->_d3dBufferInfo._indexOffsetVector[i + 1] -
					_modelData->_d3dBufferInfo._indexOffsetVector[i];
			}

			//SRV 업데이트.
			UINT tMatID = _modelData->_d3dBufferInfo._materialIDVector[i];
			//AssetTextureSRV tATS = _modelData->_materialCluster.GetMaterialATSByIndex(tMatID)[0];
			//_devCon->PSSetShaderResources(0, 1, &(tATS.texture));

			_devCon->PSSetShaderResources(0, 1, &_testSRV);
			_devCon->PSSetSamplers(0, 1, &_samplerState);

			////업데이트된 다음에 호출된 해당 Mesh만큼 그린다.
			_devCon->DrawIndexed(tToDrawIndexCount,
				_modelData->_d3dBufferInfo._indexOffsetVector[i],
				_modelData->_d3dBufferInfo._vertexOffsetVector[i]);
		}
	}

	void MultimaterialMesh::SingleMaterialDraw()
	{
		int i = 0;
		UINT tMatID = _modelData->_d3dBufferInfo._materialIDVector[i];
		AssetTextureSRV tATS = _modelData->_materialCluster.GetMaterialATSByIndex(tMatID)[0];
		//_devCon->PSSetShaderResources(0, 1, &(tATS.texture));// SRV가 설정되지 않는다.
		_devCon->PSSetShaderResources(0, 1, &_testSRV);
		_devCon->PSSetSamplers(0, 1, &_samplerState);

		_devCon->DrawIndexed(_modelData->_d3dBufferInfo._indexCount, 0, 0);
	}

	void MultimaterialMesh::SingleMaterialMultiMeshDraw()
	{
		//int tMeshCount = _modelData->_d3dBufferInfo._meshCount;
		//for (int i = 0; i < tMeshCount; i++)
		//{
		//	UINT tToDrawIndexCount = 0;
		//
		//	if (i >= tMeshCount - 1)
		//	{
		//		//마지막.
		//		tToDrawIndexCount =
		//			_modelData->_d3dBufferInfo._indexCount -
		//			_modelData->_d3dBufferInfo._indexOffsetVector[i];
		//	}
		//	else
		//	{
		//		tToDrawIndexCount =
		//			_modelData->_d3dBufferInfo._indexOffsetVector[i + 1] -
		//			_modelData->_d3dBufferInfo._indexOffsetVector[i];
		//	}
		//
		//	//SRV 업데이트.
		//	//UINT tMatID = _modelData->_d3dBufferInfo._materialIDVector[i];
		//	//AssetTextureSRV tATS = _modelData->_materialCluster.GetMaterialATSByIndex(tMatID)[0];
		//	_devCon->PSSetShaderResources(0, 1, &_testSRV);
		//	_devCon->PSSetSamplers(0, 1, &_samplerState);
		//
		//	////업데이트된 다음에 호출된 해당 Mesh만큼 그린다.
		//	_devCon->DrawIndexed(tToDrawIndexCount,
		//		_modelData->_d3dBufferInfo._indexOffsetVector[i],
		//		_modelData->_d3dBufferInfo._vertexOffsetVector[i]);
		//}
	}

}
