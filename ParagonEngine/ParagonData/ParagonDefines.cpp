#include "ParagonDefines.h"

namespace Pg::Defines
{
	//String 등 Const를 바로 쓸 수 없는 디파인들을 위해.
	const std::string ASSET_DEFAULT_DIFFUSE_TEXTURE_PATH_256 = "../Resources/Textures/Default/256x256/Paragon_DefaultDiffuseMap256x256.dds";
	const std::string ASSET_DEFAULT_NORMAL_TEXTURE_PATH_256 = "../Resources/Textures/Default/256x256/Paragon_DefaultNormalMap256x256.dds";
	const std::string ASSET_DEFAULT_ARM_TEXTURE_PATH_256 = "../Resources/Textures/Default/256x256/Paragon_DefaultArmMap256x256.dds";
	const std::string ASSET_DEFAULT_SPECULAR_TEXTURE_PATH_256 = "../Resources/Textures/Default/256x256/Paragon_DefaultSpecularMap256x256.dds";
	//const std::string ASSET_DEFAULT_DISPLACEMENT_TEXTURE_PATH_256 = "../Resources/Textures/Default/256x256/Paragon_DefaultDisplacementMap256x256.dds";

	const std::string ASSET_DEFAULT_DIFFUSE_TEXTURE_PATH_512 = "../Resources/Textures/Default/512x512/Paragon_DefaultDiffuseMap512x512.dds";
	const std::string ASSET_DEFAULT_NORMAL_TEXTURE_PATH_512 = "../Resources/Textures/Default/512x512/Paragon_DefaultNormalMap512x512.dds";
	const std::string ASSET_DEFAULT_ARM_TEXTURE_PATH_512 = "../Resources/Textures/Default/512x512/Paragon_DefaultArmMap512x512.dds";
	const std::string ASSET_DEFAULT_SPECULAR_TEXTURE_PATH_512 = "../Resources/Textures/Default/512x512/Paragon_DefaultSpecularMap512x512.dds";
	//const std::string ASSET_DEFAULT_DISPLACEMENT_TEXTURE_PATH_512 = "../Resources/Textures/Default/512x512/Paragon_DefaultDisplacementMap512x512.dds";

	const std::string ASSET_DEFAULT_DIFFUSE_TEXTURE_PATH_1024 = "../Resources/Textures/Default/1024x1024/Paragon_DefaultDiffuseMap1024x1024.dds";
	const std::string ASSET_DEFAULT_NORMAL_TEXTURE_PATH_1024 = "../Resources/Textures/Default/1024x1024/Paragon_DefaultNormalMap1024x1024.dds";
	const std::string ASSET_DEFAULT_ARM_TEXTURE_PATH_1024 = "../Resources/Textures/Default/1024x1024/Paragon_DefaultArmMap1024x1024.dds";
	const std::string ASSET_DEFAULT_SPECULAR_TEXTURE_PATH_1024 = "../Resources/Textures/Default/1024x1024/Paragon_DefaultSpecularMap1024x1024.dds";
	//const std::string ASSET_DEFAULT_DISPLACEMENT_TEXTURE_PATH_1024 = "../Resources/Textures/Default/1024x1024/Paragon_DefaultDisplacementMap1024x1024.dds";

	const std::string ASSET_DEFAULT_DIFFUSE_TEXTURE_PATH_2048 = "../Resources/Textures/Default/2048x2048/Paragon_DefaultDiffuseMap2048x2048.dds";
	const std::string ASSET_DEFAULT_NORMAL_TEXTURE_PATH_2048 = "../Resources/Textures/Default/2048x2048/Paragon_DefaultNormalMap2048x2048.dds";
	const std::string ASSET_DEFAULT_ARM_TEXTURE_PATH_2048 = "../Resources/Textures/Default/2048x2048/Paragon_DefaultArmMap2048x2048.dds";
	const std::string ASSET_DEFAULT_SPECULAR_TEXTURE_PATH_2048 = "../Resources/Textures/Default/2048x2048/Paragon_DefaultSpecularMap2048x2048.dds";
	//const std::string ASSET_DEFAULT_DISPLACEMENT_TEXTURE_PATH_2048 = "../Resources/Textures/Default/2048x2048/Paragon_DefaultDisplacementMap2048x2048.dds";


	//const std::string ASSET_DEFAULT_IBL_DIFFUSE_IRRADIANCE_CUBEMAP_PATH = "../Resources/Textures/Cubemaps/IBL/TweakedSky/TweakedSkyDiffuseHDR.dds";
	//const std::string ASSET_DEFAULT_IBL_SPECULAR_IRRADIANCE_CUBEMAP_PATH = "../Resources/Textures/Cubemaps/IBL/TweakedSky/TweakedSkySpecularHDR.dds";
	//const std::string ASSET_DEFAULT_IBL_SPECULAR_BRDF_LUT_TEXTURE_PATH = "../Resources/Textures/Cubemaps/IBL/TweakedSky/TweakedSkyBrdf.dds";

