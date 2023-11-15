#pragma once
#include <string>
#include "MaterialCluster.h"
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

		//3DModel을 실제로 로드해서 Data에 넣어주고, 렌더에 필요한 설정들을 세팅해준다.
		void Load3DModel(const std::string& path, Asset3DModelData* modelData);
		
	private:
	
	};
}


