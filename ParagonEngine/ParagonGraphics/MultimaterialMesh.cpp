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
#include <dxtk/WICTextureLoader.h>

//ASSIMP로, 리소스 관리 로직이랑 별개 관리 예정.
#include <assimp/Importer.hpp>     
#include <assimp/scene.h>          
#include <assimp/postprocess.h> 
#include <cassert> 


namespace Pg::Graphics
{
	using namespace tofu;

	using Pg::Graphics::Manager::GraphicsResourceManager;
	using Pg::Data::Enums::eAssetDefine;

	//static 
	MultimaterialMesh::MultimaterialMesh(const std::string& filePath)
	{
		_constantBufferStruct = new ConstantBufferDefine::cbPerObjectBase;

		//Device / DevCon 받아오기.
		_device = LowDX11Storage::GetInstance()->_device;
		_devCon = LowDX11Storage::GetInstance()->_deviceContext;

		CreateVertexPixelShader();
		CreateSamplerState();
		CreateRasterizerState();
		CreateConstantBuffer();
		LoadHardCodedSRVs();

		ImportSkinnedAsset(filePath);
	}

	MultimaterialMesh::~MultimaterialMesh()
	{
		delete _constantBufferStruct;
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
		//std::wstring tTempForwardVSPath = L"../Builds/x64/Debug/TempForwardVS.cso";
		std::wstring tTempForwardVSPath = L"../Builds/x64/Debug/TempForwardVSSkinned.cso";
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

	void MultimaterialMesh::UpdateConstantBuffer(Pg::Data::CameraData* camData, DirectX::XMFLOAT4X4 worldMat)
	{
		using Pg::Graphics::Helper::MathHelper;

		//기반 Struct채우기.
		DirectX::XMMATRIX tWorldMat = DirectX::XMLoadFloat4x4(&worldMat);
		DirectX::XMMATRIX tWorldInvTransposeMat = DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(nullptr, tWorldMat));

		_constantBufferStruct->gCBuf_World = tWorldMat;
		_constantBufferStruct->gCBuf_WorldInvTranspose = tWorldInvTransposeMat;

		DirectX::XMFLOAT4X4 tView = MathHelper::PG2XM_FLOAT4X4(camData->_viewMatrix);
		DirectX::XMMATRIX tViewMat = DirectX::XMLoadFloat4x4(&tView);

		DirectX::XMFLOAT4X4 tProj = MathHelper::PG2XM_FLOAT4X4(camData->_projMatrix);
		DirectX::XMMATRIX tProjMat = DirectX::XMLoadFloat4x4(&tProj);

		DirectX::XMMATRIX tWVP = DirectX::XMMatrixMultiply(tWorldMat, DirectX::XMMatrixMultiply(tViewMat, tProjMat));
		_constantBufferStruct->gCBuf_WorldViewProj = tWVP;
		_constantBufferStruct->gCBuf_CameraPositionW = MathHelper::PG2XM_FLOAT3(camData->_position);

		//Updating Subresource (Map / UnMap으로 대체됨!)
	}

	void MultimaterialMesh::CreateConstantBuffer()
	{
		//int sizeCB = (((sizeof(ConstantBufferDefine::cbPerObjectBase) - 1) / 16) + 1) * 16;	// declspec 으로 16바이트 정렬할 수 있다?
		int sizeCB = (((sizeof(ConstantBufferDefine::cbPerObjectBase) - 1) / 16) + 1) * 16;	// declspec 으로 16바이트 정렬할 수 있다?

		D3D11_BUFFER_DESC tCBufferDesc;
		tCBufferDesc.ByteWidth = sizeCB; // 상수버퍼는 16바이트 정렬
		tCBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		tCBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		tCBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		tCBufferDesc.MiscFlags = 0;

		/*CD3D11_BUFFER_DESC cbDesc(
			sizeof(float) * 16,
			D3D11_BIND_CONSTANT_BUFFER,
			D3D11_USAGE_DYNAMIC,
			D3D11_CPU_ACCESS_WRITE);*/

		_cbufferSubresourceData.pSysMem = _constantBufferStruct;

		HR(_device->CreateBuffer(&tCBufferDesc, &_cbufferSubresourceData, &(_constantBuffer)));
	}

