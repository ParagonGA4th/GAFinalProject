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
		//�� �Լ��� ȣ��Ǿ�� �ȵȴ�. ��ӵ� ���¸� �����ؾ�!
		assert(false);
	}

	void EngineResource::InternalUnload()
	{
		//�� �Լ��� ȣ��Ǿ�� �ȵȴ�. ��ӵ� ���¸� �����ؾ�!
		assert(false);
	}

}