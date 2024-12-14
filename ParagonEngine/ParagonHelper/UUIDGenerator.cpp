#include "UUIDGenerator.h"
#include <windows.h>
#include <combaseapi.h>
#include <cassert>

namespace Pg::Util
{
	std::string UUIDGenerator::GetNewUUID()
	{
		GUID tGUID;

		//Windows 고유 GUID Generator 사용.
		HRESULT hr = CoCreateGuid(&tGUID);
		assert(SUCCEEDED(hr));

		//GUID String 형식 = {XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX}
		wchar_t tGuidString[39];
		assert((StringFromGUID2(tGUID, tGuidString, 39) != 0) && "GUID가 C-Style wchar_t 스트링으로 변환 실패");

		int guidStringLen = lstrlenW(tGuidString);
		char tNarrowString[39];
		WideCharToMultiByte(CP_ACP, 0, tGuidString, guidStringLen + 1, tNarrowString, sizeof(tNarrowString), nullptr, nullptr);

		return std::string(tNarrowString);
	}

}