	void MultimaterialMesh::ImportSkinnedAsset(const std::string& filePath)
	{
		//Skinned
		_importer = new Assimp::Importer;

		selectedMesh = -1;
		selectedAnimation = -1;
		selectedNode = nullptr;

		_vertexBuffer = nullptr;
		_indexBuffer = nullptr;
		numVertices = 0;
		numIndices = 0;
		meshes.clear();

		scene = _importer->ReadFile(filePath.c_str(),
			aiProcess_Triangulate |
			aiProcess_ConvertToLeftHanded | aiProcess_JoinIdenticalVertices | aiProcess_GenBoundingBoxes |
			aiProcess_CalcTangentSpace | aiProcess_PopulateArmatureData |
			aiProcess_GenSmoothNormals | aiProcess_SortByPType | aiProcess_FixInfacingNormals | aiProcess_EmbedTextures | aiProcess_LimitBoneWeights);
		
		assert(scene != nullptr);
		
		for (uint32_t i = 0; i < scene->mNumMeshes; i++)
		{
			aiMesh* m = scene->mMeshes[i];
			numVertices += m->mNumVertices;
			numIndices += m->mNumFaces * 3;
		}

		if (numVertices == 0) return;

		LayoutDefine::Vin1stSkinned* vertices = new LayoutDefine::Vin1stSkinned[numVertices];
		int32_t* indices = new int32_t[numIndices];
		uint32_t vid = 0, iid = 0;
		for (uint32_t i = 0; i < scene->mNumMeshes; i++)
		{
			aiMesh* m = scene->mMeshes[i];
			for (uint32_t j = 0; j < m->mNumVertices; j++)
			{
				auto& pos = m->mVertices[j];
				auto& norm = m->mNormals[j];
				auto& tan = m->mTangents[j];
				auto& uv = m->mTextureCoords[0][j];

				vertices[vid + j].posL = DirectX::XMFLOAT3{ pos.x, pos.y, pos.z };
				vertices[vid + j].normalL = DirectX::XMFLOAT3{ norm.x, norm.y, norm.z };
				vertices[vid + j].tangentL = DirectX::XMFLOAT3{ tan.x, tan.y, tan.z };
				vertices[vid + j].color = DirectX::XMFLOAT4{1.0f,1.0f, 1.0f, 1.0f };
				vertices[vid + j].tex = DirectX::XMFLOAT3{ uv.x, uv.y, uv.z };
				vertices[vid + j].matID = m->mMaterialIndex;

				vertices[vid + j].blendIndice0 = 0;
				vertices[vid + j].blendIndice1 = 0;
				vertices[vid + j].blendIndice2 = 0;
				vertices[vid + j].blendIndice3 = 0;

				vertices[vid + j].blendWeight0 = 0.0f;
				vertices[vid + j].blendWeight1 = 0.0f;
				vertices[vid + j].blendWeight2 = 0.0f;
			}

			for (uint32_t j = 0; j < m->mNumFaces; j++)
			{
				indices[iid + j * 3] = m->mFaces[j].mIndices[0];
				indices[iid + j * 3 + 1] = m->mFaces[j].mIndices[1];
				indices[iid + j * 3 + 2] = m->mFaces[j].mIndices[2];
			}

			Mesh mesh;
			mesh.startVertex = vid;
			mesh.startIndex = iid;
			mesh.numVertices = m->mNumVertices;
			mesh.numIndices = m->mNumFaces * 3;
			meshes.push_back(mesh);

			vid += m->mNumVertices;
			iid += m->mNumFaces * 3;
		}

		do
		{
			CD3D11_BUFFER_DESC vbDesc(
				numVertices * sizeof(LayoutDefine::Vin1stSkinned),
				D3D11_BIND_VERTEX_BUFFER);
			D3D11_SUBRESOURCE_DATA vbData = { vertices, 0, 0 };
			if (S_OK != _device->CreateBuffer(&vbDesc, &vbData, &_vertexBuffer))
				assert(false);

			CD3D11_BUFFER_DESC ibDesc(
				numIndices * sizeof(uint32_t),
				D3D11_BIND_INDEX_BUFFER);
			D3D11_SUBRESOURCE_DATA ibData = { indices, 0, 0 };
			if (S_OK != _device->CreateBuffer(&ibDesc, &ibData, &_indexBuffer))
				assert(false);

		} while (0);

		delete[] vertices;
		delete[] indices;

		if (nullptr == _vertexBuffer || nullptr == _indexBuffer)
		{
			if (nullptr != _vertexBuffer) _vertexBuffer->Release();
			if (nullptr != _indexBuffer) _indexBuffer->Release();
		}

	}

