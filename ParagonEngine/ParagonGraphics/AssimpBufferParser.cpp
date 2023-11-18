#include "AssimpBufferParser.h"
#include "Asset3DModelData.h"
#include "LowDX11Storage.h"
#include "LayoutDefine.h"
#include "MaterialCluster.h"
#include "RenderTexture2D.h"
#include "AssetModelDataDefine.h"
#include "AssetAnimationDataDefine.h"
#include "GraphicsResourceManager.h"
#include "GraphicsResourceHelper.h"

#include "DX11Headers.h"
#include "MathHelper.h"
#include "../ParagonUtil/ResourceHelper.h"
#include "../ParagonData/AssetDefines.h"

//Assimp
#include <assimp/Importer.hpp>     
#include <assimp/scene.h>          
#include <assimp/postprocess.h> 

//Windows
#include <wincodec.h>

//DXTK
#include <dxtk/ScreenGrab.h>
#include <dxtk/DDSTextureLoader.h>
#include <dxtk/WICTextureLoader.h>

#include <cassert>
#include <vector>
#include <filesystem>

//NULL이 아닐 때만 값을 시행하는 Macro 함수.
#define IF_NOT_NULL(_ptr, _action) \
    do { \
        if (_ptr != nullptr) { \
            _action; \
        } \
    } while (false)

//value가 0이 아닐 때만 값을 시행하는 Macro 함수.
#define IF_NOT_ZERO(_val, _action) \
    do { \
        if (_val != 0) { \
            _action; \
        } \
    } while (false)

namespace Pg::Graphics::Helper
{
	using Pg::Graphics::Helper::MathHelper;
	using Pg::Util::Helper::ResourceHelper;
	using Pg::Data::Enums::eAssetDefine;
	using Pg::Graphics::Manager::GraphicsResourceManager;

	AssimpBufferParser::AssimpBufferParser()
	{
		//
	}

	AssimpBufferParser::~AssimpBufferParser()
	{
		//
	}

	void AssimpBufferParser::AssimpToDXBuffer(bool isSkinned, const aiScene* assimp, ID3D11Buffer*& outVB, ID3D11Buffer*& outIB)
	{
		//일단은 독립적으로 Data 유지하기 위해, Total Vertex/Index Count를 Assimp에서 받는다.
		unsigned int tTotalVertexCount = 0;
		unsigned int tTotalIndexCount = 0;

		for (int i = 0; i < assimp->mNumMeshes; i++)
		{
			tTotalVertexCount += assimp->mMeshes[i]->mNumVertices;
			tTotalIndexCount += (assimp->mMeshes[i]->mNumFaces * 3);
		}

		if (isSkinned)
		{
			ParseAssimpSkinned(assimp, outVB, outIB, tTotalVertexCount, tTotalIndexCount);
		}
		else
		{
			ParseAssimpStatic(assimp, outVB, outIB, tTotalVertexCount, tTotalIndexCount);
		}
	}

