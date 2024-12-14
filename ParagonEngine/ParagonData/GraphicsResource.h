#pragma once
#include "BaseResource.h"
#include "AssetDefines.h"
#include <string>

/// <summary>
/// Graphics���� �ʿ��� ���ҽ����� ��ӹ��� ��. ���� ������ ����.  
/// </summary>

namespace Pg::Data::Resources
{
	class GraphicsResource : public BaseResource
	{
	public:
		GraphicsResource(Pg::Data::Enums::eAssetDefine define, const std::string& typeInfoName, const std::string& filePath);
		~GraphicsResource();
		
		//���������� �ε�, ��ε����ִ� �Լ���. ���� �ڽ� Ŭ�������� ����ü�� ����/�Ҹ��ڿ��� ȣ��Ǿ��.
		virtual void InternalLoad() override;
		virtual void InternalUnload() override;

	};
}




