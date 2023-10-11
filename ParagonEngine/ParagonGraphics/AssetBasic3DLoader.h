#pragma once
#include <string>
#include "TextureCluster.h"
/// <summary>
/// 3D 모델 데이터를 로드해서 저장하는 클래스.
/// 3D 모델 데이터를 가져오면서, Skinned/Static 여부를 구분할 수 있게 로드한다.
/// 전부 하나의 인스턴스를 가지고 작동되어야 한다.
/// </summary>

//class aiScene;
//class aiMaterial;

namespace Pg::Graphics
{
	class AssetSceneData;
	class Asset3DModelData;
}

namespace Pg::Graphics::Loader
{
	class AssetBasic3DLoader
	{
	public:
		AssetBasic3DLoader();

		//모델이 Skinned되어 있는지 반환.
		bool IsModelSkinned(const std::string& path);

		//3DModel을 실제로 로드해서 Data에 넣어준다.
		void Load3DModel(bool isSkinned, const std::string& path, Asset3DModelData* modelData);
		
	private:
		// -> 이거 Assimp->Asset로 바뀌어야 한다.
		//// Material들이 텍스쳐 안에 임베딩되어 있을 경우, 이를 빼낸다.
		//void CheckLoadMaterialTextures(const aiScene* assimp, Asset3DModelData* modelData);
		//
		////실제로 Material 내부에 위치한 텍스쳐를 가져온다. 테크닉이 추가되면서, 더 많은 텍스쳐들이 연동될 것.
		//void LoadMaterialTextures(aiMaterial* mat, std::string typeName, const aiScene* scene);
	
		
	
	};
}


