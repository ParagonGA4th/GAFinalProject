#pragma once
#include "../ParagonMath/PgMath.h"
#include <string>

//Collider의 디버그 정보를 하나로 묶어놓은 구조체.
//그래픽스로 전달하여 렌더링 할 예정이다.
namespace Pg::Engine
{
	struct BoxInfo
	{
		Pg::Math::PGFLOAT4X4 worldTM;
		Pg::Math::PGFLOAT3 totalScale;
		Pg::Math::PGFLOAT4 color;
	};
}