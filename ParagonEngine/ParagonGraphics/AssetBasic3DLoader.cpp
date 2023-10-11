#include "AssetBasic3DLoader.h"
#include "Asset3DModelDefine.h"
#include "Asset3DModelHelper.h"
#include "Asset3DModelData.h"
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
	AssetBasic3DLoader::AssetBasic3DLoader()
	{
		////Assimp 링크 문제 없다는 것을 확인하기 위해.
		//Assimp::Importer importer;
		//const aiScene* pScene = importer.ReadFile("../Resources/3DModels/Banana.fbx",
		//	aiProcess_Triangulate |
		//	aiProcess_ConvertToLeftHanded | aiProcess_JoinIdenticalVertices | aiProcess_PreTransformVertices | aiProcess_SortByPType |
		//	aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals | aiProcess_GenBoundingBoxes);
		//
		//assert(pScene != nullptr);
		



	}

	void AssetBasic3DLoader::Load3DModel(bool isSkinned, const std::string& path, Asset3DModelData* modelData)
	{
		AssetSceneData* tAssetSceneData = new Pg::Graphics::AssetSceneData;
		Assimp::Importer importer;
		//일단은 Mesh를 여러 개를 받아도 호환 가능하게 세팅!

		if (isSkinned)
		{
			//Skinned
			const aiScene* pScene = importer.ReadFile(path.c_str(),
				aiProcess_Triangulate |
				aiProcess_ConvertToLeftHanded | aiProcess_JoinIdenticalVertices | aiProcess_GenBoundingBoxes |
				aiProcess_CalcTangentSpace | aiProcess_PopulateArmatureData |
				aiProcess_GenSmoothNormals | aiProcess_SortByPType | aiProcess_EmbedTextures | aiProcess_LimitBoneWeights);
			assert(pScene != nullptr);

			tAssetSceneData->m_Directory = path;
			Helper::Asset3DModelHelper::CopyAssimpToAssetScene(pScene, tAssetSceneData);
			Helper::Asset3DModelHelper::FinalizeDataHelper();
			modelData->_assetSceneData = tAssetSceneData;

			//이 상황에서 AssetSceneData는 로딩된 것이다.
			//Material이 있을 시, 이를 로드한다.
			//CheckLoadMaterialTextures(pScene, modelData);
		}
		else
		{
			//Static
			const aiScene* pScene = importer.ReadFile(path.c_str(),
				aiProcess_Triangulate |
				aiProcess_ConvertToLeftHanded | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType |
				aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals | aiProcess_EmbedTextures | aiProcess_GenBoundingBoxes);
			assert(pScene != nullptr);

			tAssetSceneData->m_Directory = path;
			Helper::Asset3DModelHelper::CopyAssimpToAssetScene(pScene, tAssetSceneData);
			Helper::Asset3DModelHelper::FinalizeDataHelper();
			modelData->_assetSceneData = tAssetSceneData;

			//이 상황에서 AssetSceneData는 로딩된 것이다.
			//Material이 있을 시, 이를 로드한다.
			//CheckLoadMaterialTextures(pScene, modelData);
		}

	}

	bool AssetBasic3DLoader::IsModelSkinned(const std::string& path)
	{
		std::string tPath = Pg::Util::Helper::ResourceHelper::ForcePathUniform(path);

		Assimp::Importer tImporter;
		const aiScene* tScene = tImporter.ReadFile(path.c_str(),
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
		return tIsSkinned;
	}

	//void AssetBasic3DLoader::CheckLoadMaterialTextures(const aiScene* assimp, Asset3DModelData* modelData)
	//{
	//	if (!assimp->HasMaterials())
	//	{
	//		return;
	//	}
	//
	//	//Material이 있다는 얘기이다. 여기서 Material을 처리해야 한다.
	//	//for(assimp->)
	//
	//
	//}
	//
	//void AssetBasic3DLoader::LoadMaterialTextures(aiMaterial* mat, std::string typeName, const aiScene* scene)
	//{
	//
	//}

}