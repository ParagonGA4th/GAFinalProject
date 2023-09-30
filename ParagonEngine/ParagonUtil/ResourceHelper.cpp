#include "ResourceHelper.h"
#include <algorithm>
#include <filesystem>

namespace Pg::Util::Helper
{
	using Pg::Core::Enums::eResourceType;

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

	

}
