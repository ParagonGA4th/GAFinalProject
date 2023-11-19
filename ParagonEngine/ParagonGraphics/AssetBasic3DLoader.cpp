#include "AssetBasic3DLoader.h"
#include "AssetModelDataDefine.h"
#include "AssetAnimationDataDefine.h"
#include "Asset3DModelData.h"
#include "AssimpBufferParser.h"
#include "../ParagonUtil/ResourceHelper.h"

#include <assimp/Importer.hpp>     
#include <assimp/scene.h>          
#include <assimp/postprocess.h> 
//#include <assimp/material.h> 
#include <cassert>


#ifdef _DEBUG
#pragma comment(lib,"..\\Builds\\x64\\Debug\\ParagonUtil.lib")
#else
#pragma comment(lib,"..\\Builds\\x64\\Release\\ParagonUtil.lib")
#endif // _DEBUG

namespace Pg::Graphics::Loader
{
	using Pg::Graphics::Helper::AssimpBufferParser;

	AssetBasic3DLoader::AssetBasic3DLoader()
	{
		////Assimp 링크 문제 없다는 것을 확인하기 위해.
		_importer = std::make_unique<Assimp::Importer>();

	}

	AssetBasic3DLoader::~AssetBasic3DLoader()
	{
		//
	}

	void AssetBasic3DLoader::Load3DModelBuffer(const std::string& path, Asset3DModelData* modelData)
	{
		assert(modelData->_assetSceneData == nullptr);
		modelData->_assetSceneData = new Pg::Graphics::Scene_AssetData;
		modelData->_isSkinned = IsModelSkinned(path);
		
		if (modelData->_isSkinned)
		{
			//Skinned
			const aiScene* pScene = _importer->ReadFile(path.c_str(),
				aiProcess_Triangulate |
				aiProcess_ConvertToLeftHanded | aiProcess_JoinIdenticalVertices | aiProcess_GenBoundingBoxes |
				aiProcess_CalcTangentSpace | aiProcess_PopulateArmatureData |
				aiProcess_GenSmoothNormals | aiProcess_SortByPType | aiProcess_LimitBoneWeights); //aiProcess_EmbedTextures |
			assert(pScene != nullptr);

			AssimpBufferParser::AssimpToSceneAssetData(pScene, path, modelData->_assetSceneData);
			AssimpBufferParser::AssimpToDXBuffer(true, pScene, modelData->_vertexBuffer, modelData->_indexBuffer);
			AssimpBufferParser::AssimpToMaterialClusterList(pScene, modelData->_materialClusterList, path);
		}
		else
		{
			//Static
			const aiScene* pScene = _importer->ReadFile(path.c_str(),
				aiProcess_Triangulate |
				aiProcess_ConvertToLeftHanded | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType |
				aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals | aiProcess_PreTransformVertices | aiProcess_GenBoundingBoxes); //aiProcess_EmbedTextures |
			assert(pScene != nullptr);

			AssimpBufferParser::AssimpToSceneAssetData(pScene, path, modelData->_assetSceneData);
			AssimpBufferParser::AssimpToDXBuffer(false, pScene, modelData->_vertexBuffer, modelData->_indexBuffer);
			AssimpBufferParser::AssimpToMaterialClusterList(pScene, modelData->_materialClusterList, path);
		}

		//Importer를 재사용하면서, 기존에 있던 데이터 클리어.
		_importer->FreeScene();
	}

	bool AssetBasic3DLoader::IsModelSkinned(const std::string& path)
	{
		//Importer를 재사용하면서, 기존에 있던 데이터 클리어.
		_importer->FreeScene();

		std::string tPath = Pg::Util::Helper::ResourceHelper::ForcePathUniform(path);

		const aiScene* tScene = _importer->ReadFile(path.c_str(),
			aiProcess_GenSmoothNormals | aiProcess_Triangulate | aiProcess_FlipUVs);

		bool tIsSkinned = false;

		// 모든 Mesh를 체크한다. 
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

	

}