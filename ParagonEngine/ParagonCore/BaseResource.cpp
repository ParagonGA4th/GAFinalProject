#include "BaseResource.h"
#include "../ParagonUtil/ResourceHelper.h"
#include <filesystem>

#ifdef _DEBUG
#pragma comment(lib,"..\\Builds\\x64\\Debug\\ParagonUtil.lib")
#else
#pragma comment(lib,"..\\Builds\\x64\\Release\\ParagonUtil.lib")
#endif // _DEBUG

namespace Pg::Core::Resources
{
	using Pg::Core::Enums::eResourceType;
	using Pg::Util::Helper::ResourceHelper;

	BaseResource::BaseResource(const std::string& filePath) : _filePath(filePath)
	{
		std::filesystem::path tPath(filePath);
		std::string tExt = tPath.extension().string();
		this->_resourceType = ResourceHelper::Ext2ResourceType(tExt);

		//자식은 이제 실제로 _resourceType을 활용하던, 자기 자신 내부의 정부를 Load할 수 있어야 한다.
	}

	BaseResource::~BaseResource()
	{

	}

}
