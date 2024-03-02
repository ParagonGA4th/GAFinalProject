#pragma once
#include "../ParagonMath/PgMath.h"
#include <behaviortree_cpp_v3/basic_types.h>
/// <summary>
/// Groot을 통한 기록 초기 변환을 위해, Template Specialization을 기록해야 한다.
/// XML 로더가 커스텀 자료형을 로드한 뒤 가져오게 하려면, 이와 같은 절차 필요.
/// XML로더에서 이와 같은 과정 없으면, 필요 X.
/// https://www.behaviortree.dev/docs/tutorial-basics/tutorial_03_generic_ports
/// </summary>

namespace BT
{
	//String -> PGFLOAT3 변환 파이프라인 마련.
	template <> inline Pg::Math::PGFLOAT3 convertFromString(StringView str)
	{
		//유리수들이 Semicolon에 의해 나뉘었다고 예상.
		auto parts = splitString(str, ';');

		//PGFloat3이니. 3개로 체크.
		if (parts.size() != 3)
		{
			throw RuntimeError("Invalid Input For ParagonEngine Data");
		}
		else
		{
			Pg::Math::PGFLOAT3 output;
			output.x = convertFromString<double>(parts[0]);
			output.y = convertFromString<double>(parts[1]);
			output.z = convertFromString<double>(parts[2]);
			return output;
		}
	}
}