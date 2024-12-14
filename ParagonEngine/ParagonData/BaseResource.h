#pragma once
#include "AssetDefines.h"
#include <string>
#include <memory>

/// <summary>
/// ���ҽ� �Ŵ����� ��ϵ� ��� ���ҽ��� �⺻.
/// </summary>

namespace Pg::Data::Resources
{
	class BaseResource
	{
	public:
		BaseResource(Pg::Data::Enums::eAssetDefine define, const std::string& typeInfoName, const std::string& filePath);
		virtual ~BaseResource();

		//���������� �ε�, ��ε����ִ� �Լ���. ���� �ڽ� Ŭ�������� ����ü�� ����/�Ҹ��ڿ��� ȣ��Ǿ��.
		virtual void InternalLoad() abstract;
		virtual void InternalUnload() abstract;

		std::string GetFilePath();
		std::string GetFileName();
		Pg::Data::Enums::eAssetDefine GetAssetDefine();

	protected:
		std::string _filePath;
		std::string _typeInfoName;
		Pg::Data::Enums::eAssetDefine _assetDefine;
	};
}


