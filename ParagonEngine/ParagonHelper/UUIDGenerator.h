#pragma once

#include <string>

/// <summary>
/// ���ο� UUID�� ��ȯ�ϴ� �Լ�.
/// </summary>

namespace Pg::Util
{
	class UUIDGenerator
	{
	public:
		 static std::string GetNewUUID();
	};
}

