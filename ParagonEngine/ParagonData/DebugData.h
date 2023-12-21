#pragma once
#include "../ParagonMath/PgMath.h"

/// <summary>
/// 변지상의 Collider의 디버그 정보를 담은 구조체...
/// 그래픽스로 전달하여 렌더링 할 예정.
/// </summary>
namespace Pg::Data
{
	//World TM에는 Scale 관련 정보가 들어오지 않는다.
	struct BoxInfo
	{
		Pg::Math::PGFLOAT4X4 worldTM;
		Pg::Math::PGFLOAT3 scale;
		Pg::Math::PGFLOAT4 color;
	};

	struct LineInfo
	{
		Pg::Math::PGFLOAT3 beginPoint;
		Pg::Math::PGFLOAT3 endPoint;
		Pg::Math::PGFLOAT4 color;
	};

	struct SphereInfo
	{
		Pg::Math::PGFLOAT4X4 worldTM;
		Pg::Math::PGFLOAT3 scale;
		Pg::Math::PGFLOAT4 color;
	};

	struct CapsuleInfo
	{
		Pg::Math::PGFLOAT4X4 worldTM;
		Pg::Math::PGFLOAT3 scale;
		Pg::Math::PGFLOAT4 color;
	};

	struct PlaneInfo
	{
		Pg::Math::PGFLOAT4X4 worldTM;
		Pg::Math::PGFLOAT3 scale;
		Pg::Math::PGFLOAT4 color;
	};
}