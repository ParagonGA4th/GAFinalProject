#pragma once

#include <string>

/// <summary>
/// 새로운 UUID를 반환하는 함수.
/// </summary>

namespace Pg::Util
{
	class UUIDGenerator
	{
	public:
		 static std::string GetNewUUID();
	};
}

