#include "AssetBasic3DLoader.h"
#include "Asset3DModelData.h"
#include "AssetModelDataDefine.h"
#include "AssetAnimationDataDefine.h"
#include "AssimpBufferParser.h"
#include "RenderPrepStructs.h"
#include "RenderObjectInstancedMesh3D.h"
#include "DX11Headers.h"
#include "LowDX11Storage.h"
#include "LayoutDefine.h"
#include "../ParagonHelper/ResourceHelper.h"

#include <assimp/Importer.hpp>     
#include <assimp/scene.h>          
#include <assimp/postprocess.h> 
//#include <assimp/material.h> 
#include <cassert>

namespace Pg::Graphics::Loader
{
	using Pg::Graphics::Helper::AssimpBufferParser;

	AssetBasic3DLoader::AssetBasic3DLoader()
	{
		////Assimp ??? ???? ????? ???? ?????? ????.
		_importer = std::make_unique<Assimp::Importer>();

	}

	AssetBasic3DLoader::~AssetBasic3DLoader()
	{
		//
	}

	void AssetBasic3DLoader::Load3DModelBuffer(const std::string& path, Asset3DModelData* modelData)
	{
		assert(modelData->_assetSceneData == nullptr);
		assert(modelData->_assetSkinnedData == nullptr);
		modelData->_assetSceneData = new Pg::Graphics::Scene_AssetData;
		modelData->_isSkinned = IsModelSkinned(path);
		
		if (modelData->_isSkinned)
		{
			//SkinnedAssetData ���� �Ҵ�.
			modelData->_assetSkinnedData = new Skinned_AssetData;

			//Skinned 
			//const aiScene* pScene = _importer->ReadFile(path.c_str(),
			//	aiProcess_Triangulate |
			//	aiProcess_ConvertToLeftHanded | aiProcess_JoinIdenticalVertices | aiProcess_GenBoundingBoxes |
			//	aiProcess_CalcTangentSpace | aiProcess_PopulateArmatureData |
			//	aiProcess_GenSmoothNormals | aiProcess_SortByPType | aiProcess_FixInfacingNormals | aiProcess_LimitBoneWeights); //aiProcess_EmbedTextures |
			//

			//const aiScene* pScene = _importer->ReadFile(path.c_str(),
			//	aiProcess_Triangulate
			//	| aiProcess_ConvertToLeftHanded
			//	| aiProcess_PopulateArmatureData
			//	| aiProcess_CalcTangentSpace
			//	| aiProcess_LimitBoneWeights
			//	| aiProcess_GenBoundingBoxes
			//);

			const aiScene* pScene = _importer->ReadFile(path.c_str(),
				aiProcess_Triangulate |
				aiProcess_ConvertToLeftHanded | aiProcess_JoinIdenticalVertices | aiProcess_GenBoundingBoxes |
				aiProcess_CalcTangentSpace | aiProcess_PopulateArmatureData |
				aiProcess_GenSmoothNormals | aiProcess_SortByPType | aiProcess_FixInfacingNormals | aiProcess_LimitBoneWeights);

			assert(pScene != nullptr);

			if (pScene == nullptr || pScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || pScene->mRootNode == nullptr)
			{
				MessageBox(NULL, L"�� ������ �ε��� �� ����. Assimp Skinned ����!", L"Error!", MB_ICONERROR | MB_OK);
				assert(false);
			}

			AssimpBufferParser::AssimpToSceneAssetData(pScene, path, modelData->_assetSceneData);
			AssimpBufferParser::AssimpToSkinnedDataDXBuffer(pScene, modelData->_assetSceneData, modelData->_assetSkinnedData, modelData->_vertexBuffer, modelData->_secondVertexBuffer, modelData->_indexBuffer);
			//���⼭ Alpha Blending ����ϴ��� ���� �����ڸ� ���� ��ȯ�޴´�.
			AssimpBufferParser::AssimpToMaterialClusterList(pScene, modelData->_isUseAlphaBlending, modelData->_isUseAlphaClipping, modelData->_materialClusterList, path);
			AssimpBufferParser::AssimpToPBRTextureArray(modelData->GetFileName(), modelData->GetFilePath(), modelData->_isSkinned, modelData->_materialClusterList, modelData->_pbrTextureArrays);
			AssimpBufferParser::D3DSetPrivateData(modelData->GetFileName(), modelData);
			AssimpBufferParser::Reset();
		}
		else
		{
			//Static
			const aiScene* pScene = _importer->ReadFile(path.c_str(),
				aiProcess_Triangulate |
				aiProcess_ConvertToLeftHanded | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType |
				aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals | aiProcess_PreTransformVertices | aiProcess_GenBoundingBoxes); //aiProcess_EmbedTextures |
			assert(pScene != nullptr);

			if (pScene == nullptr || pScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || pScene->mRootNode == nullptr)
			{
				MessageBox(NULL, L"�� ������ �ε��� �� ����. Assimp Static ����!", L"Error!", MB_ICONERROR | MB_OK);
				assert(false);
			}

			AssimpBufferParser::AssimpToSceneAssetData(pScene, path, modelData->_assetSceneData);
			AssimpBufferParser::AssimpToStaticDataDXBuffer(pScene, modelData->_assetSceneData, modelData->_vertexBuffer, modelData->_secondVertexBuffer, modelData->_indexBuffer);
			//���⼭ Alpha Blending ����ϴ��� ���� �����ڸ� ���� ��ȯ�޴´�.
			AssimpBufferParser::AssimpToMaterialClusterList(pScene, modelData->_isUseAlphaBlending, modelData->_isUseAlphaClipping, modelData->_materialClusterList, path);
			AssimpBufferParser::AssimpToPBRTextureArray(modelData->GetFileName(), modelData->GetFilePath(), modelData->_isSkinned, modelData->_materialClusterList, modelData->_pbrTextureArrays);
			AssimpBufferParser::D3DSetPrivateData(modelData->GetFileName(), modelData);
			AssimpBufferParser::Reset();
		}

		//Importer�� �� ������, ���� �޸𸮸� Ǯ���ش�.
		_importer->FreeScene();
	}

