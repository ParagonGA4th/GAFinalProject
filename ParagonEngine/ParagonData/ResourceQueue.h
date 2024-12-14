#pragma once
#include <queue>
#include <string>
#include "../ParagonData/AssetDefines.h"

/// <summary>
/// 씬이 바뀔 때 등등의 상황에 일괄적으로 리소스의 목록을 받아서 전달할 용도로, 만든 컨테이너 클래스.
/// (사용여부 확정 X)
/// </summary>

namespace Pg::Data
{
	using ResourceDefinePair = std::pair<std::string, Pg::Data::Enums::eAssetDefine>;

	class ResourceQueue
	{

	public:
		//매개변수로 들어온 정보를 리소스 리스트에 추가한다.
		void AppendResourcePair(const std::string& filePath, Pg::Data::Enums::eAssetDefine define);
		
		//매개변수 : 내부 큐의 제일 앞 정보를 Pop하고, 리턴한다. // Bool : Pop 작업을 성공했는가 (이미 비어있으면 False 리턴)
		bool GetPopResourcePair(std::string& outFilePath, Pg::Data::Enums::eAssetDefine& outDefine);

		//Queue가 비었는가?
		bool IsEmpty();

		//Queue 내부 요소 클리어.
		void Clear();

	private:
		//			File Path			Asset Define
		std::queue<ResourceDefinePair> _resourceQueue;
	};
}




