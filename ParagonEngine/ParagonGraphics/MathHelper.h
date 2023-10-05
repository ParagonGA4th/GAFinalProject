#pragma once
#include "../ParagonMath/PgMath.h"
#include <DirectXMath.h>

#ifdef _DEBUG
#pragma comment(lib,"..\\Builds\\x64\\Debug\\ParagonAPI.lib")
#else
#pragma comment(lib,"..\\Builds\\x64\\Release\\ParagonAPI.lib")
#endif // _DEBUG

/// <summary>
/// 그래픽스 내부에서 수학 처리 등을 위한 헬퍼 클래스.
/// </summary>

namespace Pg::Graphics::Helper
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

		static bool IsEqualXMFloat3X3(DirectX::XMFLOAT3X3 first, DirectX::XMFLOAT3X3 second);
		static bool IsEqualXMFloat4X4(DirectX::XMFLOAT4X4 first, DirectX::XMFLOAT4X4 second);

	};
}


