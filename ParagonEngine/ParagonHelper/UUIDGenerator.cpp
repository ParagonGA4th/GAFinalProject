#include "UUIDGenerator.h"
#include <windows.h>
#include <combaseapi.h>
#include <cassert>

namespace Pg::Util
{
	std::string UUIDGenerator::GetNewUUID()
	{
		GUID tGUID;

		//Windows ���� GUID Generator ���.
		HRESULT hr = CoCreateGuid(&tGUID);
		assert(SUCCEEDED(hr));

		//GUID String ���� = {XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX}
		wchar_t tGuidString[39];
		assert((StringFromGUID2(tGUID, tGuidString, 39) != 0) && "GUID�� C-Style wchar_t ��Ʈ������ ��ȯ ����");

		int guidStringLen = lstrlenW(tGuidString);
		char tNarrowString[39];
		WideCharToMultiByte(CP_ACP, 0, tGuidString, guidStringLen + 1, tNarrowString, sizeof(tNarrowString), nullptr, nullptr);

		return std::string(tNarrowString);
	}

}