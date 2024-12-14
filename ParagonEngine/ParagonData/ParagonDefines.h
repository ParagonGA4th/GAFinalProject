#pragma once
#include <string>

///
/// ���� �������� ���̴� ��� ���ڵ��� �𿩼� �����س��´�.
///

namespace Pg::Defines
{
	constexpr unsigned short MAX_MATERIAL_PER_MODEL = 10;
	constexpr unsigned int ASSET_MAX_NUMBER_OF_COLOR_SETS = 0x8;
	constexpr unsigned int ASSET_MAX_NUMBER_OF_TEXTURECOORDS = 0x8;
	constexpr unsigned int ASSET_HINTMAXTEXTURELEN = 9;
	constexpr unsigned int ASSET_MAXIMUM_TEXTURE_PROP_CNT = 21;

	constexpr unsigned int ASSET_MAXIMUM_BONE_NUMBER_PER_MESH = 256;
	constexpr unsigned int ASSET_MAXIMUM_NODE_NUMBER_PER_MESH = 256;

	constexpr unsigned int MAXIMUM_OBJECT_COUNT_PER_INSTANCING = 1024;

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

	extern const std::string ASSET_DEFAULT_PRELOADED_LUT_PATH;

	// ^, $ ���ҽ� �̸��� Ȱ�� ����!
	//��� ����Ʈ ���͸������ �ο��޴� Prefix. �̴� �Ϲ� ���ҽ� �������� Ȱ���� �� ����.
	extern const std::string GENERATED_MATERIAL_PREFIX;

	//��� ����Ʈ ���͸��� ���� Ȱ��Ǵ� Texture2DArray�� �ο��޴� Prefix. �Ϲ� ���ҽ� �������� Ȱ���� �� ����.
	extern const std::string GENERATED_MATERIAL_TEXTURE2DARRAY_PREFIX;

	//��� �ν��Ͻ��� ������ �� �޴� Static 3DModel���� �ش� Prefix�� ���� �ִ�.
	extern const std::string NON_INSTANCED_3DMODEL_PREFIX;
	extern const std::string BLENDED_OPTIONAL_3DMODEL_PREFIX;
	extern const std::string CLIPPED_3DMODEL_PREFIX;

	//Shader Directories.
	extern const std::string DEFAULT_APPENDS_RENDER_VS_NAME;
	extern const std::string DEFAULT_APPENDS_RENDER_PS_NAME;

	extern const std::string DEFAULT_APPENDS_RENDER_VS_PATH;
	extern const std::string DEFAULT_APPENDS_RENDER_PS_PATH;

	extern const std::string DEFAULT_ALPHA_STATIC_VS_NAME;
	extern const std::string DEFAULT_ALPHA_SKINNED_VS_NAME;
	extern const std::string DEFAULT_ALPHA_PS_NAME;

	extern const std::string DEFAULT_ALPHA_STATIC_VS_PATH;
	extern const std::string DEFAULT_ALPHA_SKINNED_VS_PATH;
	extern const std::string DEFAULT_ALPHA_PS_PATH;

	extern const std::wstring FIRST_STATIC_VS_DIRECTORY;
	extern const std::wstring FIRST_SKINNED_VS_DIRECTORY;
	extern const std::wstring FIRST_INSTANCED_VS_DIRECTORY;
	extern const std::wstring FIRST_STAGE_ALPHABLENDED_PS;
	extern const std::wstring FIRST_STAGE_PS_DIRECTORY;
	extern const std::wstring FIRST_INSTANCED_STAGE_PS_DIRECTORY;
	extern const std::wstring FIRST_DEPTH_ONLY_STAGE_PS_DIRECTORY;
	extern const std::wstring FIRST_INSTANCED_DEPTH_ONLY_STAGE_PS_DIRECTORY;
	extern const std::wstring FIRST_ALPHA_CLIPPING_PS_DIRECTORY;
	extern const std::wstring FIRST_ALPHA_CLIPPING_INSTANCED_DEPTH_ONLY_STAGE_PS_DIRECTORY;
	extern const std::wstring FINAL_STAGE_VS_DIRECTORY;
	extern const std::wstring FINAL_STAGE_PS_DIRECTORY;

	extern const std::wstring SPRITE_VERTEX_SHADER_TEST_DIRECTORY;

	extern const std::wstring CUBEMAP_VS_DIRECTORY;
	extern const std::wstring CUBEMAP_PS_DIRECTORY;
	extern const std::wstring PRIMTIVE_VS_DIRECTORY;
	extern const std::wstring PRIMTIVE_PS_DIRECTORY;

	extern const std::wstring SHADOW_RECORD_DEPTH_VS_DIRECTORY;
	extern const std::wstring SHADOW_RECORD_DEPTH_PS_DIRECTORY;

	extern const std::wstring SELECTED_OUTLINE_VS_DIRECTORY;
	extern const std::wstring SELECTED_OUTLINE_BLUR_PS_DIRECTORY;
	extern const std::wstring SELECTED_OUTLINE_SINGLE_COLOR_PS_DIRECTORY;

	extern const std::wstring POSTPROCESSING_DEFAULT_VS_DIRECTORY;
	extern const std::wstring HEIGHTFOG_PS_DIRECTORY;
	extern const std::wstring VIGNETTE_PS_DIRECTORY;
	extern const std::wstring BLOOM_PS_DIRECTORY;
	extern const std::wstring LUT_PS_DIRECTORY;
	extern const std::wstring DEBUG_SHADOW_QUAD_PS_DIRECTORY;


}
