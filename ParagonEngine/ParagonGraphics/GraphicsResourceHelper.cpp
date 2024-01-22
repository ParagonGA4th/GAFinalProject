#include "GraphicsResourceHelper.h"

//RendererComponent들이 더 추가될 수록 여기도 변해야 한다.
#include "../ParagonData/BaseRenderer.h"
#include "../ParagonData/RendererBase2D.h"
#include "../ParagonData/RendererBase3D.h" 

#include "../ParagonData/ImageRenderer.h"
#include "../ParagonData/TextRenderer.h"
#include "../ParagonData/StaticMeshRenderer.h"
#include "../ParagonData/SkinnedMeshRenderer.h"

#include "../ParagonUtil/Log.h"

#include <cassert>

namespace Pg::Graphics::Helper
{
	void GraphicsResourceHelper::Initialize()
	{
		//템플릿 특수화 초기화의 역할을 할 것이다.
		AssetDefineType<Pg::Data::Enums::eAssetDefine::_TEXTURE1D>();
		AssetDefineType<Pg::Data::Enums::eAssetDefine::_TEXTURE2D>();
		AssetDefineType<Pg::Data::Enums::eAssetDefine::_TEXTURE2DARRAY>();
		AssetDefineType<Pg::Data::Enums::eAssetDefine::_TEXTURECUBE>();
		AssetDefineType<Pg::Data::Enums::eAssetDefine::_FONT>();
		AssetDefineType<Pg::Data::Enums::eAssetDefine::_3DMODEL>();
		AssetDefineType<Pg::Data::Enums::eAssetDefine::_CUBEMAP>();
		AssetDefineType<Pg::Data::Enums::eAssetDefine::_RENDER_VERTEXSHADER>();
		AssetDefineType<Pg::Data::Enums::eAssetDefine::_RENDER_PIXELSHADER>();
		AssetDefineType<Pg::Data::Enums::eAssetDefine::_RENDERMATERIAL>();
	}

	short GraphicsResourceHelper::IsRenderer3D(const std::string& rendererTypeName)
	{
		//안되는 경우를 걸러낸다. (Base 클래스들)
		if (rendererTypeName.compare(std::string(typeid(Pg::Data::BaseRenderer).name())) == 0 ||
			rendererTypeName.compare(std::string(typeid(Pg::Data::RendererBase2D).name())) == 0 ||
			rendererTypeName.compare(std::string(typeid(Pg::Data::RendererBase3D).name())) == 0)
		{
			PG_WARN("Wrong Renderer Type Name Entered!");
		}

		//원래는 더 깊숙히 들어가야 하지만, 렌더러 컴포넌트가 아직 나오지 않은 지금은 하드코딩.
		//이렇게 되어도 안된다! 더 자세한 렌더러 타입이 들어가야지.
		// 포인터가 들어와야 한다. typeid(자신).name(); 이렇게, 포인터가 들어오기 때문에.
		//2D 실제.
		if (rendererTypeName.compare(std::string(typeid(Pg::Data::ImageRenderer*).name())) == 0 ||
			rendererTypeName.compare(std::string(typeid(Pg::Data::TextRenderer*).name())) == 0)
		{
			return 0;
		}

		//3D 실제.
		if (rendererTypeName.compare(std::string(typeid(Pg::Data::StaticMeshRenderer*).name())) == 0 ||
			rendererTypeName.compare(std::string(typeid(Pg::Data::SkinnedMeshRenderer*).name())) == 0)
		{
			return 1;
		}

		//렌더러가 아닌 것.
		return -1;
	}

	eCbVarType GraphicsResourceHelper::GetCbVarType(const std::string& varString)
	{
		if (varString.compare("bool") == 0)
		{
			return _CB_BOOL;
		}
		else if (varString.compare("uint") == 0)
		{
			return _CB_UINT;
		}
		else if (varString.compare("int") == 0)
		{
			return _CB_INT;
		}
		else if (varString.compare("float") == 0)
		{
			return _CB_FLOAT;
		}
		else if (varString.compare("float2") == 0)
		{
			return _CB_FLOAT2;
		}
		else if (varString.compare("float3") == 0)
		{
			return _CB_FLOAT3;
		}
		else if (varString.compare("float4") == 0)
		{
			return _CB_FLOAT4;
		}
		else
		{
			//여기까지 오면 안된다.
			assert(false && "Invalid Type");
			//유효하지 않다.
			return _CB_BOOL;
		}
	}

	eTexVarType GraphicsResourceHelper::GetTexVarType(const std::string& varString)
	{
		if (varString.compare("Texture1D") == 0)
		{
			return _TEX_TEXTURE1D;
		}
		else if (varString.compare("Texture2D") == 0)
		{
			return _TEX_TEXTURE2D;
		}
		else if (varString.compare("Texture2DArray") == 0)
		{
			return _TEX_TEXTURE2DARRAY;
		}
		else if (varString.compare("TextureCube") == 0)
		{
			return _TEX_TEXTURECUBE;
		}
		else
		{
			//여기까지 오면 안된다.
			assert(false && "Invalid Type");
			//유효하지 않다.
			return _TEX_TEXTURE1D;
		}
	}

	eTexReturnVarType GraphicsResourceHelper::GetTexReturnVarType(const std::string& varString)
	{
		if (varString.compare("PG_RETURN_TYPE_UNORM") == 0)
		{
			return _TEXRET_UNORM;
		}
		else if (varString.compare("PG_RETURN_TYPE_SNORM") == 0)
		{
			return _TEXRET_SNORM;
		}
		else if (varString.compare("PG_RETURN_TYPE_SINT") == 0)
		{
			return _TEXRET_SINT;
		}
		else if (varString.compare("PG_RETURN_TYPE_UINT") == 0)
		{
			return _TEXRET_UINT;
		}
		else if (varString.compare("PG_RETURN_TYPE_FLOAT") == 0)
		{
			return _TEXRET_FLOAT;
		}
		else
		{
			//여기까지 오면 안된다.
			assert(false && "Invalid Type");
			//유효하지 않다.
			return _TEXRET_UNORM;
		}
	}

	Pg::Data::Enums::eAssetDefine GraphicsResourceHelper::GetAssetDefine(eTexVarType texVarType)
	{
		using Pg::Data::Enums::eAssetDefine;
		eAssetDefine tRet;

		switch (texVarType)
		{
			case Pg::Graphics::_TEX_TEXTURE1D:
			{
				tRet = eAssetDefine::_TEXTURE1D;
			}
			break;
			case Pg::Graphics::_TEX_TEXTURE2D:
			{
				tRet = eAssetDefine::_TEXTURE2D;
			}
			break;
			case Pg::Graphics::_TEX_TEXTURE2DARRAY:
			{
				tRet = eAssetDefine::_TEXTURE2DARRAY;
			}
			break;
			case Pg::Graphics::_TEX_TEXTURECUBE:
			{
				tRet = eAssetDefine::_TEXTURECUBE;
			}
			break;
		}

		return tRet;
	}

	std::string GraphicsResourceHelper::GetDefaultMaterialNameFromMeshName(const std::string& name)
	{
		std::string tStr = "DefaultMaterial_";
		tStr += name;
		return tStr;
	}

}