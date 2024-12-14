#include "BaseResource.h"
#include "AssetDefines.h"
#include "../ParagonHelper/ResourceHelper.h"

#include <filesystem>

namespace Pg::Data::Resources
{
	using Pg::Util::Helper::ResourceHelper;

	BaseResource::BaseResource(Pg::Data::Enums::eAssetDefine define, const std::string& typeInfoName, const std::string& filePath) :
		_filePath(filePath), _typeInfoName(typeInfoName), _assetDefine(define)
	{
		//�ڽ��� ���� ������ _resourceType�� Ȱ���ϴ�, �ڱ� �ڽ� ������ ���θ� Load�� �� �־�� �Ѵ�.
	}

	BaseResource::~BaseResource()
	{

	}

	std::string BaseResource::GetFilePath()
	{
		return _filePath;
	}

	Pg::Data::Enums::eAssetDefine BaseResource::GetAssetDefine()
	{
		return _assetDefine;
	}

	std::string BaseResource::GetFileName()
	{
		std::filesystem::path tPath = _filePath;
		return tPath.filename().string();
	}

}
