#pragma once
#include "../ParagonData/AssetDefines.h"
#include "../ParagonData/ParagonDefines.h"

#include "ShaderParsingData.h"
#include "AssetTextureType.h"
#include "SizeTexture.h"
#include "TextureExtension.h"

//<���� Graphics Resource�� ���>
#include "RenderMaterial.h"
#include "RenderVertexShader.h"
#include "RenderPixelShader.h"
#include "RenderTexture1D.h"
#include "RenderTexture2D.h"
#include "RenderTexture2DArray.h"
#include "RenderTextureCube.h"
#include "RenderFont.h"
#include "RenderCubemap.h"
#include "Asset3DModelData.h"
#include "RenderAnimation.h"
//</>

//Macro Function ���� - ��� ���⿡ �־�� �Ѵ�.
#include <string>
#include <vector>
#include <cassert>
#include <cstddef>

/// <summary>
/// �׷��� �������� ���ҽ� ������ �־� ������ �ִ� ���� Ŭ����.
/// Enum�� Ÿ������ �ٲ��ִ� ����� �����Ѵ�.
/// ���ҽ� ���� ����� �߰��Ǹ鼭 ���������� �ڵ� �����ؾ� �Ѵ�! (��ũ�� & Template)
/// </summary>

// GraphicResourcesHelper
namespace Pg::Graphics::Helper
{
	using Pg::Data::Enums::eAssetDefine;

	//GraphicsResourceHelper �� ��ü.
	class GraphicsResourceHelper
	{
	public:
		//���ø� Ư��ȭ �ʱ�ȭ.
		static void Initialize(); 

		//���� RendererTypeName�� 
		//�������� �ƴ��� <-1> / 2D ���������� <0> / 3D ���������� <1>.
		//Renderer ������Ʈ�� �߰��� ���� ������Ʈ�Ǿ�� �Ѵ�.
		static short IsRenderer3D(const std::string& rendererTypeName);

		//Texture�� ������ ���� �⺻ �ؽ��� ��θ� ��ȯ�Ѵ�.
		static std::string GetDefaultTexturePath(eAssetTextureType textureType, eSizeTexture sizeType = eSizeTexture::_512x512);

		//�׷��Ƚ� : Material Loading!
		static eTexVarType GetTexVarType(const std::string& varString);
		static eTexReturnVarType GetTexReturnVarType(const std::string& varString);
		static eCbVarType GetCbVarType(const std::string& varString);
		static eAssetDefine GetAssetDefine(eTexVarType texVarType);

		//Default Material ���� ����.
		static std::string GetDefaultMaterialNameFromMeshName(const std::string& name);
		static std::string GetMeshNameFromDefaultMaterialName(const std::string& name);

		//Default Material�� ���� Default Texture2DArray ������ ����.
		static std::string GetGeneratedTex2DArrayNameFromValues(const std::string& defMatName, const std::string& varName, std::string* renderTextureNameSrc, unsigned int cnt);
		static void GetTextureNamesFromDefaultTex2DArrayName(const std::string& defTex2DArrName, std::vector<std::string>& outStringVector);
		
		//Texture2DArray�� ���� Ȯ���� .pgt2arr�� ���� String�� Vector�� �ٲپ� ��������.
		static void ReadPGT2ARRContents(const std::string& pgt2arrContent, std::vector<std::string>& outStringVector);

		static bool IsMaterialDefaultMaterial(RenderMaterial* renderMat);

		static eSizeTexture GetSizeTextureFromUINT(unsigned int width, unsigned int height);

		static eTextureExtension GetTexExtFromPath(const std::string& path);
	};
}

namespace Pg::Graphics::Helper
{
	template <Pg::Data::Enums::eAssetDefine define>
	struct AssetDefineType;

	//eAssetDefine�� �������� �ٸ� ������ ���ҽ��� �����. (���ҽ��� ������ Ȯ��ɼ��� �� ���ǹ� ���� Ȯ��ȴ�)
	//���� Ŭ���� ��ȯ.
	template <>
	struct AssetDefineType<Pg::Data::Enums::eAssetDefine::_NONE>
	{
		using type = Pg::Data::Resources::GraphicsResource; //2D Texture�� Ÿ�� ��ȯ.
	};

	template <>
	struct AssetDefineType<Pg::Data::Enums::eAssetDefine::_TEXTURE1D>
	{
		using type = Pg::Graphics::RenderTexture1D; //2D Texture�� Ÿ�� ��ȯ.
	};

	template <>
	struct AssetDefineType<Pg::Data::Enums::eAssetDefine::_TEXTURE2D>
	{
		using type = Pg::Graphics::RenderTexture2D; //2D Texture�� Ÿ�� ��ȯ.
	};

	template <>
	struct AssetDefineType<Pg::Data::Enums::eAssetDefine::_TEXTURE2DARRAY>
	{
		using type = Pg::Graphics::RenderTexture2DArray; //2D Texture�� Ÿ�� ��ȯ.
	};

	template <>
	struct AssetDefineType<Pg::Data::Enums::eAssetDefine::_TEXTURECUBE>
	{
		using type = Pg::Graphics::RenderTextureCube; //2D Texture�� Ÿ�� ��ȯ.
	};

	template <>
	struct AssetDefineType<Pg::Data::Enums::eAssetDefine::_FONT>
	{
		using type = Pg::Graphics::RenderFont; //RenderMaterial�� Ÿ�� ��ȯ.
	};

	template <>
	struct AssetDefineType<Pg::Data::Enums::eAssetDefine::_CUBEMAP>
	{
		using type = Pg::Graphics::RenderCubemap; //RenderMaterial�� Ÿ�� ��ȯ.
	};

	template <>
	struct AssetDefineType<Pg::Data::Enums::eAssetDefine::_3DMODEL>
	{
		using type = Pg::Graphics::Asset3DModelData; //3D Model�� Ÿ�� ��ȯ.
	};

	template <>
	struct AssetDefineType<Pg::Data::Enums::eAssetDefine::_RENDER_VERTEXSHADER>
	{
		using type = Pg::Graphics::RenderVertexShader; //RenderMaterial�� Ÿ�� ��ȯ.
	};

	template <>
	struct AssetDefineType<Pg::Data::Enums::eAssetDefine::_RENDER_PIXELSHADER>
	{
		using type = Pg::Graphics::RenderPixelShader; //RenderMaterial�� Ÿ�� ��ȯ.
	};

	template <>
	struct AssetDefineType<Pg::Data::Enums::eAssetDefine::_RENDERMATERIAL>
	{
		using type = Pg::Graphics::RenderMaterial; //RenderMaterial�� Ÿ�� ��ȯ.
	};

	template <>
	struct AssetDefineType<Pg::Data::Enums::eAssetDefine::_ANIMATION>
	{
		using type = Pg::Graphics::RenderAnimation; //RenderMaterial�� Ÿ�� ��ȯ.
	};
}

//Macro Function Helpers.

// -> AssetDefine ȣȯ�� ����!
#define ASSETDEFINE_TYPE(ASSETDEFINE) Pg::Graphics::Helper::AssetDefineType<ASSETDEFINE>::type







