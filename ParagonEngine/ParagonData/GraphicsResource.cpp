#include "GraphicsResource.h"
#include <cassert>

namespace Pg::Data::Resources
{
	GraphicsResource::GraphicsResource(Pg::Data::Enums::eAssetDefine define, const std::string& typeInfoName, const std::string& filePath) : BaseResource(define, typeInfoName, filePath)
	{

	}

	GraphicsResource::~GraphicsResource()
	{

	}

	void GraphicsResource::InternalLoad()
	{
		//�� �Լ��� ȣ��Ǿ�� �ȵȴ�. ��ӵ� ���¸� �����ؾ�!
		assert(false);
	}

	void GraphicsResource::InternalUnload()
	{
		//�� �Լ��� ȣ��Ǿ�� �ȵȴ�. ��ӵ� ���¸� �����ؾ�!
		assert(false);
	}

}
