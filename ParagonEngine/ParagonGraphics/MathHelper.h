#pragma once
#include "../ParagonAPI/PgMath.h"
#include <DirectXMath.h>

#ifdef _DEBUG
#pragma comment(lib,"..\\x64\\Debug\\ParagonAPI.lib")
#else
#pragma comment(lib,"..\\x64\\Release\\ParagonAPI.lib")
#endif // _DEBUG

/// <summary>
/// 그래픽스 내부에서 수학 처리 등을 위한 헬퍼 클래스.
/// </summary>

namespace Pg::Graphics
{
	class MathHelper
	{
	public:
		static DirectX::XMFLOAT2 PG2XM_FLOAT2(Pg::Math::PGFLOAT2 value);
		static DirectX::XMFLOAT3 PG2XM_FLOAT3(Pg::Math::PGFLOAT3 value);
		static DirectX::XMFLOAT4 PG2XM_FLOAT4(Pg::Math::PGFLOAT4 value);
		static DirectX::XMFLOAT4 PG2XM_QUATERNION(Pg::Math::PGQuaternion value);
		static DirectX::XMFLOAT3X3 PG2XM_FLOAT3X3(Pg::Math::PGFLOAT3X3 value);
		static DirectX::XMFLOAT4X4 PG2XM_FLOAT4X4(Pg::Math::PGFLOAT4X4 value);
		static DirectX::XMMATRIX PG2XM_MATRIX(Pg::Math::PGFLOAT4X4 value);


	};
}