	void AssimpBufferParser::ParseAssimpStatic(const aiScene* assimp, ID3D11Buffer*& outVB, ID3D11Buffer*& outIB, unsigned int vertexCnt, unsigned int indexCnt)
	{
		std::vector<LayoutDefine::Vin1stStatic> tVBVec;
		tVBVec.reserve(vertexCnt);

		//Vertex Buffer
		//InputLayout에 따라서 3DModel의 정보 로드. (Vin1stStatic)
		for (size_t i = 0; i < assimp->mNumMeshes; i++)
		{
			for (size_t j = 0; j < assimp->mMeshes[i]->mNumVertices; j++)
			{
				LayoutDefine::Vin1stStatic tMeshVert;
				tMeshVert.posL = MathHelper::AI2SM_VECTOR3(assimp->mMeshes[i]->mVertices[j]);
				tMeshVert.normalL = MathHelper::AI2SM_VECTOR3(assimp->mMeshes[i]->mNormals[j]);
				tMeshVert.tangentL = MathHelper::AI2SM_VECTOR3(assimp->mMeshes[i]->mTangents[j]);

				//없으면 {0.f,0.f,0.f,0.f}가 들어가 있음.
				//IF_NOT_NULL(assimp->mMeshes[i]->mColors[j],
				//	tMeshVert.color = MathHelper::AI2SM_COLOR_VECTOR4(assimp->mMeshes[i]->mColors[0][j]););
				//일단은 Color 지원을 파싱에서 받지 않는다!
				tMeshVert.color = { 0.f, 0.f, 0.f, 0.f };

				tMeshVert.tex = MathHelper::AI2SM_VECTOR3(assimp->mMeshes[i]->mTextureCoords[0][j]);
				tMeshVert.matID = assimp->mMeshes[i]->mMaterialIndex;

				tVBVec.push_back(tMeshVert);
			}
		}

		//Index Buffer
		std::vector<unsigned int> tIBVec;
		tIBVec.reserve(indexCnt);

		for (int i = 0; i < assimp->mNumMeshes; i++)
		{
			for (int j = 0; j < assimp->mMeshes[i]->mNumFaces; j++)
			{
				for (int k = 0; k < 3; k++)
				{
					tIBVec.push_back(assimp->mMeshes[i]->mFaces[j].mIndices[k]);
				}
			}
		}

		D3D11_BUFFER_DESC tVBD;
		tVBD.Usage = D3D11_USAGE_IMMUTABLE;
		tVBD.ByteWidth = static_cast<UINT>(sizeof(LayoutDefine::Vin1stStatic) * vertexCnt);
		tVBD.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		tVBD.CPUAccessFlags = 0;
		tVBD.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA vinitData;
		vinitData.pSysMem = &(tVBVec[0]);

		HR(LowDX11Storage::GetInstance()->_device->CreateBuffer(&tVBD, &vinitData, &outVB));

		D3D11_BUFFER_DESC tIBD;
		tIBD.Usage = D3D11_USAGE_IMMUTABLE;
		tIBD.ByteWidth = static_cast<UINT>(sizeof(UINT) * indexCnt);
		tIBD.BindFlags = D3D11_BIND_INDEX_BUFFER;
		tIBD.CPUAccessFlags = 0;
		tIBD.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA iinitData;
		iinitData.pSysMem = &(tIBVec[0]);

		HR(LowDX11Storage::GetInstance()->_device->CreateBuffer(&tIBD, &iinitData, &outIB));
	}

	void AssimpBufferParser::ParseAssimpSkinned(const aiScene* assimp, ID3D11Buffer*& outVB, ID3D11Buffer*& outIB, unsigned int vertexCnt, unsigned int indexCnt)
	{
		//아직까지 옮기지 않았음.
		//
		assert(false);
	}

