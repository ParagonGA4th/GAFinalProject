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
#include "../ParagonUtil/InstancingException.h"

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

//NULL�� �ƴ� ���� ���� �����ϴ� Macro �Լ�.
#define IF_NOT_NULL(_ptr, _action) \
    do { \
        if (_ptr != nullptr) { \
            _action; \
        } \
    } while (false)

//value�� 0�� �ƴ� ���� ���� �����ϴ� Macro �Լ�.
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
	Pg::Util::InstancingException* AssimpBufferParser::_instancingException{};

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
		//Total Vertex/Index Count Ȱ��.
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
		//InputLayout�� ���� 3DModel�� ���� �ε�. (Vin1stStatic)
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
				//Float3�ε�, Blender������ Float2������ �����Ǿ� ���� ���̴�. 
				// ������ Z������ Assimp ��ü Material ID�� ���� Ȱ��� ��. 
				tMeshVert._meshMatID = assimp->mMeshes[i]->mMaterialIndex;
				
				//1��° Texture Coord�� �ִ��� (Lightmap UV ����) => ���̽��� ����.
				if (assimp->mMeshes[i]->HasTextureCoords(1))
				{
					auto tLightmapTex = MathHelper::AI2SM_VECTOR3(assimp->mMeshes[i]->mTextureCoords[1][j]);
					tMeshVert._lightmapUV = { tLightmapTex.x,tLightmapTex.y };
				}
				else
				{
					tMeshVert._lightmapUV = { 0.f, 0.f }; //�ϵ��ڵ�.
				}


				//2nd
				tMeshVert2nd._normalL = MathHelper::AI2SM_VECTOR3(assimp->mMeshes[i]->mNormals[j]);
				tMeshVert2nd._tangentL = MathHelper::AI2SM_VECTOR3(assimp->mMeshes[i]->mTangents[j]);
				if (assimp->mMeshes[i]->mColors[0] != nullptr)
				{
					//Color�� �ִ�.
					aiVector3D tColorStore = { assimp->mMeshes[i]->mColors[0][j].r, assimp->mMeshes[i]->mColors[0][j].g,assimp->mMeshes[i]->mColors[0][j].b };
					tMeshVert2nd._color = MathHelper::AI2SM_VECTOR3(tColorStore);
				}
				else
				{
					//Color�� ����.
					tMeshVert2nd._color = { 1.0f,1.0f, 1.0f, }; //�ϵ��ڵ�.
				}
				
				
				tVBVec.push_back(tMeshVert);
				tVB2ndVec.push_back(tMeshVert2nd);
			}
		}

		//���� ObjMat�� �ν��Ͻ�ó�� �ٷ��� ��.
		//Object / Material ID ������ ���� + ����Ʈ ó���� ����.
		//sceneData->_posRecordVector.resize(vertexCnt);
		//sceneData->_texRecordVector.resize(vertexCnt);
		//sceneData->_meshMatIDRecordVector.resize(vertexCnt);
		//
		////RecordVector�� �ű��.
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
		//�켱������, Skinned Data���� ����.
		StoreIndependentSkinnedData(assimp, skinnedData);

		std::vector<VertexBone_TempAssetData> tVertexBoneVector;
		tVertexBoneVector.resize(sceneData->_totalVertexCount);
		StoreGetDependentSkinnedData(assimp, sceneData, skinnedData, tVertexBoneVector);

		//Total Vertex/Index Count Ȱ��.
		unsigned int tTotalVertexCount = sceneData->_totalVertexCount;
		unsigned int tTotalIndexCount = sceneData->_totalIndexCount;

		ParseAssimpSkinned(assimp, sceneData, skinnedData, tVertexBoneVector, outVB1st, outVB2nd, outIB, tTotalVertexCount, tTotalIndexCount);
	
		//���� �������� ����, ������� ��� ������ ���������� ���� ����Ѵ�.
		//RenderAnimation ������ �ش� ����� �ε����� �´� ���� �־���� ��. (������ nullptr)
		//	=> ���� �ν��Ͻ��� �̵�.
		//RecordNodeToList(sceneData->_rootNode.get(), skinnedData);
	}

	//��Ű�� ������ ��, �ǽð� �����Ϳ� ��� ���� ��Ű�� ������ ���� �Է�.
	void AssimpBufferParser::StoreIndependentSkinnedData(const aiScene* assimp, Skinned_AssetData* skinnedData)
	{
		//Global Inverse Transform ���.
		aiMatrix4x4 tStoreMat = assimp->mRootNode->mTransformation;
		DirectX::XMMATRIX tGlobalTrans = MathHelper::AI2XM_MATRIX(tStoreMat.Transpose());

		DirectX::XMVECTOR tDet = DirectX::XMVectorZero();
		skinnedData->_meshGlobalInverseTransform = DirectX::XMMatrixInverse(&tDet, tGlobalTrans);

		//�������� Dependent���� �Ű��� ��. 
	}

	void AssimpBufferParser::StoreGetDependentSkinnedData(const aiScene* assimp, const Scene_AssetData* sceneData, Skinned_AssetData* skinnedData, std::vector<VertexBone_TempAssetData>& outVertexBoneVector)
	{
		for (unsigned int i = 0; i < assimp->mNumMeshes; i++)
		{
			aiMesh* aMesh = assimp->mMeshes[i];

			//Vertex �ε�� �ε����� ������ �ʱ� ������, ����� ����.
			if (aMesh->HasBones())
			{
				SetupRenderBones(i, aMesh, sceneData, skinnedData, outVertexBoneVector);
			}
		}
	}

	void AssimpBufferParser::SetupRenderBones(unsigned int index, aiMesh* mesh, const Scene_AssetData* sceneData, Skinned_AssetData* skinnedData, std::vector<VertexBone_TempAssetData>& vBoneList)
	{
		//���� ����ü ��ü.
		//�ٸ��� ������ ���� VB/IB�� ������� �ε����� �ٸ��� �о ������ �ϴ� ����.
		//�̽��� �ٸ��� �ؾ� �Ѵ�.
		//VertexBoneVector�� �̹� Model�� ��ü Vertex ������ ������� �Ǿ���.
		
		//Bone Data�� ������ �߰��ؾ� �Ѵ�.
		//�츮�� VB/IB�� �ϳ�. �׷���, �󸶳� �߰��ؼ� ����ؾ� �ϴ����� �ľ��ؾ� �Ѵ�! (Base Vertex)
		Mesh_AssetData* tMeshData = _aiMeshToMeshMap.at(mesh);
		
		for (unsigned int i = 0; i < mesh->mNumBones; i++)
		{
			aiBone* aibone = mesh->mBones[i];

			//��ȣ ����.
			BoneInfo_AssetData* tPgBone = new BoneInfo_AssetData();
			tPgBone->_name = aibone->mName.C_Str();
			tPgBone->_index = _aiNodeToNodeMap.at(aibone->mNode)->_index;
			tPgBone->_bindedNode = _aiNodeToNodeMap.at(aibone->mNode);
			tPgBone->_offsetMatrix = MathHelper::AI2XM_MATRIX(aibone->mOffsetMatrix.Transpose());
			_aiNodeToNodeMap.at(aibone->mNode)->_bindedBone = tPgBone;

			//���� ���� Mesh�� �����Ѵٴ� �� �ݿ��� VertexOffset ������� ����.
			for (unsigned int j = 0; j < aibone->mNumWeights; j++)
			{
				UINT tVertexIndex = aibone->mWeights[j].mVertexId;
				float tWeight = aibone->mWeights[j].mWeight;
				UINT tCombinedVertexIndex = tVertexIndex + tMeshData->_vertexOffset;

				assert((tCombinedVertexIndex < vBoneList.size()) && "��ȿ�� �ε����� ���;� �Ѵ�. ������ VB/IB �̱� ������.");
				vBoneList.at(tCombinedVertexIndex).AddBoneData(tPgBone->_index, tWeight);
			}

		}
		assert("�� ���� ��.");
	}

	void AssimpBufferParser::ParseAssimpSkinned(const aiScene* assimp, Scene_AssetData* sceneData, Skinned_AssetData* skinnedData, const std::vector<VertexBone_TempAssetData>& vertexBoneVector, ID3D11Buffer*& outVB1st, ID3D11Buffer*& outVB2nd, ID3D11Buffer*& outIB, unsigned int vertexCnt, unsigned int indexCnt)
	{
		//���ݱ��� Bone Index/Weight Binding�� ����, �ε��� ī���� ����.
		UINT tTotalElapsedVertexCount = 0;
		UINT tTotalElapsedIndiceCount = 0;

		std::vector<LayoutDefine::Vin1stSkinned_Individual> vertices;
		vertices.resize(vertexCnt);

		std::vector<LayoutDefine::Vin2ndAll_Individual> vertices2nd;
		vertices2nd.resize(vertexCnt);

		std::vector<int32_t> indices;
		indices.resize(indexCnt);
		uint32_t vid = 0, iid = 0;

		//������, ���߿� Material ID, Object ID�� ����ؾ� �ϱ⿡ �ʿ��� ������ Position�� ���. (&& Blend Data Info + ����Ʈ ������)
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
				//���� ����� MultimaterialMesh (���� ����)
				auto& pos = m->mVertices[j];
				auto& norm = m->mNormals[j];
				auto& tan = m->mTangents[j];
				auto& texUV = m->mTextureCoords[0][j];
				unsigned int tMeshMatID = m->mMaterialIndex;
				//�ϴ��� �ϵ��ڵ���.

				//1st
				vertices[vid + j]._posL = DirectX::XMFLOAT3{ pos.x, pos.y, pos.z };
				vertices[vid + j]._tex = DirectX::XMFLOAT2{ texUV.x, texUV.y};
				vertices[vid + j]._meshMatID = tMeshMatID;
				
				//�ϴ��� ���� �� �´� �� ����.
				vertices[vid + j]._blendIndice0 = vertexBoneVector.at(j + tTotalElapsedVertexCount).IDs[0];
				vertices[vid + j]._blendIndice1 = vertexBoneVector.at(j + tTotalElapsedVertexCount).IDs[1];
				vertices[vid + j]._blendIndice2 = vertexBoneVector.at(j + tTotalElapsedVertexCount).IDs[2];
				vertices[vid + j]._blendIndice3 = vertexBoneVector.at(j + tTotalElapsedVertexCount).IDs[3];

				vertices[vid + j]._blendWeight0 = vertexBoneVector.at(j + tTotalElapsedVertexCount).Weights[0];
				vertices[vid + j]._blendWeight1 = vertexBoneVector.at(j + tTotalElapsedVertexCount).Weights[1];
				vertices[vid + j]._blendWeight2 = vertexBoneVector.at(j + tTotalElapsedVertexCount).Weights[2];
				vertices[vid + j]._blendWeight3 = vertexBoneVector.at(j + tTotalElapsedVertexCount).Weights[3];

				//SKINNED�� Lightmap UV�� �Ұ����ϴ�! Node Index�� �� �ٲ� ��. 
				//Node Index �� ����.
				//vertices[vid + j]._nodeIndex = sceneData->_meshList[i]._belongNodeIndex;

				//2nd
				vertices2nd[vid + j]._normalL = DirectX::XMFLOAT3{ norm.x, norm.y, norm.z };
				vertices2nd[vid + j]._tangentL = DirectX::XMFLOAT3{ tan.x, tan.y, tan.z };
				vertices2nd[vid + j]._color = DirectX::XMFLOAT3{ 1.0f,1.0f, 1.0f }; //�ϵ��ڵ�.

				//RecordVector�� ��� (���߿� Object / Material ID ���� ���� ����) + ����Ʈ ������ ���� ����.
				//sceneData->_posRecordVector.at(vid + j) = vertices[vid + j]._posL;
				//sceneData->_texRecordVector.at(vid + j) = vertices[vid + j]._tex;
				//sceneData->_meshMatIDRecordVector.at(vid + j) = vertices[vid + j]._meshMatID;
				//
				////�ϴ��� ���� �� �´� �� ����.
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

	void AssimpBufferParser::AssimpToMaterialClusterList(const aiScene* assimp, bool& bCheckIfUseAlphaBlending, bool& bChecKIfUseAlphaClipping, std::vector<MaterialCluster*>& outMatClusterList, const std::string& directory)
	{
		//�̸� GraphicsResourceManager �޾ƿ���.
		GraphicsResourceManager* tGraphicsResourceManager = GraphicsResourceManager::Instance();

		unsigned int tNumMaterials = assimp->mNumMaterials;
		outMatClusterList.reserve(assimp->mNumMaterials);

		//���� ������ mesh �ȿ��� AlphaMap�� Ȱ���ϴ���, �ٷ� �ڴ� ���� �ƴ϶�, Mesh ���� Content Checking (MeshMaterial) �� ��.
		std::set<int> useAlphaMappingList;

		//�ϴ� ������� MaterialCluster�� ���ҽ� �Ŵ������� ���������� �ʴ´�.
		//�ٸ�, ������ RenderTexture2D�� ������ ���̴�.
		for (int i = 0; i < assimp->mNumMaterials; i++) // i = Material Index.
		{
			MaterialCluster* tMatCluster = new MaterialCluster();

			for (size_t j = 0; j <= Pg::Defines::ASSET_MAXIMUM_TEXTURE_PROP_CNT; j++)
			{
				eAssetTextureType tTexType = static_cast<eAssetTextureType>(j); //Texture Type(j)
				int tTexTypeTexCnt = assimp->mMaterials[i]->GetTextureCount((aiTextureType)tTexType);
				if (tTexTypeTexCnt > 0)
				{
					//�ٸ� ��� ������ ���ư��� ����, Alpha�� ����ϴµ� TexCnt�� ������ �ش� AssetModelData�� üũ�ؾ� �Ѵ�.
					if (tTexType == PG_TextureType_OPACITY)
					{
						useAlphaMappingList.insert(i);
					}

					aiString tAssimpTexturePath;
					assimp->mMaterials[i]->GetTexture((aiTextureType)tTexType, 0, &tAssimpTexturePath); //�׻� 0��° Texture���� ��������!
					
					std::string tTextureName;
					std::string tFbmPart;
					std::string tFBMPartNoExt;
					//���ҽ� fbm fbx �̸� ���� ������ �� �ְ�.
					{
						std::string tTexturePath = tAssimpTexturePath.C_Str();
						std::filesystem::path tTempRecordingPath(tTexturePath);
						tTextureName = tTempRecordingPath.filename().string();
					}
					{
						std::filesystem::path tTempRecordingPath(directory);
						tFBMPartNoExt = tTempRecordingPath.stem().string();
						tFbmPart = tFBMPartNoExt.append(".fbm");
					}

					//������, Directory���� .fbx��� ���� ��ο��� TextureX�� �Ļ��Ǵ� ���� �ƴ϶�,
					//"�Ѵܰ�" ���� ��η� ���� ã�ƾ� �Ѵ�.
					std::filesystem::path modelParentPath = directory;
					std::string modelParentStr = modelParentPath.parent_path().string();
					std::string tCompletePath = modelParentStr + '/' + tFbmPart + '/' + tTextureName;

					tCompletePath = ResourceHelper::ForcePathUniform(tCompletePath);

					//�̹� �ش� �̸����� �� ���ҽ��� ���ٸ�
					if (!tGraphicsResourceManager->IsExistResource(tCompletePath))
					{
						//���⸦ ���� Embedding ���� ���� ���� ��θ��� ������ ������ �� �ְ� �պ��� �Ѵ�.
						//Embedded Texture�� �ִ����� �˻��� �ʿ� ����, Path�� ������ ������ ������ .fbm ���θ� ã�� �ؾ� ��!
						
						//�ϴ�, ���� �̸��� ������ �ִ� ���ҽ��� �ִ������� Ȯ��.
						std::filesystem::path tPath = tCompletePath;
						std::string tFilename = tPath.filename().string();

						if (tGraphicsResourceManager->IsExistResourceByName(tFilename))
						{
							//���� ���� �̸��� ���� ���ҽ��� �ִ�. ������ "Complete" Path�� ����.
							tCompletePath = tGraphicsResourceManager->GetResourcePathByName(tFilename, eAssetDefine::_TEXTURE2D);

							//�� ���, �̹� �־��ٴ� ���ε�,
							//���� tCompletePath�� �� tFBMPartNoExt�� �ȵ�� �־��ٸ� ���� �����!
							{
								std::size_t ind = tCompletePath.find(tFBMPartNoExt); //Substring ���� ��ġ ã��.
								//����
								if (ind == std::string::npos)
								{
									//�� ã������ Warning ���.
									PG_WARN("{0} : �ٸ� FBX�� �Ҵ�� FBM ���� �ؽ���, {1} Ȱ��.", tFbmPart, tCompletePath);
								}
							}
							
						}
						else
						{
							//�ϴ��� �ش� ���ҽ���� �ϴ� GraphicsResourceManager�� �߰�.
							tGraphicsResourceManager->LoadResource(tCompletePath, eAssetDefine::_TEXTURE2D);
							//AssetManager�� ���� ����.
							tGraphicsResourceManager->AddSecondaryResource(tCompletePath, eAssetDefine::_TEXTURE2D);
						}
					}
;
					//�̹� ������ ���� �̸����� �ε�� RenderTexture2D�� �ִ�.
					auto tTexture2dData = tGraphicsResourceManager->GetResource(tCompletePath, Pg::Data::Enums::eAssetDefine::_TEXTURE2D);
					tMatCluster->_atsList.at(j) = static_cast<RenderTexture2D*>(tTexture2dData.get());

					////����� ����Ʈ���� ���.
					//tMatCluster->_debugList.at(j).second = true;
				}
			}
			outMatClusterList.push_back(tMatCluster);
		}
		
		if (useAlphaMappingList.empty())
		{
			//Early Return. ���ʿ� OPACITY �߰� �ȵǸ� Alpha Blending�� Ȱ���� ���� ������.
			bCheckIfUseAlphaBlending = false;
			return;
		}


		bool tUseBlend = false;
		//Alpha Blending�� ����ϴ� MeshMaterial�� ������ �� ����ϴ��� ����.
		for (uint32_t i = 0; i < assimp->mNumMeshes; i++)
		{
			uint32_t tMeshMatID = assimp->mMeshes[i]->mMaterialIndex;
			
			auto it = useAlphaMappingList.find(tMeshMatID);
		
			if (it != useAlphaMappingList.end())
			{
				//���ĸ����Ѵٴ� ����. ã�ұ� ����.
				tUseBlend = true;

				//����
				std::filesystem::path tTempRecordingPath(directory);
				std::string tFilename = tTempRecordingPath.filename().string();
				std::string tPrefixFromName = tFilename.substr(0, 5);

				//�ٵ� clip_���� ����Ѵ�? �׷��� Alpha Clipping ����.
				//���� �����ϴ� ������ �ٸ� ���̴��� ��ϵ� ��. Sorting ���� ���ܵ� ��.
				if (tPrefixFromName.compare(Pg::Defines::CLIPPED_3DMODEL_PREFIX) == 0)
				{
					bChecKIfUseAlphaClipping = true;
				}
				else
				{
					bChecKIfUseAlphaClipping = false;
				}

				break;
			}
		}

		//���� ������ ���� material�� ������, �� fbx�� mesh ���� material���� �̸� �� ���ٸ�? => AlphaBlending==false;
		//�ƴϸ� True.
		bCheckIfUseAlphaBlending = tUseBlend;
		return;
	}

	void AssimpBufferParser::AssimpToSceneAssetData(const aiScene* assimp, const std::string& path, Scene_AssetData* outSceneAssetData)
	{
		outSceneAssetData->_directory = path;

		//Mesh�� Node ����� ����, Mesh�� �켱���� ����Ѵ�.
		outSceneAssetData->_totalMeshCount = assimp->mNumMeshes;
		outSceneAssetData->_meshList.resize(outSceneAssetData->_totalMeshCount);

		//Vertex, Index Count �ޱ� ����.
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

		//����, ������ Node�� Mesh�� �����ϱ� ���� Node ��ϰ� �Բ� Mesh-Node �����Ѵ�.
		outSceneAssetData->_rootNode = std::make_unique<Node_AssetData>(nullptr);
		//outSceneAssetData->_rootNode->_relTransform = std::make_unique<Pg::Data::Transform>(nullptr); 
		//�ڽ� ����� Transform ���� ��ü�� �״�� �������ش�. (�׿��� �Ҽ� Transform �ƴ�)

		//Node �̵� �ߵ��� �ñ� : -> Mesh�� �� �޾��� ��.
		UINT tIndexForNode = 0;
		StoreAssimpNode(assimp->mRootNode, outSceneAssetData, outSceneAssetData->_rootNode.get(), tIndexForNode);
	}

	//Parent�� ���� ������ ������ �����Ѵ�.
	void AssimpBufferParser::StoreAssimpNode(const aiNode* assimp, Scene_AssetData* sceneData, Node_AssetData* pgNode, UINT& index)
	{
		pgNode->_nodeName = assimp->mName.C_Str();

		//Transpose�ؼ� ���� (Column Major <-> Row Major)
		aiMatrix4x4 tStoreTrans = assimp->mTransformation;
		{
			DirectX::SimpleMath::Matrix tRelTrans = MathHelper::AI2XM_MATRIX(tStoreTrans.Transpose());
			pgNode->_offsetMatrix = tRelTrans;
		}
		
		//���� ���� FBX ���ο��� �����ϴ� Index ���� ����. ��� �� Increment.
		pgNode->_index = index;
		index++;

		//Assimp�� PgNode �����ϴ� unordered_map�� ����. (�Ŀ� Bone-Binding)
		_aiNodeToNodeMap.insert(std::make_pair(assimp,pgNode));

		pgNode->_numMeshes = assimp->mNumMeshes;
		for (int i = 0; i < pgNode->_numMeshes; i++)
		{
			//�̹� ����� ������ �������� �ű��. (Mesh_AssetData)
			pgNode->_meshList.push_back(&(sceneData->_meshList.at(assimp->mMeshes[i])));

			//Node ������ MeshList ���� ����������, ���������� Node�� �ε��� ���� Mesh�� �����ϰ� �Ѵ�.
			sceneData->_meshList.at(assimp->mMeshes[i])._belongNodeIndex = pgNode->_index;
		}

		pgNode->_numChildren = assimp->mNumChildren;
		pgNode->_childrenList.reserve(pgNode->_numChildren);

		for (int i = 0; i < pgNode->_numChildren; i++)
		{
			//�̶� Parent �����ߴ� ���̴�.
			pgNode->_childrenList.push_back(std::make_unique<Node_AssetData>(pgNode));
		
			StoreAssimpNode(assimp->mChildren[i], sceneData, pgNode->_childrenList[i].get(), index);
		}
	}

	void AssimpBufferParser::StoreAssimpMesh(const aiMesh* assimp, Mesh_AssetData* pgMesh, unsigned int vOffset, unsigned int iOffset)
	{
		//Belong Node Index ���� ��� Mesh ���� �̵�.

		pgMesh->_vertexOffset = vOffset;
		pgMesh->_indexOffset = iOffset;
		 
		pgMesh->_numVertices = assimp->mNumVertices;
		pgMesh->_numIndices = assimp->mNumFaces * 3; //aiProcess_Triangulate�� ��� ������ �׻� �� Face�� Index 3��.
		pgMesh->_materialID = assimp->mMaterialIndex;

		StoreAssimpAABB(&(assimp->mAABB), &(pgMesh->_AABB));

		//���� aiMesh ���� MeshAssetData�� �Ľ� ���� �����ؼ� ����.
		_aiMeshToMeshMap.insert(std::make_pair(assimp, pgMesh));
	}

	void AssimpBufferParser::StoreAssimpAABB(const aiAABB* assimp, DirectX::BoundingBox* pgAABB)
	{
		///�̰� �ؾ�.
		DirectX::SimpleMath::Vector3 minVec = MathHelper::AI2SM_VECTOR3(assimp->mMin);
		DirectX::SimpleMath::Vector3 maxVec = MathHelper::AI2SM_VECTOR3(assimp->mMax);
		DirectX::SimpleMath::Vector3 middlePoint = 
			DirectX::SimpleMath::Vector3((minVec.x + maxVec.x) / 2.0f, (minVec.y + maxVec.y) / 2.0f, (minVec.z + maxVec.z) / 2.0f);
		DirectX::SimpleMath::Vector3 extent((maxVec.x - minVec.x) / 2.0f, (maxVec.y - minVec.y) / 2.0f, (maxVec.z - minVec.z) / 2.0f);
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

	void AssimpBufferParser::AssimpToPBRTextureArray(const std::string& modelName, const std::string& modelPath, bool isSkinned, std::vector<MaterialCluster*>& outMatClusterList, RenderTexture2DArray** outArrayData)
	{
		//���� ��������� ���� �ȵǾ���. 240515 �����ؾ�!

		//���� MaterialClusterList�� ����Ǿ���� ����� �� �ִ� �ڵ�!

		//�ϴ�, Model Name�� Instance�� ������ �޴���, �ƴ����� �˾ƺ���.
		bool tRenderedIndividually = false;
		bool tIsAlphaClipped = false;
		{
			std::string tPrefixFromName = modelName.substr(0, 5);
			std::string tPrefixFromNameOneLonger = modelName.substr(0, 6);
			bool tIsPartOfInstanceException = _instancingException->IsExceptionFromInstance(modelPath);
			if (tPrefixFromName.compare(Pg::Defines::NON_INSTANCED_3DMODEL_PREFIX) == 0 ||
				tPrefixFromNameOneLonger.compare(Pg::Defines::BLENDED_OPTIONAL_3DMODEL_PREFIX) == 0 ||
				isSkinned || 
				tIsPartOfInstanceException)
			{
				//�ν��Ͻ� ���� X, BaseColor / Normal / ARM ���� �ʿ�.
				//��� Alpha Blended Object���� ���� 4���� �⺻ PBR ���� ���� �ʿ�.
				tRenderedIndividually = true;
			}
			else
			{
				//�ν��Ͻ��� ����Ǵ� ����� �⺻���� Mesh.
				//��� �̶����ʹ�, Normal / ARM�� ������ �ʿ䰡 ����.
				//Lightmap�� �˾Ƽ� ó���ϱ� ������!
				if (tPrefixFromName.compare(Pg::Defines::CLIPPED_3DMODEL_PREFIX) == 0)
				{
					tIsAlphaClipped = true;
				}
				else
				{
					tIsAlphaClipped = false;
				}

			}

		}


		std::vector<std::string> tRenderT2Vec;
		tRenderT2Vec.resize(outMatClusterList.size());

		eAssetTextureType tAllRequiredPBRTypes[4] = {
			eAssetTextureType::PG_TextureType_DIFFUSE, eAssetTextureType::PG_TextureType_NORMALS,
			eAssetTextureType::PG_TextureType_ARM ,eAssetTextureType::PG_TextureType_OPACITY};

		std::string tIdentifierString[4] =
		{
			"DIFFUSE", "NORMALS", "ARM", "ALPHA"
		};
		//ALPHA ������ �ε��ϸ� �ȵȴ�.

		///�Ϻ������� ������, ������ Width/Height�� ������ ��, ���� ������ �� �ְ� �Ѵ�.
		/// (����Ʈ �ؽ��� ������ ����)
		unsigned int tPrevWidth = 512;
		unsigned int tPrevHeight = 512;
		eTextureExtension tPrevExt = eTextureExtension::_DDS;

		//outArrayData�� �ε����� �ǹ� ����.
		for (int k = 0; k < 4; k++)
		{
			//���⿡�� �ʿ��� ���ҽ��� �׷��� ���� �ֵ��� �����ؾ�!
			bool tNeedsDefaultTextureType = false;
			if (tRenderedIndividually)
			{
				//BaseColor / Normal / ARM / �׸��� ���� Blend�� ����ϸ� Alpha ���� �ʿ�.
				tNeedsDefaultTextureType = true;
			}
			else
			{
				if (tIsAlphaClipped)
				{
					//BaseColor / Alpha�� �ʿ��ϴ�.
					if (k == 0 || k == 3)
					{
						tNeedsDefaultTextureType = true;
					}
				}
				else
				{
					//BaseColor�� �ʿ��ϴ�. // �׳� �ν��Ͻ��̴�.
					if (k == 0)
					{
						tNeedsDefaultTextureType = true;
					}
				}
			}

			for (short i = 0; i < outMatClusterList.size(); i++)
			{
				MaterialCluster* tMatCluster = outMatClusterList.at(i);
				std::string tPath = "";
				eAssetTextureType type = tAllRequiredPBRTypes[k];
				if (tMatCluster->GetTextureByType(type) != nullptr)
				{
					//������ ���� ���� ���, ���� �ε��ؼ� �ִ´�.
					tPath = tMatCluster->GetTextureByType(type)->GetFilePath();

					//������ ������ ���� �ڽ��� W/H ���.
					tPrevWidth = tMatCluster->GetTextureByType(type)->GetFileWidth();
					tPrevHeight = tMatCluster->GetTextureByType(type)->GetFileHeight();

					std::filesystem::path tFSP = tPath;
					tRenderT2Vec.at(i) = tFSP.filename().string();
				}
				else
				{
					//���� ���, Ÿ�Կ� �´� �⺻ ���ҽ��� �ִ´�. �� ���, Default Textures�� �ε�� ��.
					//UINT tWidth = tMatCluster->GetTextureByType(type)->GetFileWidth();
					//UINT tHeight = tMatCluster->GetTextureByType(type)->GetFileHeight();
					//
					//
					//tRenderT2Vec.at(i) = GraphicsResourceHelper::GetDefaultTexturePath(type, tSize);
					if (tNeedsDefaultTextureType && k != 3)
					{
						eSizeTexture tSize = GraphicsResourceHelper::GetSizeTextureFromUINT(tPrevWidth, tPrevHeight);
						std::filesystem::path tFSP = GraphicsResourceHelper::GetDefaultTexturePath(type, tSize);
						tRenderT2Vec.at(i) = tFSP.filename().string();
					}
					else
					{
						//Alpha�ε� ������, �ڵ����� ��������� �ʴ´�. + �׸��� ���� ��� �߰�.
						tRenderT2Vec.at(i) = "";
					}
					
				}
			}

			//Alpha�� �ִ��� �������� �������� �ؾ� �Ѵ�.
			//Alpha 
			
			//������ ����������, ������ϸ鼭 Ȯ���ϱ� ���ؼ�.
			std::string defInstMatName = modelName;
			defInstMatName += "_";
			defInstMatName += tIdentifierString[k];
			std::string varName = "PBRTexArray";

			//tRenderTVec�� ���� Mesh Material �ȿ��� ���� Type���� Texture2DArray�� �ε����� ������ �� �ְ� �� ���̴�.
			//���� ���� �˻��Ѵ�. -> ��������� Default Texture Array�� ����������� �ʴ´�!
			//�׷��� ���� ���, FBX �ϳ��� �ν��Ͻ� ����ε� BASECOLOR / NORMAL / ARM ��� ��� ������ ���������� �Ѵ�.
			//�ٸ�, ����Ʈ ���� ���;� �� �� �̸� �ź��Ѵٴ� ���̴�.

			//Alpha�� �ִ��� ������ �˻�.
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

				//�ε������� ���� ������ �� �ִ�.
				auto tTex2DRes = Pg::Graphics::Manager::GraphicsResourceManager::Instance()->GetResource(tTempTex2DArrName, Pg::Data::Enums::eAssetDefine::_TEXTURE2DARRAY);

				//���� �ش�Ǵ� ��ó�� �Ҵ�ȴ�.
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

	void AssimpBufferParser::Initialize()
	{
		//
		_instancingException = &singleton<Pg::Util::InstancingException>();
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
		//	///����
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
