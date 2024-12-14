#pragma once
#include <queue>
#include <string>
#include "../ParagonData/AssetDefines.h"

/// <summary>
/// ���� �ٲ� �� ����� ��Ȳ�� �ϰ������� ���ҽ��� ����� �޾Ƽ� ������ �뵵��, ���� �����̳� Ŭ����.
/// (��뿩�� Ȯ�� X)
/// </summary>

namespace Pg::Data
{
	using ResourceDefinePair = std::pair<std::string, Pg::Data::Enums::eAssetDefine>;

	class ResourceQueue
	{

	public:
		//�Ű������� ���� ������ ���ҽ� ����Ʈ�� �߰��Ѵ�.
		void AppendResourcePair(const std::string& filePath, Pg::Data::Enums::eAssetDefine define);
		
		//�Ű����� : ���� ť�� ���� �� ������ Pop�ϰ�, �����Ѵ�. // Bool : Pop �۾��� �����ߴ°� (�̹� ��������� False ����)
		bool GetPopResourcePair(std::string& outFilePath, Pg::Data::Enums::eAssetDefine& outDefine);

		//Queue�� ����°�?
		bool IsEmpty();

		//Queue ���� ��� Ŭ����.
		void Clear();

	private:
		//			File Path			Asset Define
		std::queue<ResourceDefinePair> _resourceQueue;
	};
}




