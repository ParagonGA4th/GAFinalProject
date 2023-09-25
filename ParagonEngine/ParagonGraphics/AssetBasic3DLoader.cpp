#include "AssetBasic3DLoader.h"

#include <assimp/Importer.hpp>     
#include <assimp/scene.h>          
#include <assimp/postprocess.h> 
#include <cassert>

namespace Pg::Graphics::Loader
{
	AssetBasic3DLoader::AssetBasic3DLoader()
	{
		//Assimp 링크 문제 없다는 것을 확인하기 위해.
		Assimp::Importer importer;
		const aiScene* pScene = importer.ReadFile("../Resources/3DModels/Banana.fbx",
			aiProcess_Triangulate |
			aiProcess_ConvertToLeftHanded | aiProcess_JoinIdenticalVertices | aiProcess_PreTransformVertices | aiProcess_SortByPType |
			aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals | aiProcess_GenBoundingBoxes);

		assert(pScene != nullptr);

	}

}