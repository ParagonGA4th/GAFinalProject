#pragma once
#include <string>

/// <summary>
/// 3D 모델 데이터를 로드해서 저장하는 클래스.
/// 3D 모델 데이터를 가져오면서, Skinned/Static 여부를 구분할 수 있게 로드한다.
/// 전부 하나의 인스턴스를 가지고 작동되어야 한다.
/// </summary>

namespace Pg::Graphics::Loader
{
	class AssetBasic3DLoader
	{
	public:
		AssetBasic3DLoader();

		void Load3DModel(const std::string& path);

	private:
		bool IsModelSkinned(const std::string& path);
	};
}


