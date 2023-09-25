#include "AssetBasic3DLoader.h"
#include "../ParagonUtil/ResourceHelper.h"

#include <assimp/Importer.hpp>     
#include <assimp/scene.h>          
#include <assimp/postprocess.h> 
#include <cassert>


#ifdef _DEBUG
#pragma comment(lib,"..\\x64\\Debug\\ParagonUtil.lib")
#else
#pragma comment(lib,"..\\x64\\Release\\ParagonUtil.lib")
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

	void AssetBasic3DLoader::Load3DModel(const std::string& path)
	{
		if (IsModelSkinned(path))
		{
			//Skinned

		}
		else
		{
			//Static

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

	

}