	void AssimpBufferParser::AssimpToMaterialClusterList(const aiScene* assimp, std::vector<MaterialCluster*>& outMatClusterList, const std::string& directory)
	{
		//미리 GraphicsResourceManager 받아오기.
		GraphicsResourceManager* tGraphicsResourceManager = GraphicsResourceManager::Instance();

		unsigned int tNumMaterials = assimp->mNumMaterials;
		outMatClusterList.reserve(assimp->mNumMaterials);

		//일단 만들어진 MaterialCluster는 리소스 매니저에서 관리되지는 않는다.
		//다만, 내부의 RenderTexture2D는 관리될 것이다.
		for (int i = 0; i < assimp->mNumMaterials; i++) // i = Material Index.
		{
			MaterialCluster* tMatCluster = new MaterialCluster();

			for (size_t j = 0; j <= Pg::Defines::ASSET_MAXIMUM_TEXTURE_PROP_CNT; j++)
			{
				eAssetTextureType tTexType = static_cast<eAssetTextureType>(j); //Texture Type(j)
				int tTexTypeTexCnt = assimp->mMaterials[i]->GetTextureCount((aiTextureType)tTexType);
				if (tTexTypeTexCnt > 0)
				{
					aiString tAssimpTexturePath;
					assimp->mMaterials[i]->GetTexture((aiTextureType)tTexType, i, &tAssimpTexturePath);
					std::string tTexturePath = tAssimpTexturePath.C_Str();
					std::string tCompletePath = directory + '/' + tTexturePath;
					tCompletePath = ResourceHelper::ForcePathUniform(tCompletePath);

					//이미 해당 이름으로 된 리소스가 없다면
					if (!tGraphicsResourceManager->IsExistResource(tCompletePath))
					{
						HRESULT hr = S_OK;

						const aiTexture* tEmbeddedTexture = assimp->GetEmbeddedTexture(tAssimpTexturePath.C_Str());

						//만약 임베딩 형태로 받아온 Texture가 존재한다면, GraphicsResourceManager에 추가함과 동시에 설정해야 한다.
						if (tEmbeddedTexture != nullptr)
						{
							//우선적으로 FilePath대로 파일을 물리적으로 존재하게 한 후, 시행.
							//이미 있으면, 아무것도 하지 않는다. 다음에 로드해서 갖고 오면 되는 문제.
							SaveEmbeddedTextureToFile(tCompletePath, tEmbeddedTexture);
						}
						else
						{
							//경로 형식으로 저장되어 있다는 뜻
							//Windows 파일로 계산된 경로에 실제로 해당 리소스가 저장되어 있는지 확인한다. 
							std::filesystem::path tCompletePathFS = tCompletePath;
							assert(std::filesystem::exists(tCompletePathFS) && "임베딩 X라서 텍스쳐 경로가 있어야 하는데, 존재하지 않는다.");
						}

						//일단은 해당 리소스대로 일단 GraphicsResourceManager에 추가.
						tGraphicsResourceManager->LoadResource(tCompletePath, eAssetDefine::_2DTEXTURE);
						//AssetManager와 연동 위해.
						tGraphicsResourceManager->AddSecondaryResource(tCompletePath, eAssetDefine::_2DTEXTURE);
					}

					//이미 동일한 파일 이름으로 로드된 RenderTexture2D가 있다.
					auto tTexture2dData = tGraphicsResourceManager->GetResource(tCompletePath, Pg::Data::Enums::eAssetDefine::_2DTEXTURE);
					tMatCluster->_atsList[j] = static_cast<RenderTexture2D*>(tTexture2dData.get());
				}
			}
			outMatClusterList.push_back(tMatCluster);
		}
	}


	void AssimpBufferParser::AssimpToSceneAssetData(const aiScene* assimp, const std::string& path, Scene_AssetData* outSceneAssetData)
	{
		outSceneAssetData->_directory = path;

		outSceneAssetData->_rootNode = std::make_unique<Node_AssetData>(nullptr);
		StoreAssimpNode(assimp->mRootNode, outSceneAssetData->_rootNode.get());

		outSceneAssetData->_totalMeshCount = assimp->mNumMeshes;
		outSceneAssetData->_meshList.resize(outSceneAssetData->_totalMeshCount);

		//Vertex, Index Count 받기 위해.
		unsigned int tTotalVertexCnt = 0;
		unsigned int tTotalIndexCnt = 0;

		for (int i = 0; i < outSceneAssetData->_totalMeshCount; i++)
		{
			StoreAssimpMesh(assimp->mMeshes[i], &(outSceneAssetData->_meshList[i]), tTotalVertexCnt, tTotalIndexCnt);
			tTotalVertexCnt += outSceneAssetData->_meshList[i]._numVertices;
			tTotalIndexCnt += outSceneAssetData->_meshList[i]._numIndices;
		}

		outSceneAssetData->_totalVertexCount = tTotalVertexCnt;
		outSceneAssetData->_totalIndexCount = tTotalIndexCnt;

		outSceneAssetData->_totalMaterialCount = assimp->mNumMaterials;
	}

	//Parent를 빼고 나머지 정보를 저장한다.
	void AssimpBufferParser::StoreAssimpNode(const aiNode* assimp, Node_AssetData* pgNode)
	{
		pgNode->_nodeName = assimp->mName.C_Str();
		pgNode->_relTransform = MathHelper::AI2SM_MATRIX(assimp->mTransformation);
		//Transpose해서 보관 (Column Major <-> Row Major)
		pgNode->_relTransform = pgNode->_relTransform.Transpose();

		pgNode->_numMeshes = assimp->mNumMeshes;
		for (int i = 0; i < pgNode->_numMeshes; i++)
		{
			pgNode->_meshIndexList.push_back(assimp->mMeshes[i]);
		}

		pgNode->_numChildren = assimp->mNumChildren;
		pgNode->_childrenList.reserve(pgNode->_numChildren);
		for (int i = 0; i < pgNode->_numChildren; i++)
		{
			pgNode->_childrenList.push_back(std::make_unique<Node_AssetData>(pgNode));
			StoreAssimpNode(assimp->mChildren[i], pgNode->_childrenList[i].get());
		}
	}

