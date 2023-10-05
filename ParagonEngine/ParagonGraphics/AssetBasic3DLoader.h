#pragma once
#include <string>

/// <summary>
/// 3D 모델 데이터를 로드해서 저장하는 클래스.
/// 3D 모델 데이터를 가져오면서, Skinned/Static 여부를 구분할 수 있게 로드한다.
/// 전부 하나의 인스턴스를 가지고 작동되어야 한다.
/// </summary>

namespace Pg::Graphics
{
	class AssetSceneData;
}

namespace Pg::Graphics::Loader
{
	class AssetBasic3DLoader
	{
	public:
		AssetBasic3DLoader();

		//모델이 Skinned되어 있는지 반환.
		bool IsModelSkinned(const std::string& path);

		//3DModel을 실제로 로드한다.
		Pg::Graphics::AssetSceneData* Load3DModel(bool isSkinned, const std::string& path);
		
	private:
		
	};
}