	const std::string ASSET_DEFAULT_IBL_DIFFUSE_IRRADIANCE_CUBEMAP_PATH = "../Resources/Textures/Cubemaps/IBL/KloppenheimSky/KloppenheimSky_IBLMapDiffuseHDR.dds";
	const std::string ASSET_DEFAULT_IBL_SPECULAR_IRRADIANCE_CUBEMAP_PATH = "../Resources/Textures/Cubemaps/IBL/KloppenheimSky/KloppenheimSky_IBLMapSpecularHDR.dds";
	const std::string ASSET_DEFAULT_IBL_SPECULAR_BRDF_LUT_TEXTURE_PATH = "../Resources/Textures/Cubemaps/IBL/KloppenheimSky/KloppenheimSky_IBLMapBrdf.dds";

	//const std::string ASSET_DEFAULT_ENVIRONMENT_CUBEMAP_PATH = "../Resources/Textures/room.dds";
	const std::string ASSET_DEFAULT_ENVIRONMENT_CUBEMAP_PATH = "../Resources/Textures/Cubemaps/IBL/KloppenheimSky/KloppenheimSky_IBLMapDiffuseMDR.dds";

	const std::string GENERATED_MATERIAL_PREFIX = "$GeneratedMaterial_$";
	const std::string GENERATED_MATERIAL_TEXTURE2DARRAY_PREFIX = "$GeneratedMaterial_Texture2DArray_$";

	const std::string NON_INSTANCED_3DMODEL_PREFIX = "norm_";

	//셰이더 이름들.
	const std::string DEFAULT_APPENDS_RENDER_VS_NAME = "AppendObjectDefaultVS.cso";
	const std::string DEFAULT_APPENDS_RENDER_PS_NAME = "AppendObjectDefaultPS.cso";

	const std::string DEFAULT_APPENDS_RENDER_VS_PATH = "../Builds/x64/Debug/AppendObjectDefaultVS.cso";
	const std::string DEFAULT_APPENDS_RENDER_PS_PATH = "../Builds/x64/Debug/AppendObjectDefaultPS.cso";

	const std::wstring FIRST_STATIC_VS_DIRECTORY = L"../Builds/x64/Debug/FirstStatic_VS.cso";
	const std::wstring FIRST_SKINNED_VS_DIRECTORY = L"../Builds/x64/Debug/FirstSkinned_VS.cso";
	const std::wstring FIRST_INSTANCED_VS_DIRECTORY = L"../Builds/x64/Debug/FirstInstanced_VS.cso";
	const std::wstring FIRST_STAGE_ALPHABLENDED_PS = L"../Builds/x64/Debug/FirstStage_AlphaBlendedPS.cso";
	const std::wstring FIRST_STAGE_PS_DIRECTORY = L"../Builds/x64/Debug/FirstStage_PS.cso";
	const std::wstring FIRST_INSTANCED_STAGE_PS_DIRECTORY = L"../Builds/x64/Debug/FirstInstancedStage_PS.cso";
	const std::wstring FINAL_STAGE_VS_DIRECTORY = L"../Builds/x64/Debug/FinalStage_VS.cso";
	const std::wstring FINAL_STAGE_PS_DIRECTORY = L"../Builds/x64/Debug/FinalStage_PS.cso";

	const std::wstring CUBEMAP_VS_DIRECTORY = L"../Builds/x64/Debug/CubemapVS.cso";
	const std::wstring CUBEMAP_PS_DIRECTORY = L"../Builds/x64/Debug/CubemapPS.cso";
	const std::wstring PRIMTIVE_VS_DIRECTORY = L"../Builds/x64/Debug/PrimitiveVS.cso";
	const std::wstring PRIMTIVE_PS_DIRECTORY = L"../Builds/x64/Debug/PrimitivePS.cso";

	const std::wstring SELECTED_OUTLINE_VS_DIRECTORY = L"../Builds/x64/Debug/SelectedOutline_VS.cso";
	const std::wstring SELECTED_OUTLINE_BLUR_PS_DIRECTORY = L"../Builds/x64/Debug/SelectedOutline_Blur_PS.cso";
	const std::wstring SELECTED_OUTLINE_SINGLE_COLOR_PS_DIRECTORY = L"../Builds/x64/Debug/SelectedOutline_SingleColor_PS.cso";

	const std::wstring POSTPROCESSING_DEFAULT_VS_DIRECTORY = L"../Builds/x64/Debug/PostProcessingDefault_VS.cso";
	const std::wstring TONEMAPPING_PS_DIRECTORY = L"../Builds/x64/Debug/ToneMapping_PS.cso";
	const std::wstring VIGNETTE_PS_DIRECTORY = L"../Builds/x64/Debug/Vignette_PS.cso";


}


