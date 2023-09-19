#include "MathHelper.h"
#include <cstring>

namespace Pg::Graphics
{
	DirectX::XMFLOAT2 MathHelper::PG2XM_FLOAT2(Pg::Math::PGFLOAT2 value)
	{
		return { value.x, value.y };
	}

	DirectX::XMFLOAT3 MathHelper::PG2XM_FLOAT3(Pg::Math::PGFLOAT3 value)
	{
		return { value.x, value.y, value.z};
	}

	DirectX::XMFLOAT4 MathHelper::PG2XM_FLOAT4(Pg::Math::PGFLOAT4 value)
	{
		return { value.x, value.y, value.z, value.w };
	}

	DirectX::XMFLOAT4 MathHelper::PG2XM_QUATERNION(Pg::Math::PGQuaternion value)
	{
		return { value.x, value.y, value.z, value.w };
	}

	DirectX::XMFLOAT3X3 MathHelper::PG2XM_FLOAT3X3(Pg::Math::PGFLOAT3X3 value)
	{
		DirectX::XMFLOAT3X3 tReturn;
		std::memcpy(&tReturn, &value, sizeof(DirectX::XMFLOAT3X3));
		return tReturn;
	}

	DirectX::XMFLOAT4X4 MathHelper::PG2XM_FLOAT4X4(Pg::Math::PGFLOAT4X4 value)
	{
		DirectX::XMFLOAT4X4 tReturn;
		std::memcpy(&tReturn, &value, sizeof(DirectX::XMFLOAT4X4));
		return tReturn;
	}

	DirectX::XMMATRIX MathHelper::PG2XM_MATRIX(Pg::Math::PGFLOAT4X4 value)
	{
		DirectX::XMFLOAT4X4 tReturn;
		std::memcpy(&tReturn, &value, sizeof(DirectX::XMFLOAT4X4));
		DirectX::XMMATRIX ttReturn = DirectX::XMLoadFloat4x4(&tReturn);
		return ttReturn;
	}

	//void* memcpy (void* dest, const void* source, size_t num)

}