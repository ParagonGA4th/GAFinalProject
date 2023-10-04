#pragma once
#include "../ParagonAPI/PgMath.h"

#ifdef _DEBUG
#pragma comment(lib, "..\\Builds\\x64\\Debug\\ParagonAPI.lib")
#else
#pragma comment(lib, "..\\Builds\\x64\\Release\\ParagonAPI.lib")
#endif // _DEBUG

//반환해줘야 할 데이터
// 주석 테스트 2
namespace Pg::Core
{
	struct CameraData
	{
		Pg::Math::PGFLOAT3 _position;
		Pg::Math::PGQuaternion _rotation;
		Pg::Math::PGFLOAT4X4 _viewMatrix;
		Pg::Math::PGFLOAT4X4 _projMatrix;
	};
}