	void MultimaterialMesh::RenderScene(Pg::Data::CameraData* camData)
	{
		if (meshes.empty() || nullptr == scene)
		{
			assert(false);
		}

		auto _DXStorage = LowDX11Storage::GetInstance();
		_devCon->OMSetRenderTargets(1, &(_DXStorage->_mainRTV), (_DXStorage->_depthStencilView));

		_devCon->IASetInputLayout(LayoutDefine::GetSkinned1stLayout());
		_devCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		_devCon->RSSetState(_rasterizerState);

		_devCon->VSSetShader(_vertexShader, nullptr, 0);
		_devCon->PSSetShader(_pixelShader, nullptr, 0);
		_devCon->PSSetSamplers(0, 1, &_samplerState);

		UINT strides[] = { sizeof(LayoutDefine::Vin1stSkinned) };
		UINT offsets[] = { 0 };

		_devCon->IASetVertexBuffers(0, 1, &_vertexBuffer, strides, offsets);
		_devCon->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
		
		//BufferMemory 매핑만 설정.
		_devCon->VSSetConstantBuffers(0, 1, &_constantBuffer);

		//1/100으로 줄여서 렌더링할 것이다. -> 일단은 World Matrix를 Identity로!
		DirectX::XMMATRIX tWorldMat = DirectX::XMMatrixIdentity();

		//0.01 스케일링 적용.
		DirectX::XMMATRIX tWorldMatScaled = DirectX::XMMatrixMultiply(DirectX::XMMatrixScaling(0.01f, 0.01f, 0.01f), tWorldMat);
		//DirectX::XMMATRIX tWorldMatScaled = DirectX::XMMatrixMultiply(DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f), tWorldMat);
		//이론 : 현재 기본적으로 aiMatrix4x4가 Transpose되어 있을 것이라는 전제로 코드가 짜여졌기에, 역으로 시작도 전치해서 넣어줘야!
		//스케일만 있는 지금이야 상관이 없겠지만, 위치가 Translate 조금만 되어도 보일 것!
		DirectX::XMMATRIX tWorldMatScaledTransposed = DirectX::XMMatrixTranspose(tWorldMatScaled);
		DirectX::XMFLOAT4X4 tWorldMatScaledTransposedFF;
		DirectX::XMStoreFloat4x4(&tWorldMatScaledTransposedFF, tWorldMatScaled);

		render_scene_node(camData, scene->mRootNode, tWorldMatScaledTransposedFF);

		//VS/PS Unbind.
		_devCon->VSSetShader(nullptr, nullptr, 0);
		_devCon->PSSetShader(nullptr, nullptr, 0);
	}

	void MultimaterialMesh::render_scene_node(Pg::Data::CameraData* camData, aiNode* node, DirectX::XMFLOAT4X4 parentTransform)
	{
		DirectX::XMMATRIX tParentTranform = DirectX::XMLoadFloat4x4(&parentTransform);

		DirectX::XMFLOAT4X4 tNodeTransformFF;
		aiMatrix4x4 tAssimpNodeTransformFF = node->mTransformation.Transpose();
		std::memcpy(&tNodeTransformFF, &tAssimpNodeTransformFF, sizeof(DirectX::XMFLOAT4X4));
		DirectX::XMMATRIX tNodeTransform = DirectX::XMLoadFloat4x4(&tNodeTransformFF);

		DirectX::XMMATRIX tCurrent = DirectX::XMMatrixMultiply(tParentTranform, tNodeTransform);
		DirectX::XMFLOAT4X4 tCurrentFF;
		DirectX::XMStoreFloat4x4(&tCurrentFF, tCurrent);

		if (node->mNumMeshes > 0)
		{
			D3D11_MAPPED_SUBRESOURCE res = {};
			if (S_OK == _devCon->Map(_constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &res))
			{
				ConstantBufferDefine::cbPerObjectBase* data = reinterpret_cast<ConstantBufferDefine::cbPerObjectBase*>(res.pData);

				UpdateConstantBuffer(camData, tCurrentFF);
				*(data) = *_constantBufferStruct;

				_devCon->Unmap(_constantBuffer, 0);
			}
		}

		for (uint32_t i = 0; i < node->mNumMeshes; i++)
		{
			Mesh& m = meshes[node->mMeshes[i]];

			aiMesh* tAiMesh = scene->mMeshes[node->mMeshes[i]];

			//이제 SolidRS-DiffuseTexture를 이용하기에, 
			//Mesh의 인덱스에 따라 PSSetShaderResources를
			//해당 Mesh의 Material의 인덱스에 맞게 호출한다.
			_devCon->PSSetShaderResources(0, 1, &(_tempSRVArray[tAiMesh->mMaterialIndex]));

			_devCon->DrawIndexed(m.numIndices, m.startIndex, m.startVertex);
		}

		for (uint32_t i = 0; i < node->mNumChildren; i++)
		{
			render_scene_node(camData, node->mChildren[i], tCurrentFF);
		}
	}

	void MultimaterialMesh::LoadHardCodedSRVs()
	{
		std::wstring t1stSRVPath = L"../Resources/3DModels/Animated/Textures/boss_lp_body_lp_AlbedoTransparency.png";
		std::wstring t2ndSRVPath = L"../Resources/3DModels/Animated/Textures/boss_lp_atc_lp_AlbedoTransparency.png";
		std::wstring t3rdSRVPath = L"../Resources/3DModels/Animated/Textures/StylizedWoodenFloor_Diffuse.png";

		HR(DirectX::CreateWICTextureFromFile(LowDX11Storage::GetInstance()->_device,
			LowDX11Storage::GetInstance()->_deviceContext,
			t1stSRVPath.c_str(), nullptr, &(_tempSRVArray[0])));

		HR(DirectX::CreateWICTextureFromFile(LowDX11Storage::GetInstance()->_device,
			LowDX11Storage::GetInstance()->_deviceContext,
			t2ndSRVPath.c_str(), nullptr, &(_tempSRVArray[1])));

		HR(DirectX::CreateWICTextureFromFile(LowDX11Storage::GetInstance()->_device,
			LowDX11Storage::GetInstance()->_deviceContext,
			t3rdSRVPath.c_str(), nullptr, &(_tempSRVArray[2])));
	}

}
