//#include "MultimaterialMesh.h"
//#include "Asset3DModelData.h"
//#include "GraphicsResourceManager.h"
//#include "LayoutDefine.h"
//#include "LowDX11Storage.h"
//#include "MathHelper.h"
//#include "AssetTextureType.h"
//
//#include "../ParagonData/GraphicsResource.h"
//#include "../ParagonData/AssetDefines.h"
//#include "../ParagonData/CameraData.h"
//
//#include "../ParagonUtil/InputSystem.h"
//
//#include <dxtk/DDSTextureLoader.h>
//#include <dxtk/WICTextureLoader.h>
//
////ASSIMP로, 리소스 관리 로직이랑 별개 관리 예정.
//#include <assimp/Importer.hpp>     
//#include <assimp/scene.h>          
//#include <assimp/postprocess.h> 
//#include <singleton-cpp/singleton.h> 
//#include <cassert> 
//#include <algorithm> 
//
//#include "../ParagonUtil/Log.h"
//
//namespace Pg::Graphics
//{
//	using Pg::Graphics::Manager::GraphicsResourceManager;
//	using Pg::Graphics::Helper::MathHelper;
//	using Pg::Data::Enums::eAssetDefine;
//
//	using DirectX::SimpleMath::Matrix;
//	using DirectX::SimpleMath::Quaternion;
//	using DirectX::SimpleMath::Vector3;
//	using DirectX::SimpleMath::Vector4;
//
//#pragma region Unchanged
//	//static 
//	MultimaterialMesh::MultimaterialMesh(const std::string& filePath)
//	{
//		_constantBufferStruct = new ConstantBufferDefine::cbPerObjectBase;
//		_skinnedCBuffer = new ConstantBufferDefine::cbPerObjectSkinned;
//
//		//Device / DevCon 받아오기.
//		_device = LowDX11Storage::GetInstance()->_device;
//		_devCon = LowDX11Storage::GetInstance()->_deviceContext;
//
//		auto& tTempInput = singleton<Pg::Util::Input::InputSystem>();
//		_tempInput = &tTempInput;
//
//		CreateVertexPixelShader();
//		CreateSamplerState();
//		CreateRasterizerState();
//		CreateConstantBuffer();
//		LoadHardCodedSRVs();
//
//
//		//std::string tFilePath = "../Resources/3DModels/AnimMesh/Timmy_Shooting/Timmy_Shooting.fbx";
//		//std::string tFilePath = "../Resources/3DModels/AnimMesh/Cylinder/Cylinder.fbx";
//		std::string tFilePath = "../Resources/3DModels/AnimMesh/FallFlatMonster/FallFlatMonster.fbx";
//		ImportSkinnedAsset(tFilePath);
//	}
//
//	MultimaterialMesh::~MultimaterialMesh()
//	{
//		delete _constantBufferStruct;
//		delete _skinnedCBuffer;
//	}
//
//	void MultimaterialMesh::CreateSamplerState()
//	{
//		D3D11_SAMPLER_DESC sd;
//		sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
//		sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
//		sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
//		sd.Filter = D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;
//		sd.MipLODBias = 0.0f;
//		sd.MaxAnisotropy = 1;
//
//		_device->CreateSamplerState(&sd, &_samplerState);
//	}
//
//	void MultimaterialMesh::CreateVertexPixelShader()
//	{
//		//TempForwardVS/PS 경로를 셋업한다.
//		//std::wstring tTempForwardVSPath = L"../Builds/x64/Debug/TempForwardVS.cso";
//		std::wstring tTempForwardVSPath = L"../Builds/x64/Debug/TempForwardVSSkinned.cso";
//		std::wstring tTempForwardPSPath = L"../Builds/x64/Debug/TempForwardPS.cso";
//
//		//TempForwardVS -> Vertex Shader 셋업.
//		ID3DBlob* tVertexShaderByteCode = nullptr;
//		HRESULT hr = D3DReadFileToBlob(tTempForwardVSPath.c_str(), &(tVertexShaderByteCode));
//		_device->CreateVertexShader(tVertexShaderByteCode->GetBufferPointer(),
//			tVertexShaderByteCode->GetBufferSize(), NULL, &_vertexShader);
//		assert(hr == S_OK);
//
//		//TempForwardPS -> Vertex Shader 셋업.
//		ID3DBlob* tPixelShaderByteCode = nullptr;
//		D3DReadFileToBlob(tTempForwardPSPath.c_str(), &(tPixelShaderByteCode));
//		_device->CreatePixelShader(tPixelShaderByteCode->GetBufferPointer(),
//			tPixelShaderByteCode->GetBufferSize(), NULL, &_pixelShader);
//		assert(hr == S_OK);
//	}
//
//	void MultimaterialMesh::CreateRasterizerState()
//	{
//		D3D11_RASTERIZER_DESC rd;
//		//rd.FillMode = D3D11_FILL_WIREFRAME;
//		rd.FillMode = D3D11_FILL_SOLID;
//		rd.CullMode = D3D11_CULL_BACK;
//		rd.FrontCounterClockwise = false;
//		rd.DepthBias = 0;
//		rd.SlopeScaledDepthBias = 0.0f;
//		rd.DepthBiasClamp = 0.0f;
//		rd.DepthClipEnable = true;
//		rd.ScissorEnable = false;
//		rd.MultisampleEnable = false;
//		rd.AntialiasedLineEnable = false;
//
//		_device->CreateRasterizerState(&rd, &_rasterizerState);
//	}
//
//	void MultimaterialMesh::CreateConstantBuffer()
//	{
//		{
//			//int sizeCB = (((sizeof(ConstantBufferDefine::cbPerObjectBase) - 1) / 16) + 1) * 16;	// declspec 으로 16바이트 정렬할 수 있다?
//
//			//상수 버퍼를 명시적으로 넣어주는 문제 때문에 문제 발생?
//			int sizeCB = (((sizeof(ConstantBufferDefine::cbPerObjectBase) - 1) / 16) + 1) * 16;	// declspec 으로 16바이트 정렬할 수 있다?
//			assert(sizeCB % 16 == 0);
//			D3D11_BUFFER_DESC tCBufferDesc;
//			tCBufferDesc.ByteWidth = sizeCB; // 상수버퍼는 16바이트 정렬
//			tCBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
//			tCBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
//			tCBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
//			tCBufferDesc.MiscFlags = 0;
//
//			/*CD3D11_BUFFER_DESC cbDesc(
//				sizeof(float) * 16,
//				D3D11_BIND_CONSTANT_BUFFER,
//				D3D11_USAGE_DYNAMIC,
//				D3D11_CPU_ACCESS_WRITE);*/
//
//			_cbufferSubresourceData0.pSysMem = _constantBufferStruct;
//
//			HR(_device->CreateBuffer(&tCBufferDesc, &_cbufferSubresourceData0, &(_constantBuffer[0])));
//		}
//
//		{
//			//int sizeCB = (((sizeof(MultimaterialMesh::SkinnedCBuffer) - 1) / 16) + 1) * 16;	// declspec 으로 16바이트 정렬할 수 있다?
//			int sizeCB = sizeof(ConstantBufferDefine::cbPerObjectSkinned);
//			assert(sizeCB % 16 == 0);
//
//			D3D11_BUFFER_DESC tCBufferDesc;
//			tCBufferDesc.ByteWidth = sizeCB; // 상수버퍼는 16바이트 정렬
//			tCBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
//			tCBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
//			tCBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
//			tCBufferDesc.MiscFlags = 0;
//
//			/*CD3D11_BUFFER_DESC cbDesc(
//				sizeof(float) * 16,
//				D3D11_BIND_CONSTANT_BUFFER,
//				D3D11_USAGE_DYNAMIC,
//				D3D11_CPU_ACCESS_WRITE);*/
//
//			_cbufferSubresourceData1.pSysMem = _skinnedCBuffer;
//
//			HR(_device->CreateBuffer(&tCBufferDesc, &_cbufferSubresourceData1, &(_constantBuffer[1])));
//		}
//
//	}
//
//	void MultimaterialMesh::LoadHardCodedSRVs()
//	{
//		std::wstring t1stSRVPath = L"../Resources/3DModels/AnimMesh/Boss_Test_NonDeform_MultiMat/Boss_Test_NonDeform_MultiMat.fbm/boss_lp_atc_lp_AlbedoTransparency.png";
//		std::wstring t2ndSRVPath = L"../Resources/3DModels/AnimMesh/Boss_Test_NonDeform_MultiMat/Boss_Test_NonDeform_MultiMat.fbm/boss_lp_body_lp_AlbedoTransparency.png";
//		std::wstring t3rdSRVPath = L"../Resources/3DModels/AnimMesh/Boss_Test_NonDeform_MultiMat/Boss_Test_NonDeform_MultiMat.fbm/StylizedWoodenFloor_Diffuse.png";
//
//		HR(DirectX::CreateWICTextureFromFile(LowDX11Storage::GetInstance()->_device,
//			LowDX11Storage::GetInstance()->_deviceContext,
//			t1stSRVPath.c_str(), nullptr, &(_tempSRVArray[0])));
//
//		HR(DirectX::CreateWICTextureFromFile(LowDX11Storage::GetInstance()->_device,
//			LowDX11Storage::GetInstance()->_deviceContext,
//			t2ndSRVPath.c_str(), nullptr, &(_tempSRVArray[1])));
//
//		HR(DirectX::CreateWICTextureFromFile(LowDX11Storage::GetInstance()->_device,
//			LowDX11Storage::GetInstance()->_deviceContext,
//			t3rdSRVPath.c_str(), nullptr, &(_tempSRVArray[2])));
//
//		//Timmy
//		HR(DirectX::CreateWICTextureFromFile(LowDX11Storage::GetInstance()->_device,
//			LowDX11Storage::GetInstance()->_deviceContext,
//			L"../Resources/3DModels/AnimMesh/Timmy_Shooting/Timmy_Shooting.fbm/Ch09_1001_Diffuse.png", nullptr, &_tempTimmySRV));
//
//		//Direct3D Cylinder
//		HR(DirectX::CreateDDSTextureFromFile(LowDX11Storage::GetInstance()->_device,
//			LowDX11Storage::GetInstance()->_deviceContext,
//			L"../Resources/3DModels/AnimMesh/Cylinder/Cylinder.fbm/WoodCrate01.dds", nullptr, &_tempCylinderSRV));
//
//		//4Q SRV
//		HR(DirectX::CreateWICTextureFromFile(LowDX11Storage::GetInstance()->_device,
//			LowDX11Storage::GetInstance()->_deviceContext,
//			L"../Resources/3DModels/AnimMesh/4QCharacter_idle_ani/4QCharacter_idle_ani.fbm/BaseColor.png", nullptr, &_temp4QSRV));
//	
//		//FallFlatMonster
//		HR(DirectX::CreateWICTextureFromFile(LowDX11Storage::GetInstance()->_device,
//			LowDX11Storage::GetInstance()->_deviceContext,
//			L"../Resources/3DModels/AnimMesh/FallFlatMonster/FallFlatMonster.fbm/Mutant_diffuse.png", nullptr, &_tempFallFlatMonsterSRV));
//	
//	}
//
//	void MultimaterialMesh::UpdateConstantBufferBase(Pg::Data::CameraData* camData, DirectX::XMFLOAT4X4 worldMat)
//	{
//		using Pg::Graphics::Helper::MathHelper;
//
//		//기반 Struct채우기.
//		DirectX::XMMATRIX tWorldMat = DirectX::XMLoadFloat4x4(&worldMat);
//		DirectX::XMMATRIX tWorldInvTransposeMat = DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(nullptr, tWorldMat));
//
//		_constantBufferStruct->gCBuf_World = tWorldMat;
//		_constantBufferStruct->gCBuf_WorldInvTranspose = tWorldInvTransposeMat;
//
//		DirectX::XMFLOAT4X4 tView = MathHelper::PG2XM_FLOAT4X4(camData->_viewMatrix);
//		DirectX::XMMATRIX tViewMat = DirectX::XMLoadFloat4x4(&tView);
//
//		DirectX::XMFLOAT4X4 tProj = MathHelper::PG2XM_FLOAT4X4(camData->_projMatrix);
//		DirectX::XMMATRIX tProjMat = DirectX::XMLoadFloat4x4(&tProj);
//
//		_constantBufferStruct->gCBuf_WorldView = DirectX::XMMatrixMultiply(tWorldMat, tViewMat);
//
//		DirectX::XMMATRIX tWVP = DirectX::XMMatrixMultiply(tWorldMat, DirectX::XMMatrixMultiply(tViewMat, tProjMat));
//		_constantBufferStruct->gCBuf_WorldViewProj = tWVP;
//		_constantBufferStruct->gCBuf_CameraPositionW = MathHelper::PG2XM_FLOAT3(camData->_position);
//
//		//Updating Subresource (Map / UnMap으로 대체됨!)
//	}
//
//#pragma endregion Unchanged
//
//	void MultimaterialMesh::Render(Pg::Data::CameraData* camData)
//	{
//		//그냥 모두 다 Looping 하게 재생!
//		BoneTransformUpdate();
//		RenderScene(camData);
//	}
//
//	void MultimaterialMesh::ImportSkinnedAsset(const std::string& filePath)
//	{
//		//Skinned
//		_importer = new Assimp::Importer;
//
//		_vertexBuffer = nullptr;
//		_indexBuffer = nullptr;
//		_numVertices = 0;
//		_numIndices = 0;
//		meshes.clear();
//
//		scene = _importer->ReadFile(filePath.c_str(),
//			aiProcess_Triangulate |
//			aiProcess_ConvertToLeftHanded | aiProcess_JoinIdenticalVertices | aiProcess_GenBoundingBoxes |
//			aiProcess_CalcTangentSpace | aiProcess_PopulateArmatureData |
//			aiProcess_GenSmoothNormals | aiProcess_SortByPType | aiProcess_FixInfacingNormals | aiProcess_LimitBoneWeights);
//
//		assert(scene != nullptr);
//
//		//Mesh의 Global Inverse Transform 할당.
//			//Global Inverse 할당!
//		DirectX::SimpleMath::Matrix tGlobalTrans = MathHelper::AI2SM_MATRIX(scene->mRootNode->mTransformation);
//		//기존
//		//MathHelper::DecomposeAssembleMatrix(tGlobalTrans);
//		tGlobalTrans = tGlobalTrans.Transpose();
//
//		DirectX::XMVECTOR tDet = DirectX::XMVectorZero();
//		_meshGlobalInverseTransform = DirectX::XMMatrixInverse(&tDet, tGlobalTrans);
//
//		//_meshGlobalInverseTransform = 
//
//		for (uint32_t i = 0; i < scene->mNumMeshes; i++)
//		{
//			aiMesh* m = scene->mMeshes[i];
//			_numVertices += m->mNumVertices;
//			_numIndices += m->mNumFaces * 3;
//		}
//
//		if (_numVertices == 0) return;
//
//		//Skinned Data를 위해서 구조체 처리.
//		//셰이더에 넣을 버퍼 처리를 위해서 Vector Resize.
//		_boneTransformVector.resize(100);
//		std::fill(_boneTransformVector.begin(), _boneTransformVector.end(), DirectX::SimpleMath::Matrix::Identity);
//
//		SetupRenderUsageMesh();
//		_vertexBoneVector.resize(_numVertices); //Vertex Bone Vector : 미리 모델의 전체 VerticeCount만큼 Resize.
//		SetupBoneData(_vertexBoneVector, scene);
//
//		//지금까지 Bone Index/Weight Binding을 위해, 인덱스 카운팅 도입.
//		UINT tTotalElapsedVertexCount = 0;
//		UINT tTotalElapsedIndiceCount = 0;
//
//		LayoutDefine::Vin1stSkinned* vertices = new LayoutDefine::Vin1stSkinned[_numVertices];
//		int32_t* indices = new int32_t[_numIndices];
//		uint32_t vid = 0, iid = 0;
//		for (uint32_t i = 0; i < scene->mNumMeshes; i++)
//		{
//			aiMesh* m = scene->mMeshes[i];
//			//tTotalElapsedVertexCount += _meshEntriesVector[i].BaseVertex;
//			tTotalElapsedVertexCount = _meshEntriesVector[i]._baseVertex;
//			tTotalElapsedIndiceCount = _meshEntriesVector[i]._baseIndex;
//			for (uint32_t j = 0; j < m->mNumVertices; j++)
//			{
//				//문제 생기면 MultimaterialMesh (원본 참고)
//				auto& pos = m->mVertices[j];
//				auto& norm = m->mNormals[j];
//				auto& tan = m->mTangents[j];
//				auto& texUV = m->mTextureCoords[0][j];
//				unsigned int tMeshMatID = m->mMaterialIndex;
//				//일단은 하드코딩됨.
//
//				vertices[vid + j]._posL = DirectX::XMFLOAT3{ pos.x, pos.y, pos.z };
//				vertices[vid + j]._alpha = 1.f;
//				vertices[vid + j]._normalL = DirectX::XMFLOAT3{ norm.x, norm.y, norm.z };
//				vertices[vid + j]._tangentL = DirectX::XMFLOAT3{ tan.x, tan.y, tan.z };
//				vertices[vid + j]._color = DirectX::XMFLOAT3{ 1.0f,1.0f, 1.0f};
//				vertices[vid + j]._meshMatID = tMeshMatID;
//				vertices[vid + j]._tex = DirectX::XMFLOAT2{ texUV.x, texUV.y };
//
//				//_uvSet2도 역시 하드코딩.
//				//vertices[vid + j]._uvSet2 = { 0.f,0.f };
//
//				//일단 LightMapUV도 FBX딴에서 들어오는 것은 확인했지만, 일단은 파싱에서 받지 않는다.
//				vertices[vid + j]._lightmapUV = { 0.f, 0.f };
//
//				vertices[vid + j]._blendIndice0 = _vertexBoneVector.at(j + tTotalElapsedVertexCount).IDs[0];
//				vertices[vid + j]._blendIndice1 = _vertexBoneVector.at(j + tTotalElapsedVertexCount).IDs[1];
//				vertices[vid + j]._blendIndice2 = _vertexBoneVector.at(j + tTotalElapsedVertexCount).IDs[2];
//				vertices[vid + j]._blendIndice3 = _vertexBoneVector.at(j + tTotalElapsedVertexCount).IDs[3];
//
//				//vertices[vid + j].blendIndice0 = _vertexBoneVector.at(j + tTotalElapsedVertexCount).IDs[0] + tTotalElapsedIndiceCount;
//				//vertices[vid + j].blendIndice1 = _vertexBoneVector.at(j + tTotalElapsedVertexCount).IDs[1] + tTotalElapsedIndiceCount;
//				//vertices[vid + j].blendIndice2 = _vertexBoneVector.at(j + tTotalElapsedVertexCount).IDs[2] + tTotalElapsedIndiceCount;
//				//vertices[vid + j].blendIndice3 = _vertexBoneVector.at(j + tTotalElapsedVertexCount).IDs[3] + tTotalElapsedIndiceCount;
//
//				vertices[vid + j]._blendWeight0 = _vertexBoneVector.at(j + tTotalElapsedVertexCount).Weights[0];
//				vertices[vid + j]._blendWeight1 = _vertexBoneVector.at(j + tTotalElapsedVertexCount).Weights[1];
//				vertices[vid + j]._blendWeight2 = _vertexBoneVector.at(j + tTotalElapsedVertexCount).Weights[2];
//
//				//vertices[vid + j].blendWeight0 = 0.2f;
//				//vertices[vid + j].blendWeight1 = 0.2f;
//				//vertices[vid + j].blendWeight2 = 0.2f;
//
//				//vertices[vid + j].blendIndice0 = _vertexBoneVector.at(j).IDs[0];
//				//vertices[vid + j].blendIndice1 = _vertexBoneVector.at(j).IDs[1];
//				//vertices[vid + j].blendIndice2 = _vertexBoneVector.at(j).IDs[2];
//				//vertices[vid + j].blendIndice3 = _vertexBoneVector.at(j).IDs[3];
//				//
//				//vertices[vid + j].blendWeight0 = _vertexBoneVector.at(j).Weights[0];
//				//vertices[vid + j].blendWeight1 = _vertexBoneVector.at(j).Weights[1];
//				//vertices[vid + j].blendWeight2 = _vertexBoneVector.at(j).Weights[2];
//			}
//
//			for (uint32_t j = 0; j < m->mNumFaces; j++)
//			{
//				indices[iid + j * 3] = m->mFaces[j].mIndices[0];
//				indices[iid + j * 3 + 1] = m->mFaces[j].mIndices[1];
//				indices[iid + j * 3 + 2] = m->mFaces[j].mIndices[2];
//			}
//
//			Mesh mesh;
//			mesh.startVertex = vid;
//			mesh.startIndex = iid;
//			mesh.numVertices = m->mNumVertices;
//			mesh.numIndices = m->mNumFaces * 3;
//			meshes.push_back(mesh);
//
//			vid += m->mNumVertices;
//			iid += m->mNumFaces * 3;
//		}
//
//		do
//		{
//			CD3D11_BUFFER_DESC vbDesc(
//				_numVertices * sizeof(LayoutDefine::Vin1stSkinned),
//				D3D11_BIND_VERTEX_BUFFER);
//			D3D11_SUBRESOURCE_DATA vbData = { vertices, 0, 0 };
//			if (S_OK != _device->CreateBuffer(&vbDesc, &vbData, &_vertexBuffer))
//				assert(false);
//
//			CD3D11_BUFFER_DESC ibDesc(
//				_numIndices * sizeof(uint32_t),
//				D3D11_BIND_INDEX_BUFFER);
//			D3D11_SUBRESOURCE_DATA ibData = { indices, 0, 0 };
//			if (S_OK != _device->CreateBuffer(&ibDesc, &ibData, &_indexBuffer))
//				assert(false);
//
//		} while (0);
//
//		delete[] vertices;
//		delete[] indices;
//
//		if (nullptr == _vertexBuffer || nullptr == _indexBuffer)
//		{
//			if (nullptr != _vertexBuffer) _vertexBuffer->Release();
//			if (nullptr != _indexBuffer) _indexBuffer->Release();
//		}
//
//	}
//
//	void MultimaterialMesh::RenderScene(Pg::Data::CameraData* camData)
//	{
//		if (meshes.empty() || nullptr == scene)
//		{
//			assert(false);
//		}
//
//		auto _DXStorage = LowDX11Storage::GetInstance();
//		_devCon->OMSetRenderTargets(1, &(_DXStorage->_mainRTV), (_DXStorage->_depthStencilView));
//
//		_devCon->IASetInputLayout(LayoutDefine::GetSkinned1stLayout());
//		_devCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//		_devCon->RSSetState(_rasterizerState);
//
//		_devCon->VSSetShader(_vertexShader, nullptr, 0);
//		_devCon->PSSetShader(_pixelShader, nullptr, 0);
//		_devCon->PSSetSamplers(0, 1, &_samplerState);
//
//		UINT strides[] = { sizeof(LayoutDefine::Vin1stSkinned) };
//		UINT offsets[] = { 0 };
//
//		_devCon->IASetVertexBuffers(0, 1, &_vertexBuffer, strides, offsets);
//		_devCon->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
//
//		//1/100으로 줄여서 렌더링할 것이다. -> 일단은 World Matrix를 Identity로!
//		///커스텀 위치 조정.
//		///현재 렌더 로직 떄문에, 이 역시 적용되지 않는 상황이다!
//		DirectX::XMFLOAT3 tPosition = { -1.0f, 3.0f, 0.0f };
//		DirectX::XMVECTOR tPosVec = DirectX::XMLoadFloat3(&tPosition);
//
//
//		DirectX::XMFLOAT4 tRotQuat = { 0.0f, 0.0f, 0.0f, 1.0f };
//		DirectX::XMVECTOR tRotQuatVec = DirectX::XMLoadFloat4(&tRotQuat);
//
//		//0.01 스케일링 적용.
//		//이 모델이 되따 크다.
//		//DirectX::XMFLOAT3 tScale = { 0.0001f, 0.0001f, 0.0001f };
//		//DirectX::XMFLOAT3 tScale = { 0.001f, 0.001f, 0.001f };
//		DirectX::XMFLOAT3 tScale = { 0.01f, 0.01f, 0.01f };
//		//DirectX::XMFLOAT3 tScale = { 0.03f, 0.03f, 0.03f };
//		//DirectX::XMFLOAT3 tScale = { 0.1f, 0.1f, 0.1f };
//		//DirectX::XMFLOAT3 tScale = {1.0f,1.0f, 1.0f};
//		DirectX::XMVECTOR tScaleVec = DirectX::XMLoadFloat3(&tScale);
//
//		DirectX::XMMATRIX tWorldMatScaled = DirectX::XMMatrixAffineTransformation(tScaleVec, tPosVec, tRotQuatVec, tPosVec);
//
//		DirectX::XMFLOAT4X4 tWorldMatScaledFF;
//		DirectX::XMStoreFloat4x4(&tWorldMatScaledFF, tWorldMatScaled);
//
//		//그런가..? 확실한 것은, 들어갈 때는 올바르게 (DX 기준) 행렬이 매개변수로 들어가는 것을 전제로 하고 있다. 
//		//다시 생각해봐라! Transpose 여부.
//
//		/// Skinned Constant Buffer 업데이트.
//		//옮겨졌던 모든 스키닝을 Vector로 옮기기. (구조체로)
//
//		for (int i = 0; i < 100; i++)
//		{
//			_skinnedCBuffer->gCBuf_Bones[i] = _boneTransformVector[i];
//		}
//
//		D3D11_MAPPED_SUBRESOURCE res;
//		ZeroMemory(&res, sizeof(D3D11_MAPPED_SUBRESOURCE));
//
//		HRESULT hr = _devCon->Map(_constantBuffer[1], 0, D3D11_MAP_WRITE_DISCARD, 0, &res);
//		if (hr == S_OK)
//		{
//			ConstantBufferDefine::cbPerObjectSkinned* data = reinterpret_cast<ConstantBufferDefine::cbPerObjectSkinned*>(res.pData);
//			*(data) = *_skinnedCBuffer;
//
//			_devCon->Unmap(_constantBuffer[1], 0);
//		}
//		else
//		{
//			assert(false);
//		}
//		///
//		_devCon->VSSetConstantBuffers(1, 1, &(_constantBuffer[1]));
//
//		/// Normal Constant Buffer 업데이트.
//		//한꺼번에 그려야 할 것. Mesh들 사이는 나누지만, 그 이상은 X!
//
//		D3D11_MAPPED_SUBRESOURCE res2;
//		ZeroMemory(&res2, sizeof(D3D11_MAPPED_SUBRESOURCE));
//		if (S_OK == _devCon->Map(_constantBuffer[0], 0, D3D11_MAP_WRITE_DISCARD, 0, &res2))
//		{
//			ConstantBufferDefine::cbPerObjectBase* data = reinterpret_cast<ConstantBufferDefine::cbPerObjectBase*>(res2.pData);
//			
//			UpdateConstantBufferBase(camData, tWorldMatScaledFF);
//			*(data) = *_constantBufferStruct;
//
//			_devCon->Unmap(_constantBuffer[0], 0);
//		}
//		else
//		{
//			assert(false);
//		}
//		/// 
//		_devCon->VSSetConstantBuffers(0, 1, &(_constantBuffer[0]));
//
//		//render_scene_node(camData, scene->mRootNode, tWorldMatScaledFF);
//		RenderSkinnedNodes(camData); // 내부에서
//
//		//원래 위에 있었음.
//		//BufferMemory 매핑만 설정. -> 이제 Skinned니까 2개!
//
//
//		//VS/PS Unbind.
//		_devCon->VSSetShader(nullptr, nullptr, 0);
//		_devCon->PSSetShader(nullptr, nullptr, 0);
//	}
//
//
//
//	void MultimaterialMesh::SetupBoneData(std::vector<RenderPrepVertexBone>& vBoneList, const aiScene* scene)
//	{
//		for (unsigned int i = 0; i < scene->mNumMeshes; i++)
//		{
//			aiMesh* aMesh = scene->mMeshes[i];
//
//			//Vertex 로드는 인덱스가 변하지 않기 때문에, 
//			//제대로 적용됨
//			if (aMesh->HasBones())
//			{
//				SetupRenderBones(i, aMesh, vBoneList);
//			}
//		}
//	}
//
//	void MultimaterialMesh::SetupRenderBones(unsigned int index, aiMesh* mesh, std::vector<RenderPrepVertexBone>& vBoneList)
//	{
//		for (unsigned int i = 0; i < mesh->mNumBones; i++) {
//
//			unsigned int BoneIndex = 0;
//
//			// Obtain the bone name.
//			std::string BoneName(mesh->mBones[i]->mName.C_Str());
//
//			// If bone isn't already in the map. 
//			if (_mappedBones.find(BoneName) == _mappedBones.end())
//			{
//				// Set the bone ID to be the current total number of bones. 
//				BoneIndex = _formationNumBone;
//
//				// Increment total bones. 
//				_formationNumBone++;
//
//				// Push new bone info into bones vector. 
//				RenderPrepBoneInfo tBi;
//				_renderBoneInfoVector.push_back(tBi);
//			}
//			else {
//				// Bone ID is already in map. 
//				BoneIndex = _mappedBones[BoneName];
//			}
//
//			_mappedBones[BoneName] = BoneIndex;
//
//			// Obtains the offset matrix which transforms the bone from mesh space into bone space. 
//			Matrix tBoneOffset = MathHelper::AI2SM_MATRIX(mesh->mBones[i]->mOffsetMatrix);
//			///기존
//			//MathHelper::DecomposeAssembleMatrix(tBoneOffset);
//			_renderBoneInfoVector[BoneIndex]._boneOffset = tBoneOffset.Transpose();
//
//			//{
//			//	using namespace DirectX;
//			//	Matrix rotationMatrix = Matrix::CreateFromYawPitchRoll(XMConvertToRadians(0.0f), XMConvertToRadians(0.f), XMConvertToRadians(0.0f));
//			//	_renderBoneInfoVector[BoneIndex]._boneOffset *= rotationMatrix;
//			//}
//			//MathHelper::DecomposeAssembleMatrix(_renderBoneInfoVector[BoneIndex]._boneOffset);
//
//			// Iterate over all the affected vertices by this bone i.e weights. 
//			for (unsigned int j = 0; j < mesh->mBones[i]->mNumWeights; j++) {
//
//				// Obtain an index to the affected vertex within the array of vertices.
//				unsigned int VertexID = _meshEntriesVector[index]._baseVertex + mesh->mBones[i]->mWeights[j].mVertexId;
//
//				// The value of how much this bone influences the vertex. 
//				float Weight = mesh->mBones[i]->mWeights[j].mWeight;
//
//				// Insert bone data for particular vertex ID. A maximum of 4 bones can influence the same vertex. 
//				vBoneList[VertexID].AddBoneData(BoneIndex, Weight);
//			}
//		}
//		assert(mesh);
//	}
//
//	void MultimaterialMesh::BoneTransformUpdate()
//	{
//		//처음에 아무것도 없으면 아예 출력이 되지 않으니, T-Pose Animation을 기본으로 잡고 코드를 짜야 한다.
//
//		//현재 3DModel은 3개인 상태 : 0 / 1 / 2 중 2가 T-Pose.
//
//		static short tChoice = 0;
//
//		aiAnimation* tAnim = nullptr;
//		tAnim = scene->mAnimations[tChoice];
//
//		//절대로 일단은 정해져 있는 Tick 수 넘어가지 않게 -> 나머지 연산을 할것. 
//		static double tPlayTickDur = 0;
//		//tPlayTickDur += 1;
//		tPlayTickDur += 0.1;
//
//
//		//if (_tempInput->GetKeyDown(API::Input::eKeyCode::MouseLeft))
//		//{
//		//	tPlayTickDur += 0.1;
//		//
//		//	std::string tMsg = "Tick : " + std::to_string(tPlayTickDur);
//		//	PG_TRACE(tMsg.c_str());
//		//}
//
//		double tInwardTick = fmod(tPlayTickDur, tAnim->mDuration);
//		assert(tInwardTick < tAnim->mDuration);
//
//		DirectX::XMFLOAT4X4 tDefaultMat =
//		{ 1.0f,0.0f,0.0f,0.0f,
//		  0.0f,1.0f,0.0f,0.0f,
//		  0.0f,0.0f,1.0f,0.0f,
//		  0.0f,0.0f,0.0f,1.0f };
//
//		//ReadNodeHierarchy(tInwardTick, scene->mRootNode, tAnim, tDefaultMat);
//		ReadNodeHierarchy(tInwardTick, scene->mRootNode, tAnim, tDefaultMat);
//
//		assert(_formationNumBone < 100);
//		// Populates transforms vector with new bone transformation matrices. 
//		for (unsigned int i = 0; i < _formationNumBone; i++)
//		{
//			//_boneTransformVector[i] = _renderBoneInfoVector[i]._finalTransformation;
//			_boneTransformVector[i] = _renderBoneInfoVector[i]._finalTransformation.Transpose();
//			//_boneTransformVector[i] = Matrix::Identity;
//		}
//	}
//
//	void MultimaterialMesh::ReadNodeHierarchy(double animTick, const aiNode* pNode, const aiAnimation* pAnim, DirectX::SimpleMath::Matrix parentTransform)
//	{
//		//using Matrix = DirectX::SimpleMath::Matrix;
//		//using Quaternion = DirectX::XMFLOAT4;
//		//DX 쿼터니언 : X,Y,Z (벡터), W(스칼라)
//		// XMFLOAT4 / XMVECTOR
//
//		// Obtain the name of the current node 
//		std::string tNodeName(pNode->mName.C_Str());
//
//		// Use the first animation 
//		const aiAnimation* pAnimation = pAnim;
//
//		// Obtain transformation relative to node's parent. 
//		Matrix tNodeTransformation = MathHelper::AI2SM_MATRIX(pNode->mTransformation);
//
//		///기존
//		//MathHelper::DecomposeAssembleMatrix(tNodeTransformation);
//		//MathHelper::DecomposeAssembleMatrix(tNodeTransformation);
//		//tNodeTransformation.Invert(tNodeTransformation);
//		tNodeTransformation = tNodeTransformation.Transpose();
//		{
//			//using namespace DirectX;
//			//Matrix rotationMatrix = Matrix::CreateFromYawPitchRoll(XMConvertToRadians(0.0f), XMConvertToRadians(180.0f), XMConvertToRadians(0.0f));
//			//
//			//tNodeTransformation *= tNodeTransformation;
//		}
//
//		///Rotation, 하지만 이는 지속성이 없는 코드.
//		{
//			using namespace DirectX;
//			tNodeTransformation = tNodeTransformation.Transpose();
//
//			Matrix rotationMatrix = Matrix::CreateFromYawPitchRoll(XMConvertToRadians(0.0f), XMConvertToRadians(0.0f), XMConvertToRadians(0.0f));
//			tNodeTransformation = rotationMatrix;
//			//tNodeTransformation *= rotationMatrix;
//		}
//
//		//여기서 Decompose를 시행안하기는 했다. 문제시 보기.
//
//		const aiNodeAnim* pNodeAnim = nullptr;
//
//		// Find the animation channel of the current node. -> 나중에 미리 매핑해놓을 수 있음!
//		//다만, Animation에 해당되지 않는 Node들은 일단 이 로직이 없으면 실행되지 않을 것이다! 이는 염두.
//		for (unsigned i = 0; i < pAnimation->mNumChannels; i++)
//		{
//			const aiNodeAnim* pNodeAnimIndex = pAnimation->mChannels[i];
//			std::string tNodeAnimName(pNodeAnimIndex->mNodeName.C_Str());
//
//			// If there is a match for a channel with the current node's name, then we've found the animation channel. 
//			if (tNodeAnimName.compare(tNodeName) == 0)
//			{
//				pNodeAnim = pNodeAnimIndex;
//			}
//		}
//
//		if (pNodeAnim)
//		{
//			// Interpolate rotation and generate rotation transformation matrix
//			Quaternion RotationQ;
//			//CalcInterpolatedRotation(RotationQ, animationTime, pNodeAnim);
//			CalcInterpolatedRotation(RotationQ, animTick, pNodeAnim);
//
//			Matrix RotationM = DirectX::XMMatrixRotationQuaternion(RotationQ);
//			//RotationM = RotationM.Transpose(); 
//
//			// Interpolate translation and generate translation transformation matrix
//			DirectX::SimpleMath::Vector3 Translation;
//			CalcInterpolatedTranslation(Translation, animTick, pNodeAnim);
//			Matrix TranslationM;
//			TranslationM = DirectX::XMMatrixTranslation(Translation.x, Translation.y, Translation.z);
//			//TranslationM = TranslationM.Transpose();
//
//			// Combine the above transformations
//			tNodeTransformation = RotationM * TranslationM;/* *ScalingM;*/
//		}
//
//		Matrix tGlobalTransformation = tNodeTransformation * parentTransform;
//
//		// Apply the final transformation to the indexed bone in the array. 
//		if (_mappedBones.find(tNodeName) != _mappedBones.end())
//		{
//			unsigned int tBoneIndex = _mappedBones[tNodeName];
//
//			_renderBoneInfoVector[tBoneIndex]._finalTransformation =
//				_renderBoneInfoVector[tBoneIndex]._boneOffset *
//				tGlobalTransformation *
//				_meshGlobalInverseTransform;
//		}
//
//		// Do the same for all the node's children. 
//		for (unsigned i = 0; i < pNode->mNumChildren; i++)
//		{
//			ReadNodeHierarchy(animTick, pNode->mChildren[i], pAnim, tGlobalTransformation);
//		}
//	}
//
//	void MultimaterialMesh::CalcInterpolatedRotation(DirectX::SimpleMath::Quaternion& outQuat, double animTick, const aiNodeAnim* pNodeAnim)
//	{
//		//using Matrix = DirectX::SimpleMath::Matrix;
//		//using Quaternion = DirectX::SimpleMath::Quaternion;
//
//		// we need at least two values to interpolate...
//		if (pNodeAnim->mNumRotationKeys == 1)
//		{
//			outQuat = MathHelper::AI2SM_QUATERNION(pNodeAnim->mRotationKeys[0].mValue);
//			return;
//		}
//		// Obtain the current rotation keyframe. 
//		unsigned int RotationIndex = FindRotation(animTick, pNodeAnim);
//
//		// Calculate the next rotation keyframe and check bounds. 
//		unsigned int NextRotationIndex = (RotationIndex + 1);
//		assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);
//
//		// Calculate delta time, i.e time between the two keyframes.
//		float DeltaTime = pNodeAnim->mRotationKeys[NextRotationIndex].mTime - pNodeAnim->mRotationKeys[RotationIndex].mTime;
//
//		// Calculate the elapsed time within the delta time.  
//		float Factor = (animTick - (float)pNodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
//
//		///오류 : Factor가 0~1 사이로 결정되지 않는 오류 확인!
//
//		// Obtain the quaternions values for the current and next keyframe. 
//		const Quaternion StartRotationQ = MathHelper::AI2SM_QUATERNION(pNodeAnim->mRotationKeys[RotationIndex].mValue);
//		const Quaternion EndRotationQ = MathHelper::AI2SM_QUATERNION(pNodeAnim->mRotationKeys[NextRotationIndex].mValue);
//
//		// Interpolate between them using the Factor. 
//		//Quaternion::Slerp(StartRotationQ, EndRotationQ, Factor, outQuat);
//
//		//Cubic Interpolation 쓰기..??
//
//		//TRY:
//		outQuat = MathHelper::QuaternionSlerpNoFlip(StartRotationQ, EndRotationQ, Factor);
//
//		// Normalise and set the reference. 
//		outQuat.Normalize();
//	}
//
//	void MultimaterialMesh::CalcInterpolatedTranslation(DirectX::SimpleMath::Vector3& outVec, double animTick, const aiNodeAnim* pNodeAnim)
//	{
//		// we need at least two values to interpolate...
//		if (pNodeAnim->mNumPositionKeys == 1)
//		{
//			outVec = MathHelper::AI2SM_VECTOR3(pNodeAnim->mPositionKeys[0].mValue);
//			return;
//		}
//
//		unsigned int PositionIndex = FindTranslation(animTick, pNodeAnim);
//		unsigned int NextPositionIndex = (PositionIndex + 1);
//		assert(NextPositionIndex < pNodeAnim->mNumPositionKeys);
//
//		float DeltaTime = pNodeAnim->mPositionKeys[NextPositionIndex].mTime - pNodeAnim->mPositionKeys[PositionIndex].mTime;
//		float Factor = (animTick - (float)pNodeAnim->mPositionKeys[PositionIndex].mTime) / DeltaTime;
//
//		//assert(Factor >= 0.0f && Factor <= 1.0f);
//		const DirectX::SimpleMath::Vector3 Start = MathHelper::AI2SM_VECTOR3(pNodeAnim->mPositionKeys[PositionIndex].mValue);
//		const DirectX::SimpleMath::Vector3 End = MathHelper::AI2SM_VECTOR3(pNodeAnim->mPositionKeys[NextPositionIndex].mValue);
//
//		DirectX::SimpleMath::Vector3 Delta = End - Start;
//		outVec = Start + Factor * Delta;
//	}
//
//	unsigned int MultimaterialMesh::FindRotation(double animTick, const aiNodeAnim* pNodeAnim)
//	{
//		// Check if there are rotation keyframes. 
//		assert(pNodeAnim->mNumRotationKeys > 0);
//
//		// Find the rotation key just before the current animation time and return the index. 
//		for (unsigned int i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++)
//		{
//			//FIX.
//			if (animTick < (float)pNodeAnim->mRotationKeys[i + 1].mTime)
//			{
//				return i;
//			}
//		}
//		assert(0);
//
//		return 0;
//	}
//
//	unsigned int MultimaterialMesh::FindTranslation(double animTick, const aiNodeAnim* pNodeAnim)
//	{
//		assert(pNodeAnim->mNumPositionKeys > 0);
//
//		// Find the translation key just before the current animation time and return the index. 
//		for (unsigned int i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++)
//		{
//			//FIX.
//			if (animTick < (float)pNodeAnim->mPositionKeys[i + 1].mTime)
//			{
//				return i;
//			}
//		}
//		assert(0);
//
//		return 0;
//	}
//
//	void MultimaterialMesh::RenderSkinnedNodes(Pg::Data::CameraData* camData)
//	{
//		//한꺼번에 렌더 함수들 모아놓기. (Mesh들 사이는 DrawCall 나누고!)
//
//		for (int i = 0; i < scene->mNumMeshes; i++)
//		{
//			Mesh& m = meshes[i];
//			aiMesh* tAiMesh = scene->mMeshes[i];
//
//			//이제 SolidRS-DiffuseTexture를 이용하기에, 
//			//Mesh의 인덱스에 따라 PSSetShaderResources를
//			//해당 Mesh의 Material의 인덱스에 맞게 호출한다.
//
//			//_devCon->PSSetShaderResources(0, 1, &(_tempSRVArray[tAiMesh->mMaterialIndex]));
//			//_devCon->PSSetShaderResources(0, 1, &_tempCylinderSRV);
//			_devCon->PSSetShaderResources(0, 1, &_tempFallFlatMonsterSRV);
//			//_devCon->PSSetShaderResources(0, 1, &_tempTimmySRV);
//			//_devCon->PSSetShaderResources(0, 1, &_temp4QSRV);
//			//_devCon->PSSetShaderResources(0, 1, &(_tempSRVArray[0]));
//
//			_devCon->DrawIndexed(m.numIndices, m.startIndex, m.startVertex);
//		}
//	}
//
//	void MultimaterialMesh::SetupRenderUsageMesh()
//	{
//		unsigned int tStartingVertices = 0;
//		unsigned int tStartingIndices = 0;
//
//		//당장 Bone 데이터를 쓰는데 사용하지는 않으나, 실제 D3D 버퍼 출력에 활용되는 정보.
//		_meshEntriesVector.resize(scene->mNumMeshes);
//
//		for (unsigned int i = 0; i < _meshEntriesVector.size(); i++)
//		{
//			// Total mesh indices. 
//			_meshEntriesVector[i]._numIndices = scene->mMeshes[i]->mNumFaces * 3;
//			_meshEntriesVector[i]._numVertices = scene->mMeshes[i]->mNumVertices;
//
//			// Set the base vertex of this mesh (initial vertex for this mesh within the vertices array) to the current total vertices. 
//			_meshEntriesVector[i]._baseVertex = tStartingVertices;
//
//			// Set the base index of this mesh (initial index for this mesh within the indices array) to the current total indices. 
//			_meshEntriesVector[i]._baseIndex = tStartingIndices;
//
//			// Increment total vertices and indices. 
//			tStartingVertices += scene->mMeshes[i]->mNumVertices;
//			tStartingIndices += _meshEntriesVector[i]._numIndices;
//		}
//	}
//
//}
