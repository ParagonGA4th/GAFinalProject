#pragma once
#include "AssetDefines.h"
#include <string>
#include <memory>

/// <summary>
/// 리소스 매니저에 기록될 모든 리소스의 기본.
/// </summary>

namespace Pg::Core::Resources
{
	class BaseResource
	{
	public:
		BaseResource(Pg::Core::Enums::eAssetDefine define, const std::string& typeInfoName, const std::string& filePath);
		virtual ~BaseResource();

		//내부적으로 로드, 언로드해주는 함수들. 최종 자식 클래스들의 구현체의 생성/소멸자에서 호출되어야.
		virtual void InternalLoad() abstract;
		virtual void InternalUnload() abstract;

		//자신을 실제로 자신이 실제하는 하위 클래스 형태로 돌려준다.
		template<typename T>
		std::shared_ptr<T> CastToSelf();

	protected:
		std::string _filePath;
		std::string _typeInfoName;
		Pg::Core::Enums::eAssetDefine _assetDefine;
	};

	template<typename T>
	std::shared_ptr<T>
		Pg::Core::Resources::BaseResource::CastToSelf()
	{

	}

}


