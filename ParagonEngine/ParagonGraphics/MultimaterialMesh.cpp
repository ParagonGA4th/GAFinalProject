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

#include "../ParagonAPI/PgInput.h"

#include <dxtk/DDSTextureLoader.h>
#include <dxtk/WICTextureLoader.h>

//ASSIMP로, 리소스 관리 로직이랑 별개 관리 예정.
#include <assimp/Importer.hpp>     
#include <assimp/scene.h>          
#include <assimp/postprocess.h> 
#include <singleton-cpp/singleton.h> 
#include <cassert> 
#include <algorithm> 

#include "../ParagonUtil/Log.h"

#ifdef _DEBUG
#pragma comment(lib,"..\\Builds\\x64\\Debug\\ParagonAPI.lib")
#else
#pragma comment(lib,"..\\Builds\\x64\\Release\\ParagonAPI.lib")
#endif // _DEBUG

namespace Pg::Graphics
{
	using namespace tofu;

	using Pg::Graphics::Manager::GraphicsResourceManager;
	using Pg::Data::Enums::eAssetDefine;

	//static 
	MultimaterialMesh::MultimaterialMesh(const std::string& filePath)
	{
		_constantBufferStruct = new ConstantBufferDefine::cbPerObjectBase;
		_skinnedCBuffer = new ConstantBufferDefine::cbPerObjectSkinned;

		//Device / DevCon 받아오기.
		_device = LowDX11Storage::GetInstance()->_device;
		_devCon = LowDX11Storage::GetInstance()->_deviceContext;

		auto& tTempInput = singleton<Pg::API::Input::PgInput>();
		_tempInput = &tTempInput;

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
		delete _skinnedCBuffer;
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
		HRESULT hr = D3DReadFileToBlob(tTempForwardVSPath.c_str(), &(tVertexShaderByteCode));
		_device->CreateVertexShader(tVertexShaderByteCode->GetBufferPointer(),
			tVertexShaderByteCode->GetBufferSize(), NULL, &_vertexShader);
		assert(hr == S_OK);

		//TempForwardPS -> Vertex Shader 셋업.
		ID3DBlob* tPixelShaderByteCode = nullptr;
		D3DReadFileToBlob(tTempForwardPSPath.c_str(), &(tPixelShaderByteCode));
		_device->CreatePixelShader(tPixelShaderByteCode->GetBufferPointer(),
			tPixelShaderByteCode->GetBufferSize(), NULL, &_pixelShader);
		assert(hr == S_OK);
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
		{
			//int sizeCB = (((sizeof(ConstantBufferDefine::cbPerObjectBase) - 1) / 16) + 1) * 16;	// declspec 으로 16바이트 정렬할 수 있다?

			//상수 버퍼를 명시적으로 넣어주는 문제 때문에 문제 발생?
			int sizeCB = (((sizeof(ConstantBufferDefine::cbPerObjectBase) - 1) / 16) + 1) * 16;	// declspec 으로 16바이트 정렬할 수 있다?
			assert(sizeCB % 16 == 0);
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

			_cbufferSubresourceData0.pSysMem = _constantBufferStruct;

			HR(_device->CreateBuffer(&tCBufferDesc, &_cbufferSubresourceData0, &(_constantBuffer[0])));
		}

		{
			//int sizeCB = (((sizeof(MultimaterialMesh::SkinnedCBuffer) - 1) / 16) + 1) * 16;	// declspec 으로 16바이트 정렬할 수 있다?
			int sizeCB = sizeof(ConstantBufferDefine::cbPerObjectSkinned);
			assert(sizeCB % 16 == 0);

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

			_cbufferSubresourceData1.pSysMem = _skinnedCBuffer;

			HR(_device->CreateBuffer(&tCBufferDesc, &_cbufferSubresourceData1, &(_constantBuffer[1])));
		}

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

		//셰이더에 넣을 버퍼 처리를 위해서 Vector Resize.
		_boneTransformVector.resize(100);
		std::fill(_boneTransformVector.begin(), _boneTransformVector.end(), DirectX::XMMatrixIdentity());

		//Skinned Data를 위해서 구조체 처리.

		SetupBoneData(_vertexBoneVector, scene, numVertices);

		//지금까지 Bone Index/Weight Binding을 위해, 인덱스 카운팅 도입.
		UINT tTotalElapsedVertexCount = 0;

		LayoutDefine::Vin1stSkinned* vertices = new LayoutDefine::Vin1stSkinned[numVertices];
		int32_t* indices = new int32_t[numIndices];
		uint32_t vid = 0, iid = 0;
		for (uint32_t i = 0; i < scene->mNumMeshes; i++)
		{
			aiMesh* m = scene->mMeshes[i];
			//tTotalElapsedVertexCount += _meshEntriesVector[i].BaseVertex;
			tTotalElapsedVertexCount = _meshEntriesVector[i].BaseVertex;
			for (uint32_t j = 0; j < m->mNumVertices; j++)
			{
				auto& pos = m->mVertices[j];
				auto& norm = m->mNormals[j];
				auto& tan = m->mTangents[j];
				auto& uv = m->mTextureCoords[0][j];

				vertices[vid + j].posL = DirectX::XMFLOAT3{ pos.x, pos.y, pos.z };
				vertices[vid + j].normalL = DirectX::XMFLOAT3{ norm.x, norm.y, norm.z };
				vertices[vid + j].tangentL = DirectX::XMFLOAT3{ tan.x, tan.y, tan.z };
				vertices[vid + j].color = DirectX::XMFLOAT4{ 1.0f,1.0f, 1.0f, 1.0f };
				vertices[vid + j].tex = DirectX::XMFLOAT3{ uv.x, uv.y, uv.z };
				vertices[vid + j].matID = m->mMaterialIndex;

				vertices[vid + j].blendIndice0 = _vertexBoneVector.at(j + tTotalElapsedVertexCount).IDs[0];
				vertices[vid + j].blendIndice1 = _vertexBoneVector.at(j + tTotalElapsedVertexCount).IDs[1];
				vertices[vid + j].blendIndice2 = _vertexBoneVector.at(j + tTotalElapsedVertexCount).IDs[2];
				vertices[vid + j].blendIndice3 = _vertexBoneVector.at(j + tTotalElapsedVertexCount).IDs[3];

				vertices[vid + j].blendWeight0 = _vertexBoneVector.at(j + tTotalElapsedVertexCount).Weights[0];
				vertices[vid + j].blendWeight1 = _vertexBoneVector.at(j + tTotalElapsedVertexCount).Weights[1];
				vertices[vid + j].blendWeight2 = _vertexBoneVector.at(j + tTotalElapsedVertexCount).Weights[2];
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

		//1/100으로 줄여서 렌더링할 것이다. -> 일단은 World Matrix를 Identity로!
		///커스텀 위치 조정.
		///현재 렌더 로직 떄문에, 이 역시 적용되지 않는 상황이다!
		DirectX::XMFLOAT3 tPosition = { 0.0f, 0.0f, 0.0f };
		DirectX::XMVECTOR tPosVec = DirectX::XMLoadFloat3(&tPosition);

		DirectX::XMFLOAT4 tRotQuat = { 0.0f, 0.0f, 0.0f, 0.0f };
		DirectX::XMVECTOR tRotQuatVec = DirectX::XMLoadFloat4(&tRotQuat);

		//0.01 스케일링 적용.
		//DirectX::XMFLOAT3 tScale = { 0.01f, 0.01f, 0.01f };
		//이 모델이 되따 크다.
		DirectX::XMFLOAT3 tScale = { 0.0001f, 0.0001f, 0.0001f };
		//DirectX::XMFLOAT3 tScale = {1.0f,1.0f, 1.0f};
		DirectX::XMVECTOR tScaleVec = DirectX::XMLoadFloat3(&tScale);

		DirectX::XMMATRIX tWorldMatScaled = DirectX::XMMatrixAffineTransformation(tScaleVec, tPosVec, tRotQuatVec, tPosVec);

		DirectX::XMFLOAT4X4 tWorldMatScaledFF;
		DirectX::XMStoreFloat4x4(&tWorldMatScaledFF, tWorldMatScaled);

		//그런가..? 확실한 것은, 들어갈 때는 올바르게 (DX 기준) 행렬이 매개변수로 들어가는 것을 전제로 하고 있다. 
		//다시 생각해봐라! Transpose 여부.

		//BufferMemory 매핑만 설정. -> 이제 Skinned니까 2개!
		_devCon->VSSetConstantBuffers(0, 1, &(_constantBuffer[0]));
		_devCon->VSSetConstantBuffers(1, 1, &(_constantBuffer[1]));

		UpdateSkinnedCBuffer(); //내부에서 Skinned Constant Buffer 업데이트.

		//render_scene_node(camData, scene->mRootNode, tWorldMatScaledFF);
		RenderSkinnedNodes(camData, tWorldMatScaledFF); // 내부에서 Normal Constant Buffer 업데이트.

		//VS/PS Unbind.
		_devCon->VSSetShader(nullptr, nullptr, 0);
		_devCon->PSSetShader(nullptr, nullptr, 0);
	}

	void MultimaterialMesh::render_scene_node(Pg::Data::CameraData* camData, aiNode* node, DirectX::XMFLOAT4X4 parentTransform)
	{
		DirectX::XMMATRIX tParentTranform = DirectX::XMLoadFloat4x4(&parentTransform);

		DirectX::XMFLOAT4X4 tNodeTransformFF;
		aiMatrix4x4 tAssimpNodeTransformFF = node->mTransformation;

		std::memcpy(&tNodeTransformFF, &tAssimpNodeTransformFF, sizeof(DirectX::XMFLOAT4X4));
		DirectX::XMMATRIX tNodeTransform = DirectX::XMLoadFloat4x4(&tNodeTransformFF);

		//원본 데이터에 영향을 안 주는 선에서 복사한 데이터에서 전치해서 행렬을 보관한다(Assimp는 Column-Major);
		tNodeTransform = DirectX::XMMatrixTranspose(tNodeTransform);

		DirectX::XMMATRIX tCurrent = DirectX::XMMatrixMultiply(tParentTranform, tNodeTransform);
		DirectX::XMFLOAT4X4 tCurrentFF;
		DirectX::XMStoreFloat4x4(&tCurrentFF, tCurrent);

		//여기에서는 0번째 인덱스 (Constant Buffer 중) 업데이트. 1번째는 아직 그러지 못한 상황이다!
		if (node->mNumMeshes > 0)
		{
			D3D11_MAPPED_SUBRESOURCE res;
			ZeroMemory(&res, sizeof(D3D11_MAPPED_SUBRESOURCE));
			if (S_OK == _devCon->Map(_constantBuffer[0], 0, D3D11_MAP_WRITE_DISCARD, 0, &res))
			{
				ConstantBufferDefine::cbPerObjectBase* data = reinterpret_cast<ConstantBufferDefine::cbPerObjectBase*>(res.pData);

				UpdateConstantBuffer(camData, tCurrentFF);
				*(data) = *_constantBufferStruct;

				_devCon->Unmap(*_constantBuffer, 0);
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

	void MultimaterialMesh::SetupBoneData(std::vector<RenderUsageVertexBone>& vBoneList, const aiScene* scene, unsigned int verticeCount)
	{
		vBoneList.resize(verticeCount);

		unsigned int tNumVertices = 0;
		unsigned int tNumIndices = 0;

		//당장 Bone 데이터를 쓰는데 사용하지는 않으나, 실제 D3D 버퍼 출력에 활용되는 정보.
		_meshEntriesVector.resize(scene->mNumMeshes);
		for (unsigned int i = 0; i < _meshEntriesVector.size(); i++)
		{
			// Total mesh indices. 
			_meshEntriesVector[i].NumIndices = scene->mMeshes[i]->mNumFaces * 3;

			// Set the base vertex of this mesh (initial vertex for this mesh within the vertices array) to the current total vertices. 
			_meshEntriesVector[i].BaseVertex = tNumVertices;

			// Set the base index of this mesh (initial index for this mesh within the indices array) to the current total indices. 
			_meshEntriesVector[i].BaseIndex = tNumIndices;

			// Increment total vertices and indices. 
			tNumVertices += scene->mMeshes[i]->mNumVertices;
			tNumIndices += _meshEntriesVector[i].NumIndices;
		}

		//outBone.resize(tNumVertices);
		//outIndice.reserve(tNumIndices);

		for (unsigned int i = 0; i < scene->mNumMeshes; i++)
		{
			aiMesh* aMesh = scene->mMeshes[i];

			//Vertex 로드는 인덱스가 변하지 않기 때문에, 
			//제대로 적용됨
			if (aMesh->HasBones())
			{
				SetupRenderBones(i, aMesh, vBoneList);
			}
		}
	}

	void MultimaterialMesh::SetupRenderBones(unsigned int index, aiMesh* mesh, std::vector<RenderUsageVertexBone>& vBoneList)
	{
		for (unsigned int i = 0; i < mesh->mNumBones; i++) {

			unsigned int BoneIndex = 0;

			// Obtain the bone name.
			std::string BoneName(mesh->mBones[i]->mName.C_Str());

			// If bone isn't already in the map. 
			if (_mappedBones.find(BoneName) == _mappedBones.end())
			{
				// Set the bone ID to be the current total number of bones. 
				BoneIndex = _formationNumBone;

				// Increment total bones. 
				_formationNumBone++;

				// Push new bone info into bones vector. 
				RenderUsageBoneInfo tBi;
				_renderBoneInfoVector.push_back(tBi);
			}
			else
			{
				// Bone ID is already in map. 
				BoneIndex = _mappedBones[BoneName];
			}

			_mappedBones[BoneName] = BoneIndex;

			// Obtains the offset matrix which transforms the bone from mesh space into bone space. 
			aiMatrix4x4 tOffsetMat = mesh->mBones[i]->mOffsetMatrix;

			//Column Major -> Row Major : 전치.
			tOffsetMat = tOffsetMat.Transpose();
			DirectX::XMFLOAT4X4 tXMOffsetMat;
			std::memcpy(&tXMOffsetMat, &tOffsetMat, sizeof(DirectX::XMFLOAT4X4));
			_renderBoneInfoVector[BoneIndex].BoneOffset = tXMOffsetMat;

			// Iterate over all the affected vertices by this bone i.e weights. 
			for (unsigned int j = 0; j < mesh->mBones[i]->mNumWeights; j++)
			{
				// Obtain an index to the affected vertex within the array of vertices.
				unsigned int VertexID = _meshEntriesVector[index].BaseVertex + mesh->mBones[i]->mWeights[j].mVertexId;

				// The value of how much this bone influences the vertex. 
				float Weight = mesh->mBones[i]->mWeights[j].mWeight;

				// Insert bone data for particular vertex ID. A maximum of 4 bones can influence the same vertex. 
				vBoneList[VertexID].AddBoneData(BoneIndex, Weight);
			}
		}
		assert(mesh);
	}

	void MultimaterialMesh::Render(Pg::Data::CameraData* camData)
	{
		//그냥 모두 다 Looping 하게 재생!
		BoneTransformUpdate();
		RenderScene(camData);
	}

	void MultimaterialMesh::BoneTransformUpdate()
	{
		//처음에 아무것도 없으면 아예 출력이 되지 않으니, T-Pose Animation을 기본으로 잡고 코드를 짜야 한다.

		//현재 3DModel은 3개인 상태 : 0 / 1 / 2 중 2가 T-Pose.
		static short tChoice = 0;

		//if (_tempInput->GetKeyDown(API::Input::eKeyCode::MoveBack))
		//{
		//	tChoice = 0;
		//}
		//else if (_tempInput->GetKeyDown(API::Input::eKeyCode::MoveLeft))
		//{
		//	tChoice = 1;
		//}
		//else if (_tempInput->GetKeyDown(API::Input::eKeyCode::MoveRight))
		//{
		//	tChoice = 2;
		//}

		aiAnimation* tAnim = nullptr;
		tAnim = scene->mAnimations[tChoice];

		//절대로 일단은 정해져 있는 Tick 수 넘어가지 않게 -> 나머지 연산을 할것. 
		static double tPlayTickDur = 0;
		tPlayTickDur += 0.1;
		double tInwardTick = fmod(tPlayTickDur, tAnim->mDuration);
		assert(tInwardTick < tAnim->mDuration);
		PG_TRACE(tInwardTick);

		DirectX::XMFLOAT4X4 tDefaultMat =
		{ 1.0f,0.0f,0.0f,0.0f,
		  0.0f,1.0f,0.0f,0.0f,
		  0.0f,0.0f,1.0f,0.0f,
		  0.0f,0.0f,0.0f,1.0f };

		//ReadNodeHierarchy(tInwardTick, scene->mRootNode, tAnim, tDefaultMat);
		ReadNodeHierarchy(tInwardTick, scene->mRootNode, tAnim, tDefaultMat);

		assert(_formationNumBone < 100);
		// Populates transforms vector with new bone transformation matrices. 
		for (unsigned int i = 0; i < _formationNumBone; i++)
		{
			_boneTransformVector[i] = DirectX::XMLoadFloat4x4(&_renderBoneInfoVector[i].FinalTransformation);
		}
	}

	void MultimaterialMesh::ReadNodeHierarchy(double animTick, const aiNode* pNode, const aiAnimation* pAnim, DirectX::XMFLOAT4X4 parentTransform)
	{
		//using Matrix = DirectX::XMMATRIX;
		//using Quaternion = DirectX::XMFLOAT4;
		//DX 쿼터니언 : X,Y,Z (벡터), W(스칼라)
		// XMFLOAT4 / XMVECTOR

		// Obtain the name of the current node 
		std::string NodeName(pNode->mName.C_Str());

		// Use the first animation 
		const aiAnimation* pAnimation = pAnim;

		// Obtain transformation relative to node's parent. 
		aiMatrix4x4 tAiTrans = pNode->mTransformation;
		tAiTrans = tAiTrans.Transpose();
		DirectX::XMFLOAT4X4 NodeTransformation;
		std::memcpy(&NodeTransformation, &tAiTrans, sizeof(DirectX::XMFLOAT4X4));

		const aiNodeAnim* pNodeAnim = nullptr;

		// Find the animation channel of the current node. -> 나중에 미리 매핑해놓을 수 있음!
		//다만, Animation에 해당되지 않는 Node들은 일단 이 로직이 없으면 실행되지 않을 것이다! 이는 염두.

		for (unsigned i = 0; i < pAnimation->mNumChannels; i++)
		{
			const aiNodeAnim* pNodeAnimIndex = pAnimation->mChannels[i];
			std::string tNodeAnimName(pNodeAnimIndex->mNodeName.C_Str());

			// If there is a match for a channel with the current node's name, then we've found the animation channel. 
			if (tNodeAnimName.compare(NodeName) == 0)
			{
				pNodeAnim = pNodeAnimIndex;
			}
		}

		if (pNodeAnim)
		{
			// Interpolate rotation and generate rotation transformation matrix
			DirectX::XMFLOAT4 RotationQ;
			//CalcInterpolatedRotation(RotationQ, animationTime, pNodeAnim);
			CalcInterpolatedRotation(RotationQ, animTick, pNodeAnim);

			DirectX::XMVECTOR RotationQVec = DirectX::XMLoadFloat4(&RotationQ);
			DirectX::XMMATRIX RotationM = DirectX::XMMatrixRotationQuaternion(RotationQVec);

			// Interpolate translation and generate translation transformation matrix
			DirectX::XMFLOAT3 Translation;
			CalcInterpolatedTranslation(Translation, animTick, pNodeAnim);

			DirectX::XMMATRIX TranslationM;
			TranslationM = DirectX::XMMatrixTranslation(Translation.x, Translation.y, Translation.z);

			DirectX::XMMATRIX tCombinedTrans = DirectX::XMMatrixMultiply(RotationM, TranslationM);
			// Combine the above transformations
			DirectX::XMStoreFloat4x4(&NodeTransformation, tCombinedTrans);
		}
		DirectX::XMMATRIX nodeTransformationMat = DirectX::XMLoadFloat4x4(&NodeTransformation);
		DirectX::XMMATRIX parentTransformMat = DirectX::XMLoadFloat4x4(&parentTransform);

		DirectX::XMMATRIX GlobalTransformation = DirectX::XMMatrixMultiply(nodeTransformationMat, parentTransformMat);
		DirectX::XMFLOAT4X4 GlobalTransformationFF;
		DirectX::XMStoreFloat4x4(&GlobalTransformationFF, GlobalTransformation);

		// Apply the final transformation to the indexed bone in the array. 
		if (_mappedBones.find(NodeName) != _mappedBones.end())
		{
			unsigned int BoneIndex = _mappedBones[NodeName];

			//현재로서는 매 프레임 역행렬을 구한다.
			aiMatrix4x4 tAiRootTrans = scene->mRootNode->mTransformation;
			tAiRootTrans = tAiRootTrans.Transpose();
			DirectX::XMFLOAT4X4 tRootTrans;
			std::memcpy(&tRootTrans, &tAiRootTrans, sizeof(DirectX::XMFLOAT4X4));

			//위에 있는 변수와 잘 구별해야 한다!
			DirectX::XMMATRIX tGlobalTrans = DirectX::XMLoadFloat4x4(&tRootTrans);
			DirectX::XMVECTOR tDet = DirectX::XMVectorZero();
			DirectX::XMMATRIX tGlobalInverseTransform = DirectX::XMMatrixInverse(&tDet, tGlobalTrans);

			DirectX::XMMATRIX tBoneOffsetMat = DirectX::XMLoadFloat4x4(&(_renderBoneInfoVector[BoneIndex].BoneOffset));
			//1차 망가지기 전 Answer.
			DirectX::XMMATRIX tFinalTrans = DirectX::XMMatrixMultiply(DirectX::XMMatrixMultiply(tBoneOffsetMat, GlobalTransformation), tGlobalInverseTransform);
			DirectX::XMStoreFloat4x4(&(_renderBoneInfoVector[BoneIndex].FinalTransformation), tFinalTrans);
		}

		// Do the same for all the node's children. 
		for (unsigned i = 0; i < pNode->mNumChildren; i++)
		{
			ReadNodeHierarchy(animTick, pNode->mChildren[i], pAnim, GlobalTransformationFF);
		}
	}

	void MultimaterialMesh::CalcInterpolatedRotation(DirectX::XMFLOAT4& xmQuat, double animTick, const aiNodeAnim* pNodeAnim)
	{
		//using Matrix = DirectX::SimpleMath::Matrix;
		//using Quaternion = DirectX::SimpleMath::Quaternion;

		// we need at least two values to interpolate...
		if (pNodeAnim->mNumRotationKeys == 1)
		{
			//aiQuaternion은 wxyz 순서.
			aiQuaternion tQuat = pNodeAnim->mRotationKeys[0].mValue;
			xmQuat = { tQuat.x,tQuat.y,tQuat.z,tQuat.w };
			return;
		}
		// Obtain the current rotation keyframe. 
		unsigned int RotationIndex = FindRotation(animTick, pNodeAnim);

		// Calculate the next rotation keyframe and check bounds. 
		unsigned int NextRotationIndex = (RotationIndex + 1);
		assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);

		// Calculate delta time, i.e time between the two keyframes.
		float DeltaTime = pNodeAnim->mRotationKeys[NextRotationIndex].mTime - pNodeAnim->mRotationKeys[RotationIndex].mTime;

		// Calculate the elapsed time within the delta time.  
		float Factor = (animTick - (float)pNodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
		//assert(Factor >= 0.0f && Factor <= 1.0f);

		// Obtain the quaternions values for the current and next keyframe. 
		aiQuaternion tAiStartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
		aiQuaternion tAiEndRotationQ = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;

		DirectX::XMFLOAT4 StartRotationQ = { tAiStartRotationQ.x,tAiStartRotationQ.y,tAiStartRotationQ.z,tAiStartRotationQ.w };
		DirectX::XMFLOAT4 EndRotationQ = { tAiEndRotationQ.x,tAiEndRotationQ.y,tAiEndRotationQ.z,tAiEndRotationQ.w };

		DirectX::XMVECTOR StartRotationQVec = DirectX::XMLoadFloat4(&StartRotationQ);
		DirectX::XMVECTOR EndRotationQVec = DirectX::XMLoadFloat4(&EndRotationQ);

		// Interpolate between them using the Factor. 
		DirectX::XMVECTOR tXMQuatVec = DirectX::XMQuaternionSlerp(StartRotationQVec, EndRotationQVec, Factor);
		// Normalise and set the reference. 
		tXMQuatVec = DirectX::XMQuaternionNormalize(tXMQuatVec);

		DirectX::XMStoreFloat4(&xmQuat, tXMQuatVec);
	}

	void MultimaterialMesh::CalcInterpolatedTranslation(DirectX::XMFLOAT3& xmTrans, double animTick, const aiNodeAnim* pNodeAnim)
	{
		// we need at least two values to interpolate...
		if (pNodeAnim->mNumPositionKeys == 1)
		{
			aiVector3D tVal = pNodeAnim->mPositionKeys[0].mValue;
			xmTrans = { tVal.x, tVal.y, tVal.z };
			return;
		}

		unsigned int PositionIndex = FindTranslation(animTick, pNodeAnim);
		unsigned int NextPositionIndex = (PositionIndex + 1);
		assert(NextPositionIndex < pNodeAnim->mNumPositionKeys);

		float DeltaTime = pNodeAnim->mPositionKeys[NextPositionIndex].mTime - pNodeAnim->mPositionKeys[PositionIndex].mTime;
		float Factor = (animTick - (float)pNodeAnim->mPositionKeys[PositionIndex].mTime) / DeltaTime;
		//assert(Factor >= 0.0f && Factor <= 1.0f);

		auto tStartVal = pNodeAnim->mPositionKeys[PositionIndex].mValue;
		auto tEndVal = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;

		DirectX::XMFLOAT3 Start = { tStartVal.x,tStartVal.y,tStartVal.z };
		DirectX::XMFLOAT3 End = { tEndVal.x,tEndVal.y,tEndVal.z };

		DirectX::XMVECTOR StartVec = DirectX::XMLoadFloat3(&Start);
		DirectX::XMVECTOR EndVec = DirectX::XMLoadFloat3(&End);

		DirectX::XMVECTOR DeltaVec = DirectX::XMVectorSubtract(EndVec, StartVec);

		using namespace DirectX;
		DirectX::XMVECTOR OutVec = StartVec + (Factor * DeltaVec);
		DirectX::XMStoreFloat3(&xmTrans, OutVec);
	}

	unsigned int MultimaterialMesh::FindRotation(double animTick, const aiNodeAnim* pNodeAnim)
	{
		// Check if there are rotation keyframes. 
		assert(pNodeAnim->mNumRotationKeys > 0);

		// Find the rotation key just before the current animation time and return the index. 
		for (unsigned int i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++)
		{
			//FIX.
			if (animTick < (float)pNodeAnim->mRotationKeys[i + 1].mTime)
			{
				return i;
			}
		}
		assert(0);

		return 0;
	}

	unsigned int MultimaterialMesh::FindTranslation(double animTick, const aiNodeAnim* pNodeAnim)
	{
		assert(pNodeAnim->mNumPositionKeys > 0);

		// Find the translation key just before the current animation time and return the index. 
		for (unsigned int i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++)
		{
			//FIX.
			if (animTick < (float)pNodeAnim->mPositionKeys[i + 1].mTime)
			{
				return i;
			}
		}
		assert(0);

		return 0;
	}

	void MultimaterialMesh::UpdateSkinnedCBuffer()
	{
		//옮겨졌던 모든 스키닝을 Vector로 옮기기. (구조체로)
	
		for (int i = 0; i < 100; i++)
		{
			_skinnedCBuffer->gCBuf_Bones[i] = _boneTransformVector[i];
		}

		D3D11_MAPPED_SUBRESOURCE res;
		ZeroMemory(&res, sizeof(D3D11_MAPPED_SUBRESOURCE));

		HRESULT hr = _devCon->Map(_constantBuffer[1], 0, D3D11_MAP_WRITE_DISCARD, 0, &res);
		if (hr == S_OK)
		{
			ConstantBufferDefine::cbPerObjectSkinned* data = reinterpret_cast<ConstantBufferDefine::cbPerObjectSkinned*>(res.pData);
			*(data) = *_skinnedCBuffer;

			_devCon->Unmap(_constantBuffer[1], 0);
		}
		else
		{
			assert(false);
		}
	}

	void MultimaterialMesh::RenderSkinnedNodes(Pg::Data::CameraData* camData, DirectX::XMFLOAT4X4 renderPos)
	{
		//한꺼번에 그려야 할 것. Mesh들 사이는 나누지만, 그 이상은 X!

		D3D11_MAPPED_SUBRESOURCE res;
		ZeroMemory(&res, sizeof(D3D11_MAPPED_SUBRESOURCE));
		if (S_OK == _devCon->Map(_constantBuffer[0], 0, D3D11_MAP_WRITE_DISCARD, 0, &res))
		{
			ConstantBufferDefine::cbPerObjectBase* data = reinterpret_cast<ConstantBufferDefine::cbPerObjectBase*>(res.pData);

			UpdateConstantBuffer(camData, renderPos);
			*(data) = *_constantBufferStruct;

			_devCon->Unmap(*_constantBuffer, 0);
		}

		//한꺼번에 렌더 함수들 모아놓기. (Mesh들 사이는 DrawCall 나누고!)

		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			Mesh& m = meshes[i];
			aiMesh* tAiMesh = scene->mMeshes[i];

			//이제 SolidRS-DiffuseTexture를 이용하기에, 
			//Mesh의 인덱스에 따라 PSSetShaderResources를
			//해당 Mesh의 Material의 인덱스에 맞게 호출한다.
			_devCon->PSSetShaderResources(0, 1, &(_tempSRVArray[tAiMesh->mMaterialIndex]));

			_devCon->DrawIndexed(m.numIndices, m.startIndex, m.startVertex);
		}
	}

}
