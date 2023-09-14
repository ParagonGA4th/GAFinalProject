#include "EngineResource.h"
#include <cassert>

namespace Pg::Core::Resources
{
	EngineResource::EngineResource(const std::string& filePath) : BaseResource(filePath)
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