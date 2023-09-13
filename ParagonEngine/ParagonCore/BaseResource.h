#pragma once
#include "ResourceType.h"
#include <string>

/// <summary>
/// 리소스 매니저에 기록될 모든 리소스의 기본.
/// </summary>

namespace Pg::Core::Resources
{
	class BaseResource
	{
	public:
		BaseResource(const std::string& filePath);
		virtual ~BaseResource();

		//내부적으로 로드, 언로드해주는 함수들. 최종 자식 클래스들의 구현체의 생성/소멸자에서 호출되어야.
		virtual void InternalLoad() abstract;
		virtual void InternalUnload() abstract;

	protected:
		std::string _filePath;
		Pg::Core::Enum::eResourceType _resourceType;
	};
}