	void AssimpBufferParser::StoreAssimpMesh(const aiMesh* assimp, Mesh_AssetData* pgMesh, unsigned int vOffset, unsigned int iOffset)
	{
		pgMesh->_vertexOffset = vOffset;
		pgMesh->_indexOffset = iOffset;

		pgMesh->_numVertices = assimp->mNumVertices;
		pgMesh->_numIndices = assimp->mNumFaces * 3; //aiProcess_Triangulate를 썼기 때문에 항상 각 Face당 Index 3개.
		pgMesh->_materialID = assimp->mMaterialIndex;

		StoreAssimpAABB(&(assimp->mAABB), &(pgMesh->_AABB));
	}

	void AssimpBufferParser::StoreAssimpAABB(const aiAABB* assimp, AABB_AssetData* pgAABB)
	{
		pgAABB->_minVec = MathHelper::AI2SM_VECTOR3(assimp->mMin);
		pgAABB->_maxVec = MathHelper::AI2SM_VECTOR3(assimp->mMax);
	}

	//For Animation
	void AssimpBufferParser::StoreAssimpVectorKey(const aiVectorKey* assimp, VectorKey_AssetData* pgVectorKey)
	{
		pgVectorKey->_time = assimp->mTime;
		pgVectorKey->_value = MathHelper::AI2SM_VECTOR3(assimp->mValue);
	}

	void AssimpBufferParser::StoreAssimpQuatKey(const aiQuatKey* assimp, QuatKey_AssetData* pgQuatKey)
	{
		pgQuatKey->_time = assimp->mTime;
		pgQuatKey->_value = MathHelper::AI2SM_QUATERNION(assimp->mValue);
	}

	void AssimpBufferParser::StoreAssimpNodeAnim(const aiNodeAnim* assimp, NodeAnim_AssetData* pgNodeAnim)
	{
		pgNodeAnim->_nodeName = assimp->mNodeName.C_Str();
		pgNodeAnim->_numPositionKeys = assimp->mNumPositionKeys;

		pgNodeAnim->_positionKeyList.resize(pgNodeAnim->_numPositionKeys);
		for (int i = 0; i < pgNodeAnim->_numPositionKeys; i++)
		{
			StoreAssimpVectorKey(&(assimp->mPositionKeys[i]), &(pgNodeAnim->_positionKeyList[i]));
		}

		pgNodeAnim->_numRotationKeys = assimp->mNumRotationKeys;

		pgNodeAnim->_rotationKeyList.resize(pgNodeAnim->_numRotationKeys);
		for (int i = 0; i < pgNodeAnim->_numRotationKeys; i++)
		{
			StoreAssimpQuatKey(&(assimp->mRotationKeys[i]), &(pgNodeAnim->_rotationKeyList[i]));
		}
	}

	void AssimpBufferParser::StoreAssimpAnimation(const aiAnimation* assimp, Animation_AssetData* pgAnim)
	{
		pgAnim->_name = assimp->mName.C_Str();
		pgAnim->_duration = assimp->mDuration;
		pgAnim->_ticksPerSecond = assimp->mTicksPerSecond;
		pgAnim->_numChannels = assimp->mNumChannels;

		pgAnim->_channelList.reserve(pgAnim->_numChannels);
		for (int i = 0; i < pgAnim->_numChannels; i++)
		{
			pgAnim->_channelList.push_back(std::make_unique<NodeAnim_AssetData>());
			StoreAssimpNodeAnim(assimp->mChannels[i], pgAnim->_channelList[i].get());
		}
	}

	//void BufferParser::StoreAssimpBone(const aiBone* assimp, Bone_AssetData* pgAABB)
	//{
	//
	//}

