#include "EngineResource.h"
#include <cassert>

namespace Pg::Data::Resources
{
	EngineResource::EngineResource(Pg::Data::Enums::eAssetDefine define, const std::string& typeInfoName, const std::string& filePath) : BaseResource(define, typeInfoName, filePath)
	{

	}

	EngineResource::~EngineResource()
	{

	}

	void EngineResource::InternalLoad()
	{
		//이 함수가 호출되어서는 안된다. 상속된 형태만 가능해야!
		assert(false);
	}

	void EngineResource::InternalUnload()
	{
		//이 함수가 호출되어서는 안된다. 상속된 형태만 가능해야!
		assert(false);
	}

}