	bool AssetBasic3DLoader::IsModelSkinned(const std::string& path)
	{
		//Importer?? ???????, ?????? ??? ?????? ?????.
		_importer->FreeScene();

		std::string tPath = Pg::Util::Helper::ResourceHelper::ForcePathUniform(path);

		const aiScene* tScene = _importer->ReadFile(path.c_str(),
			aiProcess_GenSmoothNormals | aiProcess_Triangulate | aiProcess_FlipUVs);

		bool tIsSkinned = false;

		// ??? Mesh?? u????. 
		for (unsigned int i = 0; i < tScene->mNumMeshes; ++i) 
		{
			aiMesh* mesh = tScene->mMeshes[i];

			if (mesh->HasBones()) 
			{
				tIsSkinned = true;
				break;
			}
		}
		_importer->FreeScene();

		return tIsSkinned;
	}

	//void AssetBasic3DLoader::LoadObjMatBufferStatic(ID3D11Buffer*& vb, Asset3DModelData* modelData, unsigned int objectID, unsigned int materialID)
	//{
	//	std::vector<LayoutDefine::Vin3rdStaticSkinned_Individual> tVBVector;
	//	tVBVector.reserve(modelData->_assetSceneData->_totalVertexCount);
	//	for (int i = 0; i < modelData->_assetSceneData->_totalVertexCount; i++)
	//	{
	//		LayoutDefine::Vin3rdStaticSkinned_Individual tVal;
	//		//tVal._posL = modelData->_assetSceneData->_posRecordVector[i];
	//		//�̷��� PER_INSTANCE_DATA�� ����. ������ ��� ���� ������ ������ ���ݾ�!
	//		tVal._objectID = objectID;
	//		tVal._matID = materialID;
	//		//tVal._tex = modelData->_assetSceneData->_texRecordVector[i];
	//		//tVal._meshMatID = modelData->_assetSceneData->_meshMatIDRecordVector[i];
	//
	//		tVBVector.push_back(tVal);
	//	}
	//
	//	D3D11_BUFFER_DESC tVBD;
	//	tVBD.Usage = D3D11_USAGE_IMMUTABLE;
	//	tVBD.ByteWidth = static_cast<UINT>(sizeof(LayoutDefine::Vin3rdStaticSkinned_Individual) * modelData->_assetSceneData->_totalVertexCount);
	//	tVBD.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//	tVBD.CPUAccessFlags = 0;
	//	tVBD.MiscFlags = 0;
	//	D3D11_SUBRESOURCE_DATA vinitData;
	//	vinitData.pSysMem = tVBVector.data();
	//
	//	HR(LowDX11Storage::GetInstance()->_device->CreateBuffer(&tVBD, &vinitData, &vb));
	//}
	//
	//void AssetBasic3DLoader::LoadObjMatBufferSkinned(ID3D11Buffer*& vb, Asset3DModelData* modelData, unsigned int objectID, unsigned int materialID)
	//{
	//	std::vector<LayoutDefine::Vin3rdStaticSkinned_Individual> tVBVector;
	//	tVBVector.reserve(modelData->_assetSceneData->_totalVertexCount);
	//
	//	for (int i = 0; i < modelData->_assetSceneData->_totalVertexCount; i++)
	//	{
	//		LayoutDefine::Vin3rdStaticSkinned_Individual tVal;
	//		//tVal._posL = modelData->_assetSceneData->_posRecordVector[i];
	//		//tVal._tex = modelData->_assetSceneData->_texRecordVector[i];
	//		//tVal._meshMatID = modelData->_assetSceneData->_meshMatIDRecordVector[i];
	//		tVal._objectID = objectID;
	//		tVal._matID = materialID;
	//
	//		//tVal._blendIndice0 = modelData->_assetSkinnedData->_blendDataRecordVector[i]._blendIndice0;
	//		//tVal._blendIndice1 = modelData->_assetSkinnedData->_blendDataRecordVector[i]._blendIndice1;
	//		//tVal._blendIndice2 = modelData->_assetSkinnedData->_blendDataRecordVector[i]._blendIndice2;
	//		//tVal._blendIndice3 = modelData->_assetSkinnedData->_blendDataRecordVector[i]._blendIndice3;
	//		//tVal._blendWeight0 = modelData->_assetSkinnedData->_blendDataRecordVector[i]._blendWeight0;
	//		//tVal._blendWeight1 = modelData->_assetSkinnedData->_blendDataRecordVector[i]._blendWeight1;
	//		//tVal._blendWeight2 = modelData->_assetSkinnedData->_blendDataRecordVector[i]._blendWeight2;
	//
	//		tVBVector.push_back(tVal);
	//	}
	//
	//	D3D11_BUFFER_DESC tVBD;
	//	tVBD.Usage = D3D11_USAGE_IMMUTABLE;
	//	tVBD.ByteWidth = static_cast<UINT>(sizeof(LayoutDefine::Vin3rdStaticSkinned_Individual) * modelData->_assetSceneData->_totalVertexCount);
	//	tVBD.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//	tVBD.CPUAccessFlags = 0;
	//	tVBD.MiscFlags = 0;
	//	D3D11_SUBRESOURCE_DATA vinitData;
	//	vinitData.pSysMem = tVBVector.data();
	//
	//	HR(LowDX11Storage::GetInstance()->_device->CreateBuffer(&tVBD, &vinitData, &vb));
	//}

