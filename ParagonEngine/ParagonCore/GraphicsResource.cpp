#include "GraphicsResource.h"
#include <cassert>

namespace Pg::Core::Resources
{
	GraphicsResource::GraphicsResource(const std::string& filePath) : BaseResource(filePath)
	{

	}

	GraphicsResource::~GraphicsResource()
	{

	}

	void GraphicsResource::InternalLoad()
	{
		//이 함수가 호출되어서는 안된다. 상속된 형태만 가능해야!
		assert(false);
	}

	void GraphicsResource::InternalUnload()
	{
		//이 함수가 호출되어서는 안된다. 상속된 형태만 가능해야!
		assert(false);
	}

}
