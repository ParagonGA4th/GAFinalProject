#include "SkinnedMultiMatMesh.h"
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

//VWPS Help.
#include "Joint.h"
#include "Animation.h"
#include "Skeleton.h"
#include "BonePerVertex.h"

#include "../ParagonUtil/Log.h"

#ifdef _DEBUG
#pragma comment(lib,"..\\Builds\\x64\\Debug\\ParagonAPI.lib")
#else
#pragma comment(lib,"..\\Builds\\x64\\Release\\ParagonAPI.lib")
#endif // _DEBUG

namespace Pg::Graphics
{

	using Pg::Graphics::Manager::GraphicsResourceManager;
	using Pg::Data::Enums::eAssetDefine;

	//static 
	SkinnedMultiMatMesh::SkinnedMultiMatMesh(const std::string& filePath)
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

	SkinnedMultiMatMesh::~SkinnedMultiMatMesh()
	{
		delete _constantBufferStruct;
		delete _skinnedCBuffer;
	}

	void SkinnedMultiMatMesh::CreateSamplerState()
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

	void SkinnedMultiMatMesh::CreateVertexPixelShader()
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

	void SkinnedMultiMatMesh::CreateRasterizerState()
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

	void SkinnedMultiMatMesh::CreateConstantBuffer()
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

	void SkinnedMultiMatMesh::ImportSkinnedAsset(const std::string& filePath)
	{
		//Skinned
		_importer = new Assimp::Importer;

		scene = _importer->ReadFile(filePath.c_str(),
			aiProcess_Triangulate |
			aiProcess_ConvertToLeftHanded | aiProcess_JoinIdenticalVertices | aiProcess_GenBoundingBoxes |
			aiProcess_CalcTangentSpace | aiProcess_PopulateArmatureData |
			aiProcess_GenSmoothNormals | aiProcess_SortByPType | aiProcess_FixInfacingNormals | aiProcess_EmbedTextures | aiProcess_LimitBoneWeights);

		assert(scene != nullptr);

		std::vector<LayoutDefine::Vin1stSkinned> vertices;
		std::vector<UINT> indices;

		aiMesh* mesh = scene->mMeshes[0]; //일단은 Mesh 하나만 호환가능하게.
		_numVertices = mesh->mNumVertices;

		//Vertex information
		// For every vertex
		for (int i = 0; i < _numVertices; i++)
		{
			LayoutDefine::Vin1stSkinned tempVertex;
		
			auto& pos = mesh->mVertices[i];
			auto& norm = mesh->mNormals[i];
			auto& tan = mesh->mTangents[i];
			auto& uv = mesh->mTextureCoords[0][i];

			tempVertex.posL = DirectX::XMFLOAT3{ pos.x, pos.y, pos.z };
			tempVertex.normalL = DirectX::XMFLOAT3{ norm.x, norm.y, norm.z };
			tempVertex.tangentL = DirectX::XMFLOAT3{ tan.x, tan.y, tan.z };
			tempVertex.color = DirectX::XMFLOAT4{ 1.0f,1.0f, 1.0f, 1.0f };
			tempVertex.tex = DirectX::XMFLOAT3{ uv.x, uv.y, uv.z };
			tempVertex.matID = mesh->mMaterialIndex;

			// Save the vertex
			vertices.push_back(tempVertex);
		}

		// For every face, save its indices
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			indices.push_back(mesh->mFaces[i].mIndices[0]);
			indices.push_back(mesh->mFaces[i].mIndices[1]);
			indices.push_back(mesh->mFaces[i].mIndices[2]);
		}
		_numIndices = indices.size();

		//미리 Bone 마련.
		std::vector<BonePerVertex> _bonePerVertexVec;
		_bonePerVertexVec.resize(mesh->mNumVertices);

