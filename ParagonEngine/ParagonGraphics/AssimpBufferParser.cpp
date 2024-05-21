#include "AssimpBufferParser.h"
#include "Asset3DModelData.h"
#include "LowDX11Storage.h"
#include "LayoutDefine.h"
#include "MaterialCluster.h"
#include "RenderTexture2D.h"
#include "RenderTexture2DArray.h"
#include "AssetModelDataDefine.h"
#include "GraphicsResourceManager.h"
#include "GraphicsResourceHelper.h"
#include "TextureExtension.h"

#include "DX11Headers.h"
#include "MathHelper.h"
#include "../ParagonHelper/ResourceHelper.h"
#include "../ParagonData/AssetDefines.h"
#include "../ParagonData/Transform.h"

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
#include <set>
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
	std::unordered_map<const aiMesh*, Mesh_AssetData*> AssimpBufferParser::_aiMeshToMeshMap{};

	std::unordered_map<const aiNode*, Node_AssetData*> AssimpBufferParser::_aiNodeToNodeMap{};

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

	void AssimpBufferParser::AssimpToStaticDataDXBuffer(const aiScene* assimp, Scene_AssetData* sceneData, ID3D11Buffer*& outVB1st, ID3D11Buffer*& outVB2nd, ID3D11Buffer*& outIB)
	{
		//Total Vertex/Index Count 활용.
		unsigned int tTotalVertexCount = sceneData->_totalVertexCount;
		unsigned int tTotalIndexCount = sceneData->_totalIndexCount;

		ParseAssimpStatic(assimp, sceneData, outVB1st, outVB2nd, outIB, tTotalVertexCount, tTotalIndexCount);
	}

	void AssimpBufferParser::ParseAssimpStatic(const aiScene* assimp, Scene_AssetData* sceneData, ID3D11Buffer*& outVB1st, ID3D11Buffer*& outVB2nd, ID3D11Buffer*& outIB, unsigned int vertexCnt, unsigned int indexCnt)
	{
		//1st Static.
		std::vector<LayoutDefine::Vin1stStatic_Individual> tVBVec;
		tVBVec.reserve(vertexCnt);

		//2nd Static.
		std::vector<LayoutDefine::Vin2ndAll_Individual> tVB2ndVec;
		tVB2ndVec.reserve(vertexCnt);

		//Vertex Buffer
		//InputLayout에 따라서 3DModel의 정보 로드. (Vin1stStatic)
		for (size_t i = 0; i < assimp->mNumMeshes; i++)
		{
			for (size_t j = 0; j < assimp->mMeshes[i]->mNumVertices; j++)
			{
				LayoutDefine::Vin1stStatic_Individual tMeshVert;
				LayoutDefine::Vin2ndAll_Individual tMeshVert2nd;

				//1st.
				tMeshVert._posL = MathHelper::AI2SM_VECTOR3(assimp->mMeshes[i]->mVertices[j]);
				auto tTex = MathHelper::AI2SM_VECTOR3(assimp->mMeshes[i]->mTextureCoords[0][j]);
				tMeshVert._tex = { tTex.x, tTex.y };
				//Float3인데, Blender에서는 Float2까지만 지원되어 들어올 것이다. 
				// 마지막 Z공간은 Assimp 자체 Material ID를 위해 활용될 것. 
				tMeshVert._meshMatID = assimp->mMeshes[i]->mMaterialIndex;
				
				//1번째 Texture Coord가 있는지 (Lightmap UV 관련) => 베이스로 진행.
				if (assimp->mMeshes[i]->HasTextureCoords(1))
				{
					auto tLightmapTex = MathHelper::AI2SM_VECTOR3(assimp->mMeshes[i]->mTextureCoords[1][j]);
					tMeshVert._lightmapUV = { tLightmapTex.x,tLightmapTex.y };
				}
				else
				{
					tMeshVert._lightmapUV = { 0.f, 0.f }; //하드코딩.
				}


				//2nd
				tMeshVert2nd._normalL = MathHelper::AI2SM_VECTOR3(assimp->mMeshes[i]->mNormals[j]);
				tMeshVert2nd._tangentL = MathHelper::AI2SM_VECTOR3(assimp->mMeshes[i]->mTangents[j]);
				if (assimp->mMeshes[i]->mColors[0] != nullptr)
				{
					//Color가 있다.
					aiVector3D tColorStore = { assimp->mMeshes[i]->mColors[0][j].r, assimp->mMeshes[i]->mColors[0][j].g,assimp->mMeshes[i]->mColors[0][j].b };
					tMeshVert2nd._color = MathHelper::AI2SM_VECTOR3(tColorStore);
				}
				else
				{
					//Color가 없다.
					tMeshVert2nd._color = { 1.0f,1.0f, 1.0f, }; //하드코딩.
				}
				
				
				tVBVec.push_back(tMeshVert);
				tVB2ndVec.push_back(tMeshVert2nd);
			}
		}

		//이제 ObjMat은 인스턴스처럼 다뤄질 것.
		//Object / Material ID 보관을 위해 + 디폴트 처리를 위해.
		//sceneData->_posRecordVector.resize(vertexCnt);
		//sceneData->_texRecordVector.resize(vertexCnt);
		//sceneData->_meshMatIDRecordVector.resize(vertexCnt);
		//
		////RecordVector들 옮기기.
		//for (size_t i = 0; i < vertexCnt; i++)
		//{
		//	sceneData->_posRecordVector[i] = tVBVec[i]._posL;
		//	sceneData->_texRecordVector[i] = tVBVec[i]._tex;
		//	sceneData->_meshMatIDRecordVector[i] = tVBVec[i]._meshMatID;
		//}

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

		{
			D3D11_BUFFER_DESC tVBD;
			tVBD.Usage = D3D11_USAGE_IMMUTABLE;
			tVBD.ByteWidth = static_cast<UINT>(sizeof(LayoutDefine::Vin1stStatic_Individual) * vertexCnt);
			tVBD.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			tVBD.CPUAccessFlags = 0;
			tVBD.MiscFlags = 0;
			D3D11_SUBRESOURCE_DATA vinitData;
			vinitData.pSysMem = &(tVBVec[0]);

			HR(LowDX11Storage::GetInstance()->_device->CreateBuffer(&tVBD, &vinitData, &outVB1st));
		}


		{
			D3D11_BUFFER_DESC tVBD;
			tVBD.Usage = D3D11_USAGE_IMMUTABLE;
			tVBD.ByteWidth = static_cast<UINT>(sizeof(LayoutDefine::Vin2ndAll_Individual) * vertexCnt);
			tVBD.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			tVBD.CPUAccessFlags = 0;
			tVBD.MiscFlags = 0;
			D3D11_SUBRESOURCE_DATA vinitData;
			vinitData.pSysMem = &(tVB2ndVec[0]);

			HR(LowDX11Storage::GetInstance()->_device->CreateBuffer(&tVBD, &vinitData, &outVB2nd));
		}
		

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

	void AssimpBufferParser::AssimpToSkinnedDataDXBuffer(const aiScene* assimp, Scene_AssetData* sceneData, Skinned_AssetData* skinnedData, ID3D11Buffer*& outVB1st, ID3D11Buffer*& outVB2nd, ID3D11Buffer*& outIB)
	{
		//우선적으로, Skinned Data부터 보관.
		StoreIndependentSkinnedData(assimp, skinnedData);

		std::vector<VertexBone_TempAssetData> tVertexBoneVector;
		tVertexBoneVector.resize(sceneData->_totalVertexCount);
		StoreGetDependentSkinnedData(assimp, sceneData, skinnedData, tVertexBoneVector);

		//Total Vertex/Index Count 활용.
		unsigned int tTotalVertexCount = sceneData->_totalVertexCount;
		unsigned int tTotalIndexCount = sceneData->_totalIndexCount;

		ParseAssimpSkinned(assimp, sceneData, skinnedData, tVertexBoneVector, outVB1st, outVB2nd, outIB, tTotalVertexCount, tTotalIndexCount);
	
		//추후 렌더링을 위해, 재귀적인 노드 구조를 선형적으로 편동해 기록한다.
		//RenderAnimation 딴에서 해당 노드의 인덱스에 맞는 값을 넣어놓을 것. (없으면 nullptr)
		//	=> 개별 인스턴스로 이동.
		//RecordNodeToList(sceneData->_rootNode.get(), skinnedData);
	}

	//스키닝 데이터 중, 실시간 데이터와 상관 없는 스키닝 데이터 정보 입력.
	void AssimpBufferParser::StoreIndependentSkinnedData(const aiScene* assimp, Skinned_AssetData* skinnedData)
	{
		//Global Inverse Transform 기록.
		aiMatrix4x4 tStoreMat = assimp->mRootNode->mTransformation;
		DirectX::XMMATRIX tGlobalTrans = MathHelper::AI2XM_MATRIX(tStoreMat.Transpose());

		DirectX::XMVECTOR tDet = DirectX::XMVectorZero();
		skinnedData->_meshGlobalInverseTransform = DirectX::XMMatrixInverse(&tDet, tGlobalTrans);

		//나머지는 Dependent에서 옮겨질 것. 
	}

	void AssimpBufferParser::StoreGetDependentSkinnedData(const aiScene* assimp, const Scene_AssetData* sceneData, Skinned_AssetData* skinnedData, std::vector<VertexBone_TempAssetData>& outVertexBoneVector)
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

	void AssimpBufferParser::SetupRenderBones(unsigned int index, aiMesh* mesh, const Scene_AssetData* sceneData, Skinned_AssetData* skinnedData, std::vector<VertexBone_TempAssetData>& vBoneList)
	{
		//기존 구현체 교체.
		//다르게 동일한 버퍼 VB/IB를 기반으로 인덱스를 다르게 읽어서 렌더를 하는 구조.
		//이식을 다르게 해야 한다.
		//VertexBoneVector는 이미 Model의 전체 Vertex 개수로 리사이즈가 되었다.
		
		//Bone Data를 실제로 추가해야 한다.
		//우리는 VB/IB가 하나. 그러니, 얼마나 추가해서 계산해야 하는지를 파악해야 한다! (Base Vertex)
		Mesh_AssetData* tMeshData = _aiMeshToMeshMap.at(mesh);
		
		for (unsigned int i = 0; i < mesh->mNumBones; i++)
		{
			aiBone* aibone = mesh->mBones[i];

			//상호 저장.
			BoneInfo_AssetData* tPgBone = new BoneInfo_AssetData();
			tPgBone->_name = aibone->mName.C_Str();
			tPgBone->_index = _aiNodeToNodeMap.at(aibone->mNode)->_index;
			tPgBone->_bindedNode = _aiNodeToNodeMap.at(aibone->mNode);
			tPgBone->_offsetMatrix = MathHelper::AI2XM_MATRIX(aibone->mOffsetMatrix.Transpose());
			_aiNodeToNodeMap.at(aibone->mNode)->_bindedBone = tPgBone;

			//여러 가지 Mesh가 존재한다는 점 반영해 VertexOffset 기반으로 설정.
			for (unsigned int j = 0; j < aibone->mNumWeights; j++)
			{
				UINT tVertexIndex = aibone->mWeights[j].mVertexId;
				float tWeight = aibone->mWeights[j].mWeight;
				UINT tCombinedVertexIndex = tVertexIndex + tMeshData->_vertexOffset;

				assert((tCombinedVertexIndex < vBoneList.size()) && "유효한 인덱스가 나와야 한다. 합쳐진 VB/IB 이기 때문에.");
				vBoneList.at(tCombinedVertexIndex).AddBoneData(tPgBone->_index, tWeight);
			}

		}
		assert("값 조정 중.");
	}

	void AssimpBufferParser::ParseAssimpSkinned(const aiScene* assimp, Scene_AssetData* sceneData, Skinned_AssetData* skinnedData, const std::vector<VertexBone_TempAssetData>& vertexBoneVector, ID3D11Buffer*& outVB1st, ID3D11Buffer*& outVB2nd, ID3D11Buffer*& outIB, unsigned int vertexCnt, unsigned int indexCnt)
	{
		//지금까지 Bone Index/Weight Binding을 위해, 인덱스 카운팅 도입.
		UINT tTotalElapsedVertexCount = 0;
		UINT tTotalElapsedIndiceCount = 0;

		std::vector<LayoutDefine::Vin1stSkinned_Individual> vertices;
		vertices.resize(vertexCnt);

		std::vector<LayoutDefine::Vin2ndAll_Individual> vertices2nd;
		vertices2nd.resize(vertexCnt);

		std::vector<int32_t> indices;
		indices.resize(indexCnt);
		uint32_t vid = 0, iid = 0;

		//별개로, 나중에 Material ID, Object ID를 기록해야 하기에 필요한 정보인 Position만 기록. (&& Blend Data Info + 디폴트 데이터)
		//sceneData->_posRecordVector.resize(vertexCnt);
		//sceneData->_texRecordVector.resize(vertexCnt);
		//sceneData->_meshMatIDRecordVector.resize(vertexCnt);
		//skinnedData->_blendDataRecordVector.resize(vertexCnt);

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

				//1st
				vertices[vid + j]._posL = DirectX::XMFLOAT3{ pos.x, pos.y, pos.z };
				vertices[vid + j]._tex = DirectX::XMFLOAT2{ texUV.x, texUV.y};
				vertices[vid + j]._meshMatID = tMeshMatID;
				
				//일단은 로직 상 맞는 것 같다.
				vertices[vid + j]._blendIndice0 = vertexBoneVector.at(j + tTotalElapsedVertexCount).IDs[0];
				vertices[vid + j]._blendIndice1 = vertexBoneVector.at(j + tTotalElapsedVertexCount).IDs[1];
				vertices[vid + j]._blendIndice2 = vertexBoneVector.at(j + tTotalElapsedVertexCount).IDs[2];
				vertices[vid + j]._blendIndice3 = vertexBoneVector.at(j + tTotalElapsedVertexCount).IDs[3];

				vertices[vid + j]._blendWeight0 = vertexBoneVector.at(j + tTotalElapsedVertexCount).Weights[0];
				vertices[vid + j]._blendWeight1 = vertexBoneVector.at(j + tTotalElapsedVertexCount).Weights[1];
				vertices[vid + j]._blendWeight2 = vertexBoneVector.at(j + tTotalElapsedVertexCount).Weights[2];

				//SKINNED는 Lightmap UV가 불가능하다! Node Index로 값 바꿀 것. 
				//Node Index 값 투입.
				vertices[vid + j]._nodeIndex = sceneData->_meshList[i]._belongNodeIndex;

				//2nd
				vertices2nd[vid + j]._normalL = DirectX::XMFLOAT3{ norm.x, norm.y, norm.z };
				vertices2nd[vid + j]._tangentL = DirectX::XMFLOAT3{ tan.x, tan.y, tan.z };
				vertices2nd[vid + j]._color = DirectX::XMFLOAT3{ 1.0f,1.0f, 1.0f }; //하드코딩.

				//RecordVector들 기록 (나중에 Object / Material ID 관련 생성 위해) + 디폴트 데이터 관리 위해.
				//sceneData->_posRecordVector.at(vid + j) = vertices[vid + j]._posL;
				//sceneData->_texRecordVector.at(vid + j) = vertices[vid + j]._tex;
				//sceneData->_meshMatIDRecordVector.at(vid + j) = vertices[vid + j]._meshMatID;
				//
				////일단은 로직 상 맞는 것 같다.
				//skinnedData->_blendDataRecordVector.at(vid + j)._blendIndice0 = vertices[vid + j]._blendIndice0;
				//skinnedData->_blendDataRecordVector.at(vid + j)._blendIndice1 = vertices[vid + j]._blendIndice1;
				//skinnedData->_blendDataRecordVector.at(vid + j)._blendIndice2 = vertices[vid + j]._blendIndice2;
				//skinnedData->_blendDataRecordVector.at(vid + j)._blendIndice3 = vertices[vid + j]._blendIndice3;
				//
				//skinnedData->_blendDataRecordVector.at(vid + j)._blendWeight0 = vertices[vid + j]._blendWeight0;
				//skinnedData->_blendDataRecordVector.at(vid + j)._blendWeight1 = vertices[vid + j]._blendWeight1;
				//skinnedData->_blendDataRecordVector.at(vid + j)._blendWeight2 = vertices[vid + j]._blendWeight2;
			}

			for (uint32_t j = 0; j < m->mNumFaces; j++)
			{
				indices[iid + (j * 3)] = m->mFaces[j].mIndices[0];
				indices[iid + (j * 3) + 1] = m->mFaces[j].mIndices[1];
				indices[iid + (j * 3) + 2] = m->mFaces[j].mIndices[2];
			}

			vid += m->mNumVertices;
			iid += m->mNumFaces * 3;
		}

		do
		{
			{
				CD3D11_BUFFER_DESC vbDesc(
					vertexCnt * sizeof(LayoutDefine::Vin1stSkinned_Individual),
					D3D11_BIND_VERTEX_BUFFER);
				D3D11_SUBRESOURCE_DATA vbData = { vertices.data(), 0, 0 };

				HR(LowDX11Storage::GetInstance()->_device->CreateBuffer(&vbDesc, &vbData, &outVB1st));
			}
			
			{
				CD3D11_BUFFER_DESC vbDesc(
					vertexCnt * sizeof(LayoutDefine::Vin2ndAll_Individual),
					D3D11_BIND_VERTEX_BUFFER);
				D3D11_SUBRESOURCE_DATA vbData = { vertices2nd.data(), 0, 0 };

				HR(LowDX11Storage::GetInstance()->_device->CreateBuffer(&vbDesc, &vbData, &outVB2nd));
			}

			//assert(false);

			CD3D11_BUFFER_DESC ibDesc(
				indexCnt * sizeof(uint32_t),
				D3D11_BIND_INDEX_BUFFER);
			D3D11_SUBRESOURCE_DATA ibData = { indices.data(), 0, 0};
			HR(LowDX11Storage::GetInstance()->_device->CreateBuffer(&ibDesc, &ibData, &outIB));
		} while (0);

		//delete[] vertices;
		//delete[] indices;

		if (nullptr == outVB1st || nullptr == outVB2nd || nullptr == outIB)
		{
			if (nullptr != outVB1st) outVB1st->Release();
			if (nullptr != outVB2nd) outVB2nd->Release();
			if (nullptr != outIB) outIB->Release();
		}
	}

	void AssimpBufferParser::AssimpToMaterialClusterList(const aiScene* assimp, bool& bCheckIfUseAlphaBlending, std::vector<MaterialCluster*>& outMatClusterList, const std::string& directory)
	{
		//미리 GraphicsResourceManager 받아오기.
		GraphicsResourceManager* tGraphicsResourceManager = GraphicsResourceManager::Instance();

		unsigned int tNumMaterials = assimp->mNumMaterials;
		outMatClusterList.reserve(assimp->mNumMaterials);

		//따로 실제로 mesh 안에서 AlphaMap을 활용하는지, 바로 박는 것이 아니라, Mesh 내부 Content Checking (MeshMaterial) 할 것.
		std::set<int> useAlphaMappingList;

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
					//다른 모든 로직이 돌아가기 전에, Alpha를 사용하는데 TexCnt가 있으면 해당 AssetModelData에 체크해야 한다.
					if (tTexType == PG_TextureType_OPACITY)
					{
						useAlphaMappingList.insert(i);
					}

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
		
		if (useAlphaMappingList.empty())
		{
			//Early Return. 애초에 OPACITY 발견 안되면 Alpha Blending을 활용할 일이 없으니.
			bCheckIfUseAlphaBlending = false;
			return;
		}


		bool tUseBlend = false;
		//Alpha Blending을 사용하는 MeshMaterial을 실제로 잘 사용하는지 보기.
		for (uint32_t i = 0; i < assimp->mNumMeshes; i++)
		{
			uint32_t tMeshMatID = assimp->mMeshes[i]->mMaterialIndex;
			
			auto it = useAlphaMappingList.find(tMeshMatID);
		
			if (it != useAlphaMappingList.end())
			{
				//알파매핑한다는 얘기다. 찾았기 때문.
				tUseBlend = true;
				break;
			}
		}

		//알파 매핑을 쓰는 material이 있지만, 본 fbx의 mesh 내부 material에는 이를 안 쓴다면? => AlphaBlending==false;
		//아니면 True.
		bCheckIfUseAlphaBlending = tUseBlend;
		return;
	}

	void AssimpBufferParser::AssimpToSceneAssetData(const aiScene* assimp, const std::string& path, Scene_AssetData* outSceneAssetData)
	{
		outSceneAssetData->_directory = path;

		//Mesh에 Node 기록을 위해, Mesh를 우선으로 기록한다.
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

		//이제, 실제로 Node와 Mesh랑 연결하기 위해 Node 기록과 함께 Mesh-Node 연결한다.
		outSceneAssetData->_rootNode = std::make_unique<Node_AssetData>(nullptr);
		//outSceneAssetData->_rootNode->_relTransform = std::make_unique<Pg::Data::Transform>(nullptr); 
		//자식 노드의 Transform 없는 객체를 그대로 전달해준다. (겜옵젝 소속 Transform 아님)

		//Node 이동 발동될 시기 : -> Mesh를 다 받았을 때.
		UINT tIndexForNode = 0;
		StoreAssimpNode(assimp->mRootNode, outSceneAssetData, outSceneAssetData->_rootNode.get(), tIndexForNode);
	}

	//Parent를 빼고 나머지 정보를 저장한다.
	void AssimpBufferParser::StoreAssimpNode(const aiNode* assimp, Scene_AssetData* sceneData, Node_AssetData* pgNode, UINT& index)
	{
		pgNode->_nodeName = assimp->mName.C_Str();

		//Transpose해서 보관 (Column Major <-> Row Major)
		aiMatrix4x4 tStoreTrans = assimp->mTransformation;
		{
			DirectX::SimpleMath::Matrix tRelTrans = MathHelper::AI2XM_MATRIX(tStoreTrans.Transpose());
			pgNode->_offsetMatrix = tRelTrans;
		}
		
		//이제 각각 FBX 내부에서 차지하는 Index 역시 보관. 기록 후 Increment.
		pgNode->_index = index;
		index++;

		//Assimp와 PgNode 연결하는 unordered_map에 저장. (후에 Bone-Binding)
		_aiNodeToNodeMap.insert(std::make_pair(assimp,pgNode));

		pgNode->_numMeshes = assimp->mNumMeshes;
		for (int i = 0; i < pgNode->_numMeshes; i++)
		{
			//이미 저장된 정보를 기준으로 옮긴다. (Mesh_AssetData)
			pgNode->_meshList.push_back(&(sceneData->_meshList.at(assimp->mMeshes[i])));

			//Node 딴에서 MeshList 역시 보관하지만, 직접적으로 Node의 인덱스 역시 Mesh가 보관하게 한다.
			sceneData->_meshList.at(assimp->mMeshes[i])._belongNodeIndex = pgNode->_index;
		}

		pgNode->_numChildren = assimp->mNumChildren;
		pgNode->_childrenList.reserve(pgNode->_numChildren);

		for (int i = 0; i < pgNode->_numChildren; i++)
		{
			//이때 Parent 전달했던 것이다.
			pgNode->_childrenList.push_back(std::make_unique<Node_AssetData>(pgNode));
		
			StoreAssimpNode(assimp->mChildren[i], sceneData, pgNode->_childrenList[i].get(), index);
		}
	}

	void AssimpBufferParser::StoreAssimpMesh(const aiMesh* assimp, Mesh_AssetData* pgMesh, unsigned int vOffset, unsigned int iOffset)
	{
		//Belong Node Index 빼고 모든 Mesh 정보 이동.

		pgMesh->_vertexOffset = vOffset;
		pgMesh->_indexOffset = iOffset;
		 
		pgMesh->_numVertices = assimp->mNumVertices;
		pgMesh->_numIndices = assimp->mNumFaces * 3; //aiProcess_Triangulate를 썼기 때문에 항상 각 Face당 Index 3개.
		pgMesh->_materialID = assimp->mMaterialIndex;

		StoreAssimpAABB(&(assimp->mAABB), &(pgMesh->_AABB));

		//이제 aiMesh 역시 MeshAssetData로 파싱 한정 매핑해서 보관.
		_aiMeshToMeshMap.insert(std::make_pair(assimp, pgMesh));
	}

	void AssimpBufferParser::StoreAssimpAABB(const aiAABB* assimp, DirectX::BoundingBox* pgAABB)
	{
		///이거 해야.
		DirectX::SimpleMath::Vector3 minVec = MathHelper::AI2SM_VECTOR3(assimp->mMin);
		DirectX::SimpleMath::Vector3 maxVec = MathHelper::AI2SM_VECTOR3(assimp->mMax);
		DirectX::SimpleMath::Vector3 middlePoint = 
			DirectX::SimpleMath::Vector3((minVec.x + maxVec.x) / 2.0f, (minVec.y + maxVec.y) / 2.0f, (minVec.z + maxVec.z) / 2.0f);
		DirectX::SimpleMath::Vector3 extent(maxVec.x - minVec.x, maxVec.y - minVec.y, maxVec.z - minVec.z);
		pgAABB->Center = middlePoint;
		pgAABB->Extents = extent;
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

	//Now Defunct.
	void AssimpBufferParser::StoreAssimpAnimation(const aiAnimation* assimp, Animation_AssetData* pgAnim)
	{
		//Now Defunct.
		//pgAnim->_name = assimp->mName.C_Str();
		//pgAnim->_duration = assimp->mDuration;
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
		///아직 디버깅으로 검증 안되었음. 240515 검증해야!

		//먼저 MaterialClusterList가 실행되었어야 실행될 수 있는 코드!

		std::vector<std::string> tRenderT2Vec;
		tRenderT2Vec.resize(outMatClusterList.size());

		eAssetTextureType tAllRequiredPBRTypes[4] = {
			eAssetTextureType::PG_TextureType_DIFFUSE, eAssetTextureType::PG_TextureType_NORMALS,
			eAssetTextureType::PG_TextureType_ARM ,eAssetTextureType::PG_TextureType_OPACITY};

		std::string tIdentifierString[4] =
		{
			"DIFFUSE", "NORMALS", "ARM", "ALPHA"
		};
		//ALPHA 없으면 로드하면 안된다.

		///완벽하지는 않지만, 직전의 Width/Height을 저장한 뒤, 값을 가져올 수 있게 한다.
		/// (디폴트 텍스쳐 제작을 위해)
		unsigned int tPrevWidth = 512;
		unsigned int tPrevHeight = 512;
		eTextureExtension tPrevExt = eTextureExtension::_DDS;

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

					//별도로 다음을 위해 자신의 W/H 기록.
					tPrevWidth = tMatCluster->GetTextureByType(type)->GetFileWidth();
					tPrevHeight = tMatCluster->GetTextureByType(type)->GetFileHeight();

					std::filesystem::path tFSP = tPath;
					tRenderT2Vec.at(i) = tFSP.filename().string();
				}
				else
				{
					//없을 경우, 타입에 맞는 기본 리소스를 넣는다. 이 경우, Default Textures가 로드될 것.
					//UINT tWidth = tMatCluster->GetTextureByType(type)->GetFileWidth();
					//UINT tHeight = tMatCluster->GetTextureByType(type)->GetFileHeight();
					//
					//
					//tRenderT2Vec.at(i) = GraphicsResourceHelper::GetDefaultTexturePath(type, tSize);
					if (k != 3) //Alpha면 nullptr 넣어라.
					{
						eSizeTexture tSize = GraphicsResourceHelper::GetSizeTextureFromUINT(tPrevWidth, tPrevHeight);
						std::filesystem::path tFSP = GraphicsResourceHelper::GetDefaultTexturePath(type, tSize);
						tRenderT2Vec.at(i) = tFSP.filename().string();
					}
					else
					{
						//Alpha인데 없으면, 자동으로 만들어주지 않는다.
						tRenderT2Vec.at(i) = "";
					}
				}
			}

			//Alpha가 있는지 없는지를 기준으로 해야 한다.
			//Alpha 
			
			//어차피 누락되지만, 디버깅하면서 확인하기 위해서.
			std::string defInstMatName = modelName;
			defInstMatName += "_";
			defInstMatName += tIdentifierString[k];
			std::string varName = "PBRTexArray";

			//Alpha가 있는지 없는지 검사.
			bool canBeMade = true;
			for (auto& it : tRenderT2Vec)
			{
				if (it.empty())
				{
					canBeMade = false;
					break;
				}
			}

			if (canBeMade)
			{
				std::string tTempTex2DArrName = GraphicsResourceHelper::GetGeneratedTex2DArrayNameFromValues(defInstMatName, varName, tRenderT2Vec.data(), tRenderT2Vec.size());
				Pg::Graphics::Manager::GraphicsResourceManager::Instance()->LoadResource(tTempTex2DArrName, Pg::Data::Enums::eAssetDefine::_TEXTURE2DARRAY);
				Pg::Graphics::Manager::GraphicsResourceManager::Instance()->AddSecondaryResource(tTempTex2DArrName, Pg::Data::Enums::eAssetDefine::_TEXTURE2DARRAY);

				//로드했으니 이제 가져올 수 있다.
				auto tTex2DRes = Pg::Graphics::Manager::GraphicsResourceManager::Instance()->GetResource(tTempTex2DArrName, Pg::Data::Enums::eAssetDefine::_TEXTURE2DARRAY);

				//값이 해당되는 것처럼 할당된다.
				outArrayData[k] = static_cast<RenderTexture2DArray*>(tTex2DRes.get());
			}
			else
			{
				outArrayData[k] = nullptr;
			}
		}

		assert("");
	}

	void AssimpBufferParser::D3DSetPrivateData(const std::string& modelName, Asset3DModelData* modelData)
	{
		std::string tVbString = "VB : ";
		tVbString.append(modelName);
		std::string tIbString = "IB : ";
		tIbString.append(modelName);

#if defined(DEBUG) | defined(_DEBUG)
		HR(modelData->_vertexBuffer->SetPrivateData(WKPDID_D3DDebugObjectName, tVbString.length(), tVbString.data()));
		HR(modelData->_indexBuffer->SetPrivateData(WKPDID_D3DDebugObjectName, tIbString.length(), tIbString.data()));
#endif

	}

	void AssimpBufferParser::Reset()
	{
		if (!_aiNodeToNodeMap.empty())
		{
			_aiNodeToNodeMap.clear();
		}

		if (!_aiMeshToMeshMap.empty())
		{
			_aiMeshToMeshMap.clear();
		}
	}



}

