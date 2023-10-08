#pragma once
#include "../ParagonData/AssetDefines.h"

//<실제 Graphics Resource의 목록>
#include "RenderMaterial.h"
#include "RenderTexture2D.h"
#include "Asset3DModelData.h"
//</>

//Macro Function 내부 - 모두 여기에 있어야 한다.
#include <string>
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
	};

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
	struct AssetDefineType<Pg::Data::Enums::eAssetDefine::_2DTEXTURE>
	{
		using type = Pg::Graphics::RenderTexture2D; //2D Texture로 타입 변환.
	};

	template <>
	struct AssetDefineType<Pg::Data::Enums::eAssetDefine::_3DMODEL>
	{
		using type = Pg::Graphics::Asset3DModelData; //3D Model로 타입 변환.
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

//실행은 되지만, 통일성을 위해 쓰지 않는다.
//#define ASSETDEFINE2TYPE_MEMFUNC_2PARAMS(_enumAssetDefine, _obj, _memberFunc, _1stParam, _2ndParam) \
//	do { \
//		if (_enumAssetDefine == Pg::Core::Enums::eAssetDefine::_2DTEXTURE) { \
//			(_obj->_memberFunc<ASSETDEFINE_TYPE(Pg::Core::Enums::eAssetDefine::_2DTEXTURE)>)(_1stParam, _2ndParam); \
//		} \
//		else if (_enumAssetDefine == Pg::Core::Enums::eAssetDefine::_3DMODEL) { \
//			(_obj->_memberFunc<ASSETDEFINE_TYPE(Pg::Core::Enums::eAssetDefine::_3DMODEL)>)(_1stParam, _2ndParam); \
//		} \
//		else if (_enumAssetDefine == Pg::Core::Enums::eAssetDefine::_RENDERMATERIAL) { \
//			(_obj->_memberFunc<ASSETDEFINE_TYPE(Pg::Core::Enums::eAssetDefine::_RENDERMATERIAL)>)(_1stParam, _2ndParam); \
//		} \
//	} while (false)








