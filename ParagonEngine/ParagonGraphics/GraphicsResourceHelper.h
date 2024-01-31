#pragma once
#include "../ParagonData/AssetDefines.h"

#include "ShaderParsingData.h"
#include "AssetTextureType.h"
#include "SizeTexture.h"

//<실제 Graphics Resource의 목록>
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
//</>

//Macro Function 내부 - 모두 여기에 있어야 한다.
#include <string>
#include <vector>
#include <cassert>
#include <cstddef>

/// <summary>
/// 그래픽 엔진에서 리소스 관리에 있어 도움을 주는 헬퍼 클래스.
/// Enum을 타입으로 바꿔주는 기능을 수행한다.
/// 리소스 관련 기능이 추가되면서 지속적으로 코드 갱신해야 한다! (매크로 & Template)
/// </summary>

// GraphicResourcesHelper
namespace Pg::Graphics::Helper
{
	using Pg::Data::Enums::eAssetDefine;

	//GraphicsResourceHelper 그 자체.
	class GraphicsResourceHelper
	{
	public:
		//템플릿 특수화 초기화.
		static void Initialize(); 

		//받은 RendererTypeName이 
		//렌더러가 아닌지 <-1> / 2D 렌더러인지 <0> / 3D 렌더러인지 <1>.
		//Renderer 컴포넌트가 추가될 수록 업데이트되어야 한다.
		static short IsRenderer3D(const std::string& rendererTypeName);

		//Texture의 종류에 따라 기본 텍스쳐 경로를 반환한다.
		static std::string GetDefaultTexturePath(eAssetTextureType textureType, eSizeTexture sizeType = eSizeTexture::_512x512);

		//그래픽스 : Material Loading!
		static eTexVarType GetTexVarType(const std::string& varString);
		static eTexReturnVarType GetTexReturnVarType(const std::string& varString);
		static eCbVarType GetCbVarType(const std::string& varString);
		static eAssetDefine GetAssetDefine(eTexVarType texVarType);

		//Default Material 연동 위해.
		static std::string GetDefaultMaterialNameFromMeshName(const std::string& name);
		static std::string GetMeshNameFromDefaultMaterialName(const std::string& name);

		//Default Material을 위한 Default Texture2DArray 연동을 위해.
		static std::string GetGeneratedTex2DArrayNameFromValues(const std::string& defMatName, const std::string& varName, std::string* renderTextureNameSrc, unsigned int cnt);
		static void GetTextureNamesFromDefaultTex2DArrayName(const std::string& defTex2DArrName, std::vector<std::string>& outStringVector);
		
		//Texture2DArray를 위한 확장자 .pgt2arr의 내용 String을 Vector로 바꾸어 내보낸다.
		static void ReadPGT2ARRContents(const std::string& pgt2arrContent, std::vector<std::string>& outStringVector);

		static eSizeTexture GetSizeTextureFromUINT(unsigned int width, unsigned int height);
	public:
		// ^, $ 리소스 이름에 활용 금지!
		
		//모든 디폴트 매터리얼들이 부여받는 Prefix. 이는 일반 리소스 생성에서 활용할 수 없다.
		static const std::string GENERATED_MATERIAL_PREFIX;

		//모든 디폴트 매터리얼 내부 활용되는 Texture2DArray가 부여받는 Prefix. 일반 리소스 생성에서 활용할 수 없다.
		static const std::string GENERATED_MATERIAL_TEXTURE2DARRAY_PREFIX;
	};
}

namespace Pg::Graphics::Helper
{
	template <Pg::Data::Enums::eAssetDefine define>
	struct AssetDefineType;

	//eAssetDefine을 기준으로 다른 형태의 리소스를 만든다. (리소스의 개수가 확대될수록 이 조건문 역시 확대된다)
	//세부 클래스 변환.
	template <>
	struct AssetDefineType<Pg::Data::Enums::eAssetDefine::_NONE>
	{
		using type = Pg::Data::Resources::GraphicsResource; //2D Texture로 타입 변환.
	};

	template <>
	struct AssetDefineType<Pg::Data::Enums::eAssetDefine::_TEXTURE1D>
	{
		using type = Pg::Graphics::RenderTexture1D; //2D Texture로 타입 변환.
	};

	template <>
	struct AssetDefineType<Pg::Data::Enums::eAssetDefine::_TEXTURE2D>
	{
		using type = Pg::Graphics::RenderTexture2D; //2D Texture로 타입 변환.
	};

	template <>
	struct AssetDefineType<Pg::Data::Enums::eAssetDefine::_TEXTURE2DARRAY>
	{
		using type = Pg::Graphics::RenderTexture2DArray; //2D Texture로 타입 변환.
	};

	template <>
	struct AssetDefineType<Pg::Data::Enums::eAssetDefine::_TEXTURECUBE>
	{
		using type = Pg::Graphics::RenderTextureCube; //2D Texture로 타입 변환.
	};

	template <>
	struct AssetDefineType<Pg::Data::Enums::eAssetDefine::_FONT>
	{
		using type = Pg::Graphics::RenderFont; //RenderMaterial로 타입 변환.
	};

	template <>
	struct AssetDefineType<Pg::Data::Enums::eAssetDefine::_CUBEMAP>
	{
		using type = Pg::Graphics::RenderCubemap; //RenderMaterial로 타입 변환.
	};

	template <>
	struct AssetDefineType<Pg::Data::Enums::eAssetDefine::_3DMODEL>
	{
		using type = Pg::Graphics::Asset3DModelData; //3D Model로 타입 변환.
	};

	template <>
	struct AssetDefineType<Pg::Data::Enums::eAssetDefine::_RENDER_VERTEXSHADER>
	{
		using type = Pg::Graphics::RenderVertexShader; //RenderMaterial로 타입 변환.
	};

	template <>
	struct AssetDefineType<Pg::Data::Enums::eAssetDefine::_RENDER_PIXELSHADER>
	{
		using type = Pg::Graphics::RenderPixelShader; //RenderMaterial로 타입 변환.
	};

	template <>
	struct AssetDefineType<Pg::Data::Enums::eAssetDefine::_RENDERMATERIAL>
	{
		using type = Pg::Graphics::RenderMaterial; //RenderMaterial로 타입 변환.
	};
}

//Macro Function Helpers.

// -> AssetDefine 호환을 위해!
#define ASSETDEFINE_TYPE(ASSETDEFINE) Pg::Graphics::Helper::AssetDefineType<ASSETDEFINE>::type







