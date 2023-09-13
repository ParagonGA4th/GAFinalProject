#pragma once
#include "ResourceType.h"
#include <map>
#include <string>

/// <summary>
/// AssetManager를 도와주는 리소스 헬퍼, 리소스 관리에 도움을 준다.
/// </summary>

namespace Pg::Core::Helper
{
	class ResourceHelper
	{
	public:
		ResourceHelper();
		~ResourceHelper();

		//확장자 (Ex. .png) 를 리소스 타입으로 변환해주는 함수이다.
		static Pg::Core::Enum::eResourceType Ext2ResourceType(const std::string& extension);
	private:
		//확장자를 eResourceType으로 변환해준다.
		struct ExtensionSorter : public std::map<std::string, Pg::Core::Enum::eResourceType>
		{
			ExtensionSorter();
			~ExtensionSorter();
		};

	};
}


