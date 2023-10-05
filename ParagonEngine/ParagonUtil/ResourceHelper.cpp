#include "ResourceHelper.h"
#include <algorithm>
#include <filesystem>
#include <cassert>

namespace Pg::Util::Helper
{
	using Pg::Core::Enums::eAssetDefine;

	ResourceHelper::ResourceHelper()
	{
		//
	}

	ResourceHelper::~ResourceHelper()
	{

	}

	std::string ResourceHelper::ForcePathUniform(const std::string& filePath)
	{
		std::filesystem::path tPath(filePath);
		return tPath.generic_string();
	}

	bool ResourceHelper::IsGraphicsResource(Pg::Core::Enums::eAssetDefine define)
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
		case eAssetDefine::_2DTEXTURE: [[fallthrough]];
		case eAssetDefine::_CUBEMAP: [[fallthrough]];
		case eAssetDefine::_3DMODEL: [[fallthrough]];
		case eAssetDefine::_FONT: [[fallthrough]];
		case eAssetDefine::_RENDERSHADER: [[fallthrough]];
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

	short ResourceHelper::IsPlainRenderable(Pg::Core::Enums::eAssetDefine define)
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
		case eAssetDefine::_2DTEXTURE: [[fallthrough]];
		case eAssetDefine::_FONT:
			tIsPlainRenderable = 0;
			break;
			//</2D>
			//<3D>
		case eAssetDefine::_CUBEMAP: [[fallthrough]];
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

}