#pragma region OldCode
//for (unsigned int i = 0; i < mesh->mNumBones; i++) 
		//{
		//	unsigned int BoneIndex = 0;

		//	// Obtain the bone name.
		//	std::string BoneName(mesh->mBones[i]->mName.C_Str());

		//	// If bone isn't already in the map. 
		//	if (skinnedData->_mappedBones.find(BoneName) == skinnedData->_mappedBones.end())
		//	{
		//		// Set the bone ID to be the current total number of bones. 
		//		BoneIndex = skinnedData->_numFormationBone;

		//		// Increment total bones. 
		//		skinnedData->_numFormationBone++;

		//		// Push new bone info into bones vector. 
		//		BoneInfo_AssetData tBi;
		//		skinnedData->_renderBoneInfoVector.push_back(tBi);
		//	}
		//	else {
		//		// Bone ID is already in map. 
		//		BoneIndex = skinnedData->_mappedBones[BoneName];
		//	}

		//	skinnedData->_mappedBones[BoneName] = BoneIndex;

		//	// Obtains the offset matrix which transforms the bone from mesh space into bone space. 
		//	Matrix tBoneOffset = MathHelper::AI2SM_MATRIX(mesh->mBones[i]->mOffsetMatrix);
		//	///기존
		//	//MathHelper::DecomposeAssembleMatrix(tBoneOffset);
		//	skinnedData->_renderBoneInfoVector[BoneIndex]._boneOffset = tBoneOffset.Transpose();

		//	// Iterate over all the affected vertices by this bone i.e weights. 
		//	for (unsigned int j = 0; j < mesh->mBones[i]->mNumWeights; j++) {

		//		// Obtain an index to the affected vertex within the array of vertices.
		//		//unsigned int VertexID = _meshEntriesVector[index]._baseVertex + mesh->mBones[i]->mWeights[j].mVertexId;
		//		unsigned int VertexID = sceneData->_meshList[index]._vertexOffset + mesh->mBones[i]->mWeights[j].mVertexId;

		//		// The value of how much this bone influences the vertex. 
		//		float Weight = mesh->mBones[i]->mWeights[j].mWeight;

		//		// Insert bone data for particular vertex ID. A maximum of 4 bones can influence the same vertex. 
		//		vBoneList[VertexID].AddBoneData(BoneIndex, Weight);
		//	}
		//}
		//assert(mesh);

//void BufferParser::StoreAssimpBone(const aiBone* assimp, Bone_AssetData* pgAABB)
	//{
	//
	//}
#pragma endregion OldCode
