#pragma once
#include "../ParagonCore/ResourceType.h"
#include <map>
#include <string>

/// <summary>
/// AssetManager를 도와주는 리소스 헬퍼, 리소스 관리에 도움을 준다.
/// </summary>

namespace Pg::Util::Helper
{
	class ResourceHelper
	{
	public:
		ResourceHelper();
		~ResourceHelper();

		//다양한 상대적 경로 기록 양식을 하나의 양식으로 강제로 통합해 저장해주는 함수이다. 
		static std::string ForcePathUniform(const std::string& filePath);

		//확장자 (Ex. .png) 를 리소스 타입으로 변환해주는 함수이다.
		static Pg::Core::Enums::eResourceType Ext2ResourceType(const std::string& extension);


	private:
		//확장자를 eResourceType으로 변환해준다.
		struct ExtensionSorter : public std::map<std::string, Pg::Core::Enums::eResourceType>
		{
			ExtensionSorter();
			~ExtensionSorter();
		};

	};
}


