#include "MathHelper.h"
#include <cstring>

namespace Pg::Graphics::Helper
{
	DirectX::XMFLOAT2 MathHelper::PG2XM_FLOAT2(Pg::Math::PGFLOAT2 value)
	{
		return { value.x, value.y };
	}

	DirectX::XMFLOAT3 MathHelper::PG2XM_FLOAT3(Pg::Math::PGFLOAT3 value)
	{
		return { value.x, value.y, value.z };
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

	bool MathHelper::IsEqualXMFloat3X3(DirectX::XMFLOAT3X3 first, DirectX::XMFLOAT3X3 second)
	{
		// IIFE 람다 즉시 호출, goto문 사용을 피했다!
		bool tIsEqual = [&first, &second]
		{
			for (short i = 0; i < 3; i++)
			{
				for (short j = 0; j < 3; j++)
				{
					if (first.m[j][i] != second.m[j][i])
					{
						return false;
					}
				}
			}
			return true;
		}();

		return tIsEqual;
	}

	bool MathHelper::IsEqualXMFloat4X4(DirectX::XMFLOAT4X4 first, DirectX::XMFLOAT4X4 second)
	{
		bool tIsEqual = [&first, &second]
		{
			for (short i = 0; i < 4; i++)
			{
				for (short j = 0; j < 4; j++)
				{
					if (first.m[j][i] != second.m[j][i])
					{
						return false;
					}
				}
			}
			return true;
		}(); // IIFE 람다 즉시 호출, goto문 사용을 피했다!

		return tIsEqual;
	}

	DirectX::SimpleMath::Matrix MathHelper::AI2SM_MATRIX(const aiMatrix4x4& mat)
	{
		DirectX::SimpleMath::Matrix toReturn;

		toReturn._11 = mat.a1;
		toReturn._12 = mat.a2;
		toReturn._13 = mat.a3;
		toReturn._14 = mat.a4;
		toReturn._21 = mat.b1;
		toReturn._22 = mat.b2;
		toReturn._23 = mat.b3;
		toReturn._24 = mat.b4;
		toReturn._31 = mat.c1;
		toReturn._32 = mat.c2;
		toReturn._33 = mat.c3;
		toReturn._34 = mat.c4;
		toReturn._41 = mat.d1;
		toReturn._42 = mat.d2;
		toReturn._43 = mat.d3;
		toReturn._44 = mat.d4;

		return toReturn;
	}

	DirectX::SimpleMath::Quaternion MathHelper::AI2SM_QUATERNION(const aiQuaternion& quat)
	{
		return DirectX::SimpleMath::Quaternion(quat.x, quat.y, quat.z, quat.w);
	}

	DirectX::SimpleMath::Vector3 MathHelper::AI2SM_VECTOR3(const aiVector3D& vec)
	{
		return DirectX::SimpleMath::Vector3(vec.x, vec.y, vec.z);
	}

	void MathHelper::DecomposeAssembleMatrix(DirectX::SimpleMath::Matrix& mat)
	{
		using namespace DirectX;

		DirectX::XMVECTOR ttScale;
		DirectX::XMVECTOR ttRotQuat;
		DirectX::XMVECTOR ttTranslate;
		DirectX::XMMatrixDecompose(&ttScale, &ttRotQuat, &ttTranslate, mat);

		DirectX::XMMATRIX tAssembleTransform = DirectX::XMMatrixScalingFromVector(ttScale) *
			DirectX::XMMatrixRotationQuaternion(ttRotQuat) *
			DirectX::XMMatrixTranslationFromVector(ttTranslate);

		/*DirectX::XMMATRIX tAssembleTransform =
			DirectX::XMMatrixTranslationFromVector(ttTranslate) *
			DirectX::XMMatrixRotationQuaternion(ttRotQuat) *
			DirectX::XMMatrixScalingFromVector(ttScale);*/
			
		mat = tAssembleTransform;
	}

	DirectX::SimpleMath::Quaternion MathHelper::QuaternionSlerpNoFlip(const DirectX::SimpleMath::Quaternion& q1, const DirectX::SimpleMath::Quaternion& q2, float t)
	{
		// Ensure input quaternions are normalized
		DirectX::SimpleMath::Quaternion a = q1;
		a.Normalize();
		DirectX::SimpleMath::Quaternion b = q2;
		b.Normalize();

		// Calculate the dot product between the quaternions
		float dot = a.Dot(b);

		// Adjust the signs if needed to take the shorter path
		if (dot < 0.0f)
		{
			b = -b;
			dot = -dot;
		}

		// Clamp the dot product to prevent numerical instability
		dot = std::min(1.0f, std::max(-1.0f, dot));

		// Calculate the angle and axis of rotation
		float angle = acos(dot);

		// Perform the spherical linear interpolation (Slerp)
		DirectX::SimpleMath::Quaternion tBaseQuat = (a * sin((1.0f - t) * angle) + b * sin(t * angle));

		const DirectX::XMVECTOR v1 = DirectX::XMLoadFloat4(&tBaseQuat);
		const DirectX::XMVECTOR X = DirectX::XMVectorScale(v1, 1.f / sin(angle));
		DirectX::SimpleMath::Quaternion result;
		DirectX::XMStoreFloat4(&result, X);

		return result;
	}


}