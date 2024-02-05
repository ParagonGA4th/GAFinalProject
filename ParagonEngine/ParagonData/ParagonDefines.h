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

	constexpr unsigned int ASSET_MAXIMUM_BONE_NUMBER_PER_MESH = 100;

	extern const std::string ASSET_DEFAULT_DIFFUSE_TEXTURE_PATH_256;
	extern const std::string ASSET_DEFAULT_NORMAL_TEXTURE_PATH_256;
	extern const std::string ASSET_DEFAULT_ARM_TEXTURE_PATH_256;
	extern const std::string ASSET_DEFAULT_SPECULAR_TEXTURE_PATH_256;
	//extern const std::string ASSET_DEFAULT_DISPLACEMENT_TEXTURE_PATH_256;

	extern const std::string ASSET_DEFAULT_DIFFUSE_TEXTURE_PATH_512;
	extern const std::string ASSET_DEFAULT_NORMAL_TEXTURE_PATH_512;
	extern const std::string ASSET_DEFAULT_ARM_TEXTURE_PATH_512;
	extern const std::string ASSET_DEFAULT_SPECULAR_TEXTURE_PATH_512;
	//extern const std::string ASSET_DEFAULT_DISPLACEMENT_TEXTURE_PATH_512;

	extern const std::string ASSET_DEFAULT_DIFFUSE_TEXTURE_PATH_1024;
	extern const std::string ASSET_DEFAULT_NORMAL_TEXTURE_PATH_1024;
	extern const std::string ASSET_DEFAULT_ARM_TEXTURE_PATH_1024;
	extern const std::string ASSET_DEFAULT_SPECULAR_TEXTURE_PATH_1024;
	//extern const std::string ASSET_DEFAULT_DISPLACEMENT_TEXTURE_PATH_1024;

	extern const std::string ASSET_DEFAULT_DIFFUSE_TEXTURE_PATH_2048;
	extern const std::string ASSET_DEFAULT_NORMAL_TEXTURE_PATH_2048;
	extern const std::string ASSET_DEFAULT_ARM_TEXTURE_PATH_2048;
	extern const std::string ASSET_DEFAULT_SPECULAR_TEXTURE_PATH_2048;
	//extern const std::string ASSET_DEFAULT_DISPLACEMENT_TEXTURE_PATH_2048;
	
	extern const std::string ASSET_DEFAULT_IBL_DIFFUSE_IRRADIANCE_CUBEMAP_PATH;
	extern const std::string ASSET_DEFAULT_IBL_SPECULAR_IRRADIANCE_CUBEMAP_PATH;
	extern const std::string ASSET_DEFAULT_IBL_SPECULAR_BRDF_LUT_TEXTURE_PATH;
	extern const std::string ASSET_DEFAULT_ENVIRONMENT_CUBEMAP_PATH;

	extern const std::string DEFAULT_APPENDS_RENDER_VS_NAME;
	extern const std::string DEFAULT_APPENDS_RENDER_PS_NAME;

	extern const std::string DEFAULT_APPENDS_RENDER_VS_PATH;
	extern const std::string DEFAULT_APPENDS_RENDER_PS_PATH;
}
