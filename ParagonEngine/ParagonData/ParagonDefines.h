#pragma once
#include <string>

///
/// 엔진 전역에서 쓰이는 상수 숫자들을 모여서 선언해놓는다.
///

namespace Pg::Defines
{
	constexpr unsigned short MAX_MATERIAL_PER_MODEL = 10;
	constexpr unsigned int ASSET_MAX_NUMBER_OF_COLOR_SETS = 0x8;
	constexpr unsigned int ASSET_MAX_NUMBER_OF_TEXTURECOORDS = 0x8;
	constexpr unsigned int ASSET_HINTMAXTEXTURELEN = 9;
	constexpr unsigned int ASSET_MAXIMUM_TEXTURE_PROP_CNT = 21;

	extern const std::string ASSET_DEFAULT_DIFFUSE_TEXTURE_PATH;
	extern const std::string ASSET_DEFAULT_NORMAL_TEXTURE_PATH;
}