		Skeleton* tempSkeleton = nullptr;
		// If the mesh in the scene has more than 0 bones
		// We will then contstruct a skeleton
		if (mesh->mNumBones > 0)
		{
			tempSkeleton = new Skeleton();

			/*
			The reason we want to use GlobalMeshInverse and InverseBindpose (later in the code)
			is to bring us from the skeletons local space to each bones local space.
			*/
			// Collect the global mesh inverse transform from the scene
			tempSkeleton->SetGlobalMeshInverseTransformMatrix(DirectX::XMMATRIX(&scene->mRootNode->mTransformation.Inverse().a1));

			// For each bone
			for (unsigned int i = 0; i < mesh->mNumBones; i++)
			{
				int boneIndex = 0;

				// Get the bones name
				std::string boneName = mesh->mBones[i]->mName.data;

				// Check if the bone is already in our map, if not, we create a new joint and saves it, together with an index
				if (tempSkeleton->boneMap.find(boneName) == tempSkeleton->boneMap.end())
				{
					boneIndex = tempSkeleton->GetBoneAmount();
					tempSkeleton->AddBoneAmount();
					tempSkeleton->boneMap[boneName] = boneIndex;

					Joint joint;

					// Sets the inverse bind pose matrix for each bone
					joint.SetInverseBindPoseMatrix(DirectX::XMMATRIX(&mesh->mBones[i]->mOffsetMatrix.a1));
					joint.SetFinalTransformMatrix(DirectX::XMMatrixIdentity());
					joint.SetName(boneName);

					// Pushes back the bone 
					tempSkeleton->AddNewBone(joint);
				}
				else
				{
					boneIndex = tempSkeleton->boneMap[boneName];
				}

				// For each bones amount of weights, which is how many vertices each bone is affecting
				for (unsigned int j = 0; j < mesh->mBones[i]->mNumWeights; j++)
				{
					// Collect which vertices the bone affects
					int vertexId = mesh->mBones[i]->mWeights[j].mVertexId;
					// Saves the weight
					float weight = mesh->mBones[i]->mWeights[j].mWeight;

					_bonePerVertexVec[vertexId].AddBoneData(boneIndex, weight);
				}
			}
		}

		//BonePerVertex가 다 들어간 상황.
		//이제, 실제로 Vertex에 넣어야 한다!
		for (int i = 0; i < _numVertices; i++)
		{
			vertices[i].blendIndice0 = _bonePerVertexVec[i].IDs[0];
			vertices[i].blendIndice1 = _bonePerVertexVec[i].IDs[1];
			vertices[i].blendIndice2 = _bonePerVertexVec[i].IDs[2];
			vertices[i].blendIndice3 = _bonePerVertexVec[i].IDs[3];

			vertices[i].blendWeight0 = _bonePerVertexVec[i].Weights[0];
			vertices[i].blendWeight1 = _bonePerVertexVec[i].Weights[1];
			vertices[i].blendWeight2 = _bonePerVertexVec[i].Weights[2];
		}