	void AssetBasic3DLoader::LoadObjMatTRSBufferInstanced(ID3D11Buffer*& vb, const std::vector<RenderObjectInstancedMesh3D*>& instancedMeshList)
	{
		//Instanced Mesh List�� ��������� ����.
		assert((!instancedMeshList.empty()) && "���� InstancedMeshList�� ��������� �ȵ�.");

		//�̰� �� �̻� Model�� �������� ���� �ƴ϶�,
		//������ Scene �ȿ� N���� ������Ʈ�� �ִ��� (�����Ǵ�)�� ���� ���� �޶����� ���̴�.
		std::vector<LayoutDefine::Vin3rdInstanced_Individual> tInstancedVector;
		tInstancedVector.reserve(instancedMeshList.size());

		//Instanced Mesh ����Ʈ ä���.
		for (int i = 0; i < instancedMeshList.size(); i++)
		{
			LayoutDefine::Vin3rdInstanced_Individual tElement;
			tElement._matID = instancedMeshList.at(i)->GetMaterialID();
			tElement._objectID = instancedMeshList.at(i)->GetObjectID();

			if (tElement._matID > 100000)
			{
				//�Ұ�.
				assert(false && "�Ұ�.");
			}

			//Transform ��������.
			Pg::Math::PGFLOAT4X4 tWorldTM = instancedMeshList.at(i)->GetBaseRenderer()->_object->_transform.GetWorldTM();
			DirectX::XMMATRIX tWorldTMMat = Pg::Math::PG2XM_MATRIX4X4(tWorldTM);
			tWorldTMMat = DirectX::XMMatrixMultiply(DirectX::XMMatrixScaling(0.01f, 0.01f, 0.01f), tWorldTMMat);
			tElement._transform = tWorldTMMat;
			tInstancedVector.push_back(tElement);
		}

		D3D11_BUFFER_DESC tVBD;
		ZeroMemory(&tVBD, sizeof(tVBD));

		tVBD.Usage = D3D11_USAGE_DEFAULT;
		tVBD.ByteWidth = static_cast<UINT>(sizeof(LayoutDefine::Vin3rdInstanced_Individual) * instancedMeshList.size());
		tVBD.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		tVBD.CPUAccessFlags = 0;
		tVBD.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA vinitData;
		vinitData.pSysMem = tInstancedVector.data();

		HR(LowDX11Storage::GetInstance()->_device->CreateBuffer(&tVBD, &vinitData, &vb));

	}


}