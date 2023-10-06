#pragma once
#include "BaseResource.h"
#include <string>

/// <summary>
/// GameEngine에서 필요할 리소스들이 상속받을 것.  구분 연동을 위해.  
/// </summary>

namespace Pg::Data::Resources
{
	class EngineResource : public BaseResource
	{
	public:
		EngineResource(Pg::Data::Enums::eAssetDefine define, const std::string& typeInfoName, const std::string& filePath);
		~EngineResource();

		//내부적으로 로드, 언로드해주는 함수들. 최종 자식 클래스들의 구현체의 생성/소멸자에서 호출되어야.
		virtual void InternalLoad() override;
		virtual void InternalUnload() override;


	};
}