		do
		{
			CD3D11_BUFFER_DESC vbDesc(
				_numVertices * sizeof(LayoutDefine::Vin1stSkinned),
				D3D11_BIND_VERTEX_BUFFER);
			D3D11_SUBRESOURCE_DATA vbData = { vertices.data() , 0, 0};
			if (S_OK != _device->CreateBuffer(&vbDesc, &vbData, &_vertexBuffer))
				assert(false);

			CD3D11_BUFFER_DESC ibDesc(
				_numIndices * sizeof(uint32_t),
				D3D11_BIND_INDEX_BUFFER);
			D3D11_SUBRESOURCE_DATA ibData = { indices.data(), 0, 0 };
			if (S_OK != _device->CreateBuffer(&ibDesc, &ibData, &_indexBuffer))
				assert(false);

		} while (0);
	}

	void SkinnedMultiMatMesh::LoadHardCodedSRVs()
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

		//Timmy
		HR(DirectX::CreateWICTextureFromFile(LowDX11Storage::GetInstance()->_device,
			LowDX11Storage::GetInstance()->_deviceContext,
			L"../Resources/3DModels/Animated/Textures/Timmy_Diffuse.png", nullptr, &_tempTimmySRV));

		//Direct3D Cylinder
		HR(DirectX::CreateDDSTextureFromFile(LowDX11Storage::GetInstance()->_device,
			LowDX11Storage::GetInstance()->_deviceContext,
			L"../Resources/3DModels/Animated/Textures/WoodCrate01.dds", nullptr, &_tempCylinderSRV));
	}

	void SkinnedMultiMatMesh::Render(Pg::Data::CameraData* camData)
	{
		RenderScene(camData);
	}

	void SkinnedMultiMatMesh::RenderScene(Pg::Data::CameraData* camData)
	{

	}

	void SkinnedMultiMatMesh::ReadSceneHierarchy(unsigned int keyframe, const aiScene* scene, aiNode* node, DirectX::XMMATRIX parentTransform, Animation* animation, Skeleton* skeleton)
	{
		// Name of the current node
		std::string nodeName = node->mName.data;

		// First animation in the scene
		const aiAnimation* assimpAnimation = scene->mAnimations[0];

		// The nodes transformation relative to its parent
		// If this node is affected in the animation, it will be overwritten later
		DirectX::XMMATRIX nodeTransformation = DirectX::XMMATRIX(&node->mTransformation.a1);

		// Check if the node is affected by the animation
		const aiNodeAnim* nodeAnim = CheckNodeAnim(assimpAnimation, nodeName);

		// If its not null, the nodes transformation will be overwritten here
		if (nodeAnim)
		{
			aiVector3D scaling;
			aiQuaternion rotation;
			aiVector3D translation;

			// Get the position, rotation and scaling at the current keyframe
			GetPositionAtKeyframe(translation, keyframe, nodeAnim);
			GetRotationAtKeyframe(rotation, keyframe, nodeAnim);
			GetScaleAtKeyframe(scaling, keyframe, nodeAnim);

			// Convert the data from Assimps structure to DirectX XMVector
			DirectX::XMVECTOR scale = DirectX::XMVectorSet(scaling.x, scaling.y, scaling.z, 1.0f);
			DirectX::XMVECTOR trans = DirectX::XMVectorSet(translation.x, translation.y, translation.z, 1.0f);
			DirectX::XMVECTOR rotate = DirectX::XMVectorSet(rotation.x, rotation.y, rotation.z, 1.0f);
			DirectX::XMVECTOR origin = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

			// Create a transformation matrix and transpose it, for multiplication purposes
			nodeTransformation = DirectX::XMMatrixAffineTransformation(scale, origin, rotate, trans);
			nodeTransformation = DirectX::XMMatrixTranspose(nodeTransformation);
		}

		// Multiply the transformation with its parent
		DirectX::XMMATRIX globalTransform = parentTransform * nodeTransformation;

		// If the current node is also a bone in the skeleton
		if (skeleton->boneMap.find(nodeName) != skeleton->boneMap.end())
		{
			unsigned int boneIndex = skeleton->boneMap[nodeName];

			// Save these to the animation struct, just for future reference
			//animation->GetBoneAtIndex(boneIndex).SetGlobalTransform(globalTransform);
			animation->GetBoneAtIndex(boneIndex).SetInverseBindPoseMatrix(skeleton->GetBone(boneIndex).GetInverseBindPoseMatrix());

			// Combine the transformation with the skeletons globalmeshInverse and the bones inverse bind pose
			DirectX::XMMATRIX final = skeleton->GetGlobalMeshInverseMatrix() * globalTransform * skeleton->GetBone(boneIndex).GetInverseBindPoseMatrix();

			// Save the transformation to the corresponding bone in the animation class
			animation->GetBoneAtIndex(boneIndex).SetFinalTransformMatrix(final);
		}

		// Repeat the function recursively for each child
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			ReadSceneHierarchy(keyframe, scene, node->mChildren[i], globalTransform, animation, skeleton);
		}

	}

	const aiNodeAnim* SkinnedMultiMatMesh::CheckNodeAnim(const aiAnimation* animation, const std::string nodeName)
	{
		// For every channel/curve in the current animation
		for (unsigned int i = 0; i < animation->mNumChannels; i++)
		{
			// Checks if the node responding to this channel, is the same node we are on in the main loop
			const aiNodeAnim* nodeAnim = animation->mChannels[i];
			if (std::string(nodeAnim->mNodeName.data) == nodeName)
			{
				// returns the node
				return nodeAnim;
			}
		}
		// If the node is not in this animation, return null
		return nullptr;
	}
	
	void SkinnedMultiMatMesh::GetPositionAtKeyframe(aiVector3D& position, unsigned int keyframe, const aiNodeAnim* nodeAnim)
	{
		// Check if there are more than 1 key of translation, otherwise return the translation at keyframe 0
		if (nodeAnim->mNumPositionKeys == 1)
		{
			position = nodeAnim->mPositionKeys[0].mValue;
			return;
		}

		// Get the position at the current keyframe
		const aiVector3D& pos = nodeAnim->mPositionKeys[keyframe].mValue;
		position = pos;

	}

	void SkinnedMultiMatMesh::GetRotationAtKeyframe(aiQuaternion& rotation, unsigned int keyframe, const aiNodeAnim* nodeAnim)
	{
		// Check if there are more than 1 key of rotation, otherwise return the rotation at keyframe 0
		if (nodeAnim->mNumRotationKeys == 1)
		{
			rotation = nodeAnim->mRotationKeys[0].mValue;
			return;
		}

		// Get the rotation at the current keyframe and normalize it
		aiQuaternion& rot = nodeAnim->mRotationKeys[keyframe].mValue;
		rot.Normalize();
		rotation = rot;
	}

	void SkinnedMultiMatMesh::GetScaleAtKeyframe(aiVector3D& scale, unsigned int keyframe, const aiNodeAnim* nodeAnim)
	{
		// Check if there are more than 1 key of scaling, otherwise return the scaling at keyframe 0
		if (nodeAnim->mNumScalingKeys == 1)
		{
			scale = nodeAnim->mScalingKeys[0].mValue;
			return;
		}

		// Get the scaling at the current keyframe
		aiVector3D& scaling = nodeAnim->mScalingKeys[keyframe].mValue;
		scale = scaling;
	}
}