	void AssimpBufferParser::SaveEmbeddedTextureToFile(const std::string& filePath, const aiTexture* assimp)
	{
		//이 Dir는 상대 경로로 들어와야 한다.
		//목표는 여기서 만드는게 아니라, 이 함수 안에서 LoadResource 호출 -> 후에 GetResource로 가져올 수 있게 하기.

		HRESULT hr;
		ID3D11Resource* tUseResource = nullptr;
		ID3D11Texture2D* tUseTexture2D = nullptr;
		//이 둘이 같은 대상을 가리키게 해야 한다.

		if (assimp->mHeight != 0)
		{
			// Load an uncompressed ARGB8888 embedded texture
			D3D11_TEXTURE2D_DESC desc;
			desc.Width = assimp->mWidth;
			desc.Height = assimp->mHeight;
			desc.MipLevels = 1;
			desc.ArraySize = 1;
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
			desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			desc.CPUAccessFlags = 0;
			desc.MiscFlags = 0;

			D3D11_SUBRESOURCE_DATA subresourceData;
			subresourceData.pSysMem = assimp->pcData;
			subresourceData.SysMemPitch = assimp->mWidth * 4;
			subresourceData.SysMemSlicePitch = assimp->mWidth * assimp->mHeight * 4;

			hr = LowDX11Storage::GetInstance()->_device->CreateTexture2D(&desc, &subresourceData, &tUseTexture2D);
			if (FAILED(hr))
			{
				MessageBox(LowDX11Storage::GetInstance()->_hWnd, L"임베디드 텍스쳐 로드 안에서, CreateTexture2D 실패!", L"오류", MB_ICONERROR | MB_OK);
			}
			//tUseResource, tUseTexture2D는 같은 대상을 가리킨다.
			HR(tUseTexture2D->QueryInterface(IID_ID3D11Resource, (void**)&tUseResource));
		}
		else
		{
			//Texture만 유지시키고, SRV는 RenderTexture때 다시 만들어질 것이다.
			ID3D11ShaderResourceView* tDumpSRV = nullptr;

			// mHeight is 0, so try to load a compressed texture of mWidth bytes
			const size_t tSize = assimp->mWidth;
			std::string tExt = assimp->achFormatHint;

			if (tExt != "dds" && tExt != "DDS")
			{
				DirectX::CreateWICTextureFromMemory(LowDX11Storage::GetInstance()->_device, LowDX11Storage::GetInstance()->_deviceContext,
					reinterpret_cast<const unsigned char*>(assimp->pcData), tSize, &tUseResource, &tDumpSRV);
			}
			else
			{
				HR(DirectX::CreateDDSTextureFromMemory(LowDX11Storage::GetInstance()->_device, LowDX11Storage::GetInstance()->_deviceContext,
					reinterpret_cast<const unsigned char*>(assimp->pcData), tSize, &tUseResource, &tDumpSRV));
			}

			HR(tUseResource->QueryInterface(IID_ID3D11Texture2D, (void**)&tUseTexture2D));
		}

		//SRV를 만드는 대신, ScreenGrab의 파일로 쓰기를 활용할 것!
		///여기하고 있었음!
		//Directory는 NNN.fbm//XXX.png 뭐 이런 형태로 들어온다.
		//기록할 FileDirectory + PathName 적기.

		std::wstring tFilenameWS = std::wstring(filePath.begin(), filePath.end());

		//DDS/WIC 따라 달라진다.
		std::string tExt = assimp->achFormatHint;
		if (tExt != "dds" || tExt != "DDS")
		{
			HR(DirectX::SaveDDSTextureToFile(LowDX11Storage::GetInstance()->_deviceContext, tUseTexture2D, tFilenameWS.c_str()));
		}
		else
		{
			//WIC는 DDS보다 더 다양한 설정이 있었다. 일단 이를 기반해서 보자.
			if (tExt == "jpeg" || tExt == "JPEG" || tExt == "jpg" || tExt == "JPG")
			{
				HR(DirectX::SaveWICTextureToFile(LowDX11Storage::GetInstance()->_deviceContext, tUseTexture2D, GUID_ContainerFormatJpeg, tFilenameWS.c_str()));
			}
			else if (tExt == "png" || tExt == "PNG")
			{
				HR(DirectX::SaveWICTextureToFile(LowDX11Storage::GetInstance()->_deviceContext, tUseTexture2D, GUID_ContainerFormatPng, tFilenameWS.c_str()));
			}
			else
			{
				assert(false && "모르는 임베딩된 이미지 형식.");
			}
		}

		//이제 함수 밖에서 실제로 값을 추가해야 한다. 
		//여기서 실패한다면, 임베딩된 리소스의 경로가 잘못되어 만들어졌다는 것.
		return;
	}

	


}
