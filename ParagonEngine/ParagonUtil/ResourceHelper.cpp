#include "ResourceHelper.h"
#include <algorithm>
#include <filesystem>
#include <cassert>

namespace Pg::Util::Helper
{
	using Pg::Data::Enums::eAssetDefine;

	ResourceHelper::ResourceHelper()
	{
		//
	}

	ResourceHelper::~ResourceHelper()
	{

	}

	bool ResourceHelper::IsFileExist(const std::string& filePath)
	{
		std::filesystem::path tPath(filePath);
		return std::filesystem::exists(tPath);
	}

	std::string ResourceHelper::ForcePathUniform(const std::string& filePath)
	{
		std::filesystem::path tPath(filePath);
		return tPath.generic_string();
	}

	std::string ResourceHelper::ForcePathUniformFull(const std::string& filePath)
	{
		std::filesystem::path tPath(filePath);

		if (tPath.is_absolute())
		{
			return tPath.generic_string();
		}

		//이제 상대적 경로.
		return std::filesystem::absolute(tPath).generic_string();
	}

	bool ResourceHelper::IsGraphicsResource(Pg::Data::Enums::eAssetDefine define)
	{
		bool tIsGraphicsResource = false;

		switch (define)
		{
		case eAssetDefine::_NONE:
		{
			assert(false);
		}
		break;
		//<Graphics>
		case eAssetDefine::_TEXTURE1D: [[fallthrough]];
		case eAssetDefine::_TEXTURE2D: [[fallthrough]];
		case eAssetDefine::_TEXTURE2DARRAY: [[fallthrough]];
		case eAssetDefine::_TEXTURECUBE: [[fallthrough]];
		case eAssetDefine::_CUBEMAP: [[fallthrough]];
		case eAssetDefine::_3DMODEL: [[fallthrough]];
		case eAssetDefine::_FONT: [[fallthrough]];
		case eAssetDefine::_RENDER_VERTEXSHADER: [[fallthrough]];
		case eAssetDefine::_RENDER_PIXELSHADER: [[fallthrough]];
		case eAssetDefine::_RENDERMATERIAL:
			tIsGraphicsResource = true;
			break;
			//</Graphics>
		default:
		{
			tIsGraphicsResource = false;
		}
		break;
		}

		return tIsGraphicsResource;
	}

	short ResourceHelper::IsPlainRenderable(Pg::Data::Enums::eAssetDefine define)
	{
		short tIsPlainRenderable = -1;

		switch (define)
		{
		case eAssetDefine::_NONE:
		{
			assert(false);
		}
		break;
		//<2D>
		case eAssetDefine::_TEXTURE2D: [[fallthrough]]; //TEXTURE2D 제외 리소스들 : 전부 자체적으로 렌더될 수는 없다.
		case eAssetDefine::_FONT:
		case eAssetDefine::_CUBEMAP: [[fallthrough]]; //일단은 "2D"로 분류. (DDS / WIC 범위에 들어가니)
			tIsPlainRenderable = 0;
			break;
			//</2D>
			//<3D>
		case eAssetDefine::_3DMODEL:
			tIsPlainRenderable = 1;
			break;
			//</3D>
		default:
		{
			//<NOT>
			tIsPlainRenderable = -1;
		}
		break;
		}

		return tIsPlainRenderable;
	}

	bool ResourceHelper::IsResourceDDS(const std::string& filePath)
	{
		std::filesystem::path tPath(filePath);
		std::string tExtString = tPath.extension().string();
		return (tExtString == ".dds" || tExtString == ".DDS");
	}

	bool ResourceHelper::IsResourceDDS(const std::wstring& filePath)
	{
		std::filesystem::path tPath(filePath);
		std::string tExtString = tPath.extension().string();
		return (tExtString == ".dds" || tExtString == ".DDS");
	}

	std::string ResourceHelper::GetNameFromPath(const std::string& filePath)
	{
		std::filesystem::path tPath(filePath);
		return tPath.filename().string();
	}
	
	

}
