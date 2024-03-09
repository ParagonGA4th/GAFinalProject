#include "AssimpBufferParser.h"
#include "Asset3DModelData.h"
#include "LowDX11Storage.h"
#include "LayoutDefine.h"
#include "MaterialCluster.h"
#include "RenderTexture2D.h"
#include "RenderTexture2DArray.h"
#include "AssetModelDataDefine.h"
#include "AssetAnimationDataDefine.h"
#include "GraphicsResourceManager.h"
#include "GraphicsResourceHelper.h"

#include "DX11Headers.h"
#include "MathHelper.h"
#include "../ParagonHelper/ResourceHelper.h"
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

	void AssimpBufferParser::AssimpToStaticDataDXBuffer(const aiScene* assimp, Scene_AssetData* sceneData, ID3D11Buffer*& outVB, ID3D11Buffer*& outIB)
	{
		//Total Vertex/Index Count 활용.
		unsigned int tTotalVertexCount = sceneData->_totalVertexCount;
		unsigned int tTotalIndexCount = sceneData->_totalIndexCount;

		ParseAssimpStatic(assimp, sceneData, outVB, outIB, tTotalVertexCount, tTotalIndexCount);
	}

	void AssimpBufferParser::ParseAssimpStatic(const aiScene* assimp, Scene_AssetData* sceneData, ID3D11Buffer*& outVB, ID3D11Buffer*& outIB, unsigned int vertexCnt, unsigned int indexCnt)
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

				tMeshVert._posL = MathHelper::AI2SM_VECTOR3(assimp->mMeshes[i]->mVertices[j]);
				tMeshVert._alpha = 1.f; //하드코딩.
				tMeshVert._normalL = MathHelper::AI2SM_VECTOR3(assimp->mMeshes[i]->mNormals[j]);
				tMeshVert._tangentL = MathHelper::AI2SM_VECTOR3(assimp->mMeshes[i]->mTangents[j]);
				//없으면 {0.f,0.f,0.f,0.f}가 들어가 있음.
				//IF_NOT_NULL(assimp->mMeshes[i]->mColors[j],
				//	tMeshVert.color = MathHelper::AI2SM_COLOR_VECTOR4(assimp->mMeshes[i]->mColors[0][j]););
				//일단은 Color 지원을 파싱에서 받지 않는다!
				tMeshVert._color = { 1.0f,1.0f, 1.0f, }; //하드코딩.
				
				//Float3인데, Blender에서는 Float2까지만 지원되어 들어올 것이다. 
				// 마지막 Z공간은 Assimp 자체 Material ID를 위해 활용될 것. 
				tMeshVert._meshMatID = assimp->mMeshes[i]->mMaterialIndex;

				auto tTex = MathHelper::AI2SM_VECTOR3(assimp->mMeshes[i]->mTextureCoords[0][j]);
				tMeshVert._tex = { tTex.x, tTex.y };
				tMeshVert._uvSet2 = { 0.f, 0.f }; //하드코딩.
				//일단 LightMapUV도 FBX딴에서 들어오는 것은 확인했지만, 일단은 파싱에서 받지 않는다.
				tMeshVert._lightmapUV = { 0.f, 0.f }; //하드코딩.

				tVBVec.push_back(tMeshVert);
			}
		}

		//Object / Material ID 보관을 위해 + 디폴트 처리를 위해.
		sceneData->_posRecordVector.resize(vertexCnt);
		sceneData->_texRecordVector.resize(vertexCnt);
		sceneData->_meshMatIDRecordVector.resize(vertexCnt);

		//RecordVector들 옮기기.
		for (size_t i = 0; i < vertexCnt; i++)
		{
			sceneData->_posRecordVector[i] = tVBVec[i]._posL;
			sceneData->_texRecordVector[i] = tVBVec[i]._tex;
			sceneData->_meshMatIDRecordVector[i] = tVBVec[i]._meshMatID;
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

	void AssimpBufferParser::AssimpToSkinnedDataDXBuffer(const aiScene* assimp, Scene_AssetData* sceneData, Skinned_AssetData* skinnedData, ID3D11Buffer*& outVB, ID3D11Buffer*& outIB)
	{
		//우선적으로, Skinned Data부터 보관.
		StoreIndependentSkinnedData(assimp, skinnedData);

		std::vector<RenderPrepVertexBone> tVertexBoneVector;
		tVertexBoneVector.resize(sceneData->_totalVertexCount);
		StoreGetDependentSkinnedData(assimp, sceneData, skinnedData, tVertexBoneVector);

		//Total Vertex/Index Count 활용.
		unsigned int tTotalVertexCount = sceneData->_totalVertexCount;
		unsigned int tTotalIndexCount = sceneData->_totalIndexCount;

		ParseAssimpSkinned(assimp, sceneData, skinnedData, tVertexBoneVector, outVB, outIB, tTotalVertexCount, tTotalIndexCount);
	}

	//스키닝 데이터 중, 실시간 데이터와 상관 없는 스키닝 데이터 정보 입력.
	void AssimpBufferParser::StoreIndependentSkinnedData(const aiScene* assimp, Skinned_AssetData* skinnedData)
	{
		//Global Inverse Transform 기록.
		DirectX::SimpleMath::Matrix tGlobalTrans = MathHelper::AI2SM_MATRIX(assimp->mRootNode->mTransformation);
		tGlobalTrans = tGlobalTrans.Transpose();
		DirectX::XMVECTOR tDet = DirectX::XMVectorZero();
		skinnedData->_meshGlobalInverseTransform = DirectX::XMMatrixInverse(&tDet, tGlobalTrans);

		//나머지는 Dependent에서 옮겨질 것. 
	}

	void AssimpBufferParser::StoreGetDependentSkinnedData(const aiScene* assimp, const Scene_AssetData* sceneData, Skinned_AssetData* skinnedData, std::vector<RenderPrepVertexBone>& outVertexBoneVector)
	{
		for (unsigned int i = 0; i < assimp->mNumMeshes; i++)
		{
			aiMesh* aMesh = assimp->mMeshes[i];

			//Vertex 로드는 인덱스가 변하지 않기 때문에, 제대로 적용.
			if (aMesh->HasBones())
			{
				SetupRenderBones(i, aMesh, sceneData, skinnedData, outVertexBoneVector);
			}
		}
	}

	void AssimpBufferParser::SetupRenderBones(unsigned int index, aiMesh* mesh, const Scene_AssetData* sceneData, Skinned_AssetData* skinnedData, std::vector<RenderPrepVertexBone>& vBoneList)
	{
		using DirectX::SimpleMath::Matrix;

		for (unsigned int i = 0; i < mesh->mNumBones; i++) {

			unsigned int BoneIndex = 0;

			// Obtain the bone name.
			std::string BoneName(mesh->mBones[i]->mName.C_Str());

			// If bone isn't already in the map. 
			if (skinnedData->_mappedBones.find(BoneName) == skinnedData->_mappedBones.end())
			{
				// Set the bone ID to be the current total number of bones. 
				BoneIndex = skinnedData->_numFormationBone;

				// Increment total bones. 
				skinnedData->_numFormationBone++;

				// Push new bone info into bones vector. 
				BoneInfo_AssetData tBi;
				skinnedData->_renderBoneInfoVector.push_back(tBi);
			}
			else {
				// Bone ID is already in map. 
				BoneIndex = skinnedData->_mappedBones[BoneName];
			}

			skinnedData->_mappedBones[BoneName] = BoneIndex;

			// Obtains the offset matrix which transforms the bone from mesh space into bone space. 
			Matrix tBoneOffset = MathHelper::AI2SM_MATRIX(mesh->mBones[i]->mOffsetMatrix);
			///기존
			//MathHelper::DecomposeAssembleMatrix(tBoneOffset);
			skinnedData->_renderBoneInfoVector[BoneIndex]._boneOffset = tBoneOffset.Transpose();

			// Iterate over all the affected vertices by this bone i.e weights. 
			for (unsigned int j = 0; j < mesh->mBones[i]->mNumWeights; j++) {

				// Obtain an index to the affected vertex within the array of vertices.
				//unsigned int VertexID = _meshEntriesVector[index]._baseVertex + mesh->mBones[i]->mWeights[j].mVertexId;
				unsigned int VertexID = sceneData->_meshList[index]._vertexOffset + mesh->mBones[i]->mWeights[j].mVertexId;

				// The value of how much this bone influences the vertex. 
				float Weight = mesh->mBones[i]->mWeights[j].mWeight;

				// Insert bone data for particular vertex ID. A maximum of 4 bones can influence the same vertex. 
				vBoneList[VertexID].AddBoneData(BoneIndex, Weight);
			}
		}
		assert(mesh);
	}

	void AssimpBufferParser::ParseAssimpSkinned(const aiScene* assimp, Scene_AssetData* sceneData, Skinned_AssetData* skinnedData, const std::vector<RenderPrepVertexBone>& vertexBoneVector, ID3D11Buffer*& outVB, ID3D11Buffer*& outIB, unsigned int vertexCnt, unsigned int indexCnt)
	{
		//지금까지 Bone Index/Weight Binding을 위해, 인덱스 카운팅 도입.
		UINT tTotalElapsedVertexCount = 0;
		UINT tTotalElapsedIndiceCount = 0;

		LayoutDefine::Vin1stSkinned* vertices = new LayoutDefine::Vin1stSkinned[vertexCnt];
		int32_t* indices = new int32_t[indexCnt];
		uint32_t vid = 0, iid = 0;

		//별개로, 나중에 Material ID, Object ID를 기록해야 하기에 필요한 정보인 Position만 기록. (&& Blend Data Info + 디폴트 데이터)
		sceneData->_posRecordVector.resize(vertexCnt);
		sceneData->_texRecordVector.resize(vertexCnt);
		sceneData->_meshMatIDRecordVector.resize(vertexCnt);
		skinnedData->_blendDataRecordVector.resize(vertexCnt);

		for (uint32_t i = 0; i < assimp->mNumMeshes; i++)
		{
			aiMesh* m = assimp->mMeshes[i];
			tTotalElapsedVertexCount = sceneData->_meshList[i]._vertexOffset;
			tTotalElapsedIndiceCount = sceneData->_meshList[i]._indexOffset;

			for (uint32_t j = 0; j < m->mNumVertices; j++)
			{
				//문제 생기면 MultimaterialMesh (원본 참고)
				auto& pos = m->mVertices[j];
				auto& norm = m->mNormals[j];
				auto& tan = m->mTangents[j];
				auto& texUV = m->mTextureCoords[0][j];
				unsigned int tMeshMatID = m->mMaterialIndex;
				//일단은 하드코딩됨.

				vertices[vid + j]._posL = DirectX::XMFLOAT3{ pos.x, pos.y, pos.z };
				vertices[vid + j]._alpha = 1.f; //하드코딩.
				vertices[vid + j]._normalL = DirectX::XMFLOAT3{ norm.x, norm.y, norm.z };
				vertices[vid + j]._tangentL = DirectX::XMFLOAT3{ tan.x, tan.y, tan.z };
				vertices[vid + j]._color = DirectX::XMFLOAT3{ 1.0f,1.0f, 1.0f }; //하드코딩.
				vertices[vid + j]._meshMatID = tMeshMatID;
				vertices[vid + j]._tex = DirectX::XMFLOAT2{ texUV.x, texUV.y};
				vertices[vid + j]._uvSet2 = { 0.f, 0.f }; //하드코딩.
				//일단 LightMapUV도 FBX딴에서 들어오는 것은 확인했지만, 일단은 파싱에서 받지 않는다.
				vertices[vid + j]._lightmapUV = { 0.f, 0.f }; //하드코딩.

				vertices[vid + j]._blendIndice0 = vertexBoneVector.at(j + tTotalElapsedVertexCount).IDs[0];
				vertices[vid + j]._blendIndice1 = vertexBoneVector.at(j + tTotalElapsedVertexCount).IDs[1];
				vertices[vid + j]._blendIndice2 = vertexBoneVector.at(j + tTotalElapsedVertexCount).IDs[2];
				vertices[vid + j]._blendIndice3 = vertexBoneVector.at(j + tTotalElapsedVertexCount).IDs[3];

				vertices[vid + j]._blendWeight0 = vertexBoneVector.at(j + tTotalElapsedVertexCount).Weights[0];
				vertices[vid + j]._blendWeight1 = vertexBoneVector.at(j + tTotalElapsedVertexCount).Weights[1];
				vertices[vid + j]._blendWeight2 = vertexBoneVector.at(j + tTotalElapsedVertexCount).Weights[2];

				//RecordVector들 기록 (나중에 Object / Material ID 관련 생성 위해) + 디폴트 데이터 관리 위해.
				sceneData->_posRecordVector.at(vid + j) = vertices[vid + j]._posL;
				sceneData->_texRecordVector.at(vid + j) = vertices[vid + j]._tex;
				sceneData->_meshMatIDRecordVector.at(vid + j) = vertices[vid + j]._meshMatID;
				skinnedData->_blendDataRecordVector.at(vid + j)._blendIndice0 = vertices[vid + j]._blendIndice0;
				skinnedData->_blendDataRecordVector.at(vid + j)._blendIndice1 = vertices[vid + j]._blendIndice1;
				skinnedData->_blendDataRecordVector.at(vid + j)._blendIndice2 = vertices[vid + j]._blendIndice2;
				skinnedData->_blendDataRecordVector.at(vid + j)._blendIndice3 = vertices[vid + j]._blendIndice3;

				skinnedData->_blendDataRecordVector.at(vid + j)._blendWeight0 = vertices[vid + j]._blendWeight0;
				skinnedData->_blendDataRecordVector.at(vid + j)._blendWeight1 = vertices[vid + j]._blendWeight1;
				skinnedData->_blendDataRecordVector.at(vid + j)._blendWeight2 = vertices[vid + j]._blendWeight2;
			}

			for (uint32_t j = 0; j < m->mNumFaces; j++)
			{
				indices[iid + j * 3] = m->mFaces[j].mIndices[0];
				indices[iid + j * 3 + 1] = m->mFaces[j].mIndices[1];
				indices[iid + j * 3 + 2] = m->mFaces[j].mIndices[2];
			}

			vid += m->mNumVertices;
			iid += m->mNumFaces * 3;
		}

		do
		{
			CD3D11_BUFFER_DESC vbDesc(
				vertexCnt * sizeof(LayoutDefine::Vin1stSkinned),
				D3D11_BIND_VERTEX_BUFFER);
			D3D11_SUBRESOURCE_DATA vbData = { vertices, 0, 0 };
			HR(LowDX11Storage::GetInstance()->_device->CreateBuffer(&vbDesc, &vbData, &outVB));
			assert(false);

			CD3D11_BUFFER_DESC ibDesc(
				indexCnt * sizeof(uint32_t),
				D3D11_BIND_INDEX_BUFFER);
			D3D11_SUBRESOURCE_DATA ibData = { indices, 0, 0 };
			HR(LowDX11Storage::GetInstance()->_device->CreateBuffer(&ibDesc, &ibData, &outIB));
		} while (0);

		delete[] vertices;
		delete[] indices;

		if (nullptr == outVB || nullptr == outIB)
		{
			if (nullptr != outVB) outVB->Release();
			if (nullptr != outIB) outIB->Release();
		}
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
					assimp->mMaterials[i]->GetTexture((aiTextureType)tTexType, 0, &tAssimpTexturePath); //항상 0번째 Texture만을 가져오게!
					std::string tTexturePath = tAssimpTexturePath.C_Str();

					//이전에, Directory에서 .fbx라는 파일 경로에서 TextureX가 파생되는 것이 아니라,
					//"한단계" 상위 경로로 가서 찾아야 한다.
					std::filesystem::path modelParentPath = directory;
					std::string modelParentStr = modelParentPath.parent_path().string();
					std::string tCompletePath = modelParentStr + '/' + tTexturePath;

					tCompletePath = ResourceHelper::ForcePathUniform(tCompletePath);

					//이미 해당 이름으로 된 리소스가 없다면
					if (!tGraphicsResourceManager->IsExistResource(tCompletePath))
					{
						//여기를 이제 Embedding 없이 내부 저장 경로만을 가지고 가져올 수 있게 손봐야 한다.
						//Embedded Texture가 있는지도 검사할 필요 없이, Path만 가지고 있으면 무조건 .fbm 내부를 찾게 해야 함!
						
						//일단, 동일 이름을 가지고 있는 리소스가 있는지부터 확인.
						std::filesystem::path tPath = tCompletePath;
						std::string tFilename = tPath.filename().string();

						if (tGraphicsResourceManager->IsExistResourceByName(tFilename))
						{
							//동일 파일 이름을 가진 리소스가 있다. 기존의 "Complete" Path를 변경.
							tCompletePath = tGraphicsResourceManager->GetResourcePathByName(tFilename, eAssetDefine::_TEXTURE2D);
						}
						else
						{
							//일단은 해당 리소스대로 일단 GraphicsResourceManager에 추가.
							tGraphicsResourceManager->LoadResource(tCompletePath, eAssetDefine::_TEXTURE2D);
							//AssetManager와 연동 위해.
							tGraphicsResourceManager->AddSecondaryResource(tCompletePath, eAssetDefine::_TEXTURE2D);
						}
					}
;
					//이미 동일한 파일 이름으로 로드된 RenderTexture2D가 있다.
					auto tTexture2dData = tGraphicsResourceManager->GetResource(tCompletePath, Pg::Data::Enums::eAssetDefine::_TEXTURE2D);
					tMatCluster->_atsList.at(j) = static_cast<RenderTexture2D*>(tTexture2dData.get());

					////디버그 리스트에도 기록.
					//tMatCluster->_debugList.at(j).second = true;
				}
			}
			outMatClusterList.push_back(tMatCluster);
		}
		//
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

	void AssimpBufferParser::AssimpToPBRTextureArray(const std::string& modelName, std::vector<MaterialCluster*>& outMatClusterList, RenderTexture2DArray** outArrayData)
	{
		//먼저 MaterialClusterList가 실행되었어야 실행될 수 있는 코드!

		std::vector<std::string> tRenderT2Vec;
		tRenderT2Vec.resize(outMatClusterList.size());

		eAssetTextureType tAllRequiredPBRTypes[4] = {
			eAssetTextureType::PG_TextureType_DIFFUSE, eAssetTextureType::PG_TextureType_NORMALS,
			eAssetTextureType::PG_TextureType_SPECULAR, eAssetTextureType::PG_TextureType_ARM };

		std::string tIdentifierString[4] =
		{
			"DIFFUSE", "NORMALS", "SPECULAR", "ARM"
		};

		//outArrayData의 인덱스와 의미 동일.
		for (int k = 0; k < 4; k++)
		{
			for (short i = 0; i < outMatClusterList.size(); i++)
			{
				MaterialCluster* tMatCluster = outMatClusterList.at(i);
				std::string tPath = "";
				eAssetTextureType type = tAllRequiredPBRTypes[k];
				if (tMatCluster->GetTextureByType(type) != nullptr)
				{
					//실제로 값이 있을 경우, 값을 로딩해서 넣는다.
					tPath = tMatCluster->GetTextureByType(type)->GetFilePath();
					std::filesystem::path tFSP = tPath;
					tRenderT2Vec.at(i) = tFSP.filename().string();
				}
				else
				{
					//없을 경우, 타입에 맞는 기본 리소스를 넣는다. 이 경우, Default Textures가 로드될 것.
					//UINT tWidth = tMatCluster->GetTextureByType(type)->GetFileWidth();
					//UINT tHeight = tMatCluster->GetTextureByType(type)->GetFileHeight();
					//
					//eSizeTexture tSize = GraphicsResourceHelper::GetSizeTextureFromUINT(tWidth, tHeight);
					//tRenderT2Vec.at(i) = GraphicsResourceHelper::GetDefaultTexturePath(type, tSize);
					std::filesystem::path tFSP = GraphicsResourceHelper::GetDefaultTexturePath(type);
					tRenderT2Vec.at(i) = tFSP.filename().string();
				}
			}
			
			//어차피 누락되지만, 디버깅하면서 확인하기 위해서.
			std::string defInstMatName = modelName;
			defInstMatName += "_";
			defInstMatName += tIdentifierString[k];
			std::string varName = "PBRTexArray";
			std::string tTempTex2DArrName = GraphicsResourceHelper::GetGeneratedTex2DArrayNameFromValues(defInstMatName, varName, tRenderT2Vec.data(), tRenderT2Vec.size());
			Pg::Graphics::Manager::GraphicsResourceManager::Instance()->LoadResource(tTempTex2DArrName, Pg::Data::Enums::eAssetDefine::_TEXTURE2DARRAY);
			Pg::Graphics::Manager::GraphicsResourceManager::Instance()->AddSecondaryResource(tTempTex2DArrName, Pg::Data::Enums::eAssetDefine::_TEXTURE2DARRAY);

			//로드했으니 이제 가져올 수 있다.
			auto tTex2DRes = Pg::Graphics::Manager::GraphicsResourceManager::Instance()->GetResource(tTempTex2DArrName, Pg::Data::Enums::eAssetDefine::_TEXTURE2DARRAY);
			
			//값이 해당되는 것처럼 할당된다.
			outArrayData[k] = static_cast<RenderTexture2DArray*>(tTex2DRes.get());
		}
		


	}



	//void BufferParser::StoreAssimpBone(const aiBone* assimp, Bone_AssetData* pgAABB)
	//{
	//
	//}
}
