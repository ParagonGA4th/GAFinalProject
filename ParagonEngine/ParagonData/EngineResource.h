#pragma once
#include "BaseResource.h"
#include <string>

/// <summary>
/// GameEngine���� �ʿ��� ���ҽ����� ��ӹ��� ��.  ���� ������ ����.  
/// </summary>

namespace Pg::Data::Resources
{
	class EngineResource : public BaseResource
	{
	public:
		EngineResource(Pg::Data::Enums::eAssetDefine define, const std::string& typeInfoName, const std::string& filePath);
		~EngineResource();

		//���������� �ε�, ��ε����ִ� �Լ���. ���� �ڽ� Ŭ�������� ����ü�� ����/�Ҹ��ڿ��� ȣ��Ǿ��.
		virtual void InternalLoad() override;
		virtual void InternalUnload() override;


	};
}


