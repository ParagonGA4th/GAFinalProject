#include "MathHelper.h"
#include "../ParagonData/GameConstantData.h"
#include <cstring>
#include <cassert>

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

	DirectX::XMVECTOR MathHelper::PG2XM_VECTOR(Pg::Math::PGFLOAT4 value)
	{
		DirectX::XMFLOAT4 tFF = { value.x, value.y, value.z, value.w };
		return DirectX::XMLoadFloat4(&tFF);
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

	//void MathHelper::DecomposeAssembleFlipMatrix(DirectX::SimpleMath::Matrix& mat)
	//{
	//	using namespace DirectX;
	//
	//	DirectX::XMVECTOR ttScale;
	//	DirectX::XMVECTOR ttRotQuat;
	//	DirectX::XMVECTOR ttTranslate;
	//	DirectX::XMMatrixDecompose(&ttScale, &ttRotQuat, &ttTranslate, mat);
	//
	//	DirectX::XMMATRIX tAssembleTransform = DirectX::XMMatrixScalingFromVector(ttScale) *
	//		DirectX::XMMatrixRotationQuaternion(ttRotQuat) *
	//		DirectX::XMMatrixTranslationFromVector(ttTranslate);
	//
	//	mat = tAssembleTransform;
	//}

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

	DirectX::XMFLOAT3 MathHelper::RotateVectorAroundAxis(DirectX::XMFLOAT3 vecToRotate, DirectX::XMFLOAT3 rotAxis, float angleInRad)
	{
		using namespace DirectX;

		XMFLOAT3 vectorToRotate = { vecToRotate.x, vecToRotate.y, vecToRotate.z };;
		XMFLOAT3 rotationAxis = { rotAxis.x, rotAxis.y, rotAxis.z };
		float angleInRadians = angleInRad;

		// Step 1: Create Matrices
		XMMATRIX translationToOrigin = XMMatrixTranslation(-rotationAxis.x, -rotationAxis.y, -rotationAxis.z);
		XMMATRIX rotation = XMMatrixRotationAxis(XMLoadFloat3(&rotationAxis), angleInRadians);
		XMMATRIX translationBack = XMMatrixTranslation(rotationAxis.x, rotationAxis.y, rotationAxis.z);

		// Step 2: Combine Matrices
		XMMATRIX transformationMatrix = translationToOrigin * rotation * translationBack;

		// Step 3: Apply Transformation
		XMFLOAT3 rotatedVector;
		XMStoreFloat3(&rotatedVector, XMVector3Transform(XMLoadFloat3(&vectorToRotate), transformationMatrix));

		return { rotatedVector.x, rotatedVector.y, rotatedVector.z };
	}

	DirectX::XMMATRIX MathHelper::AI2XM_MATRIX(const aiMatrix4x4& matrix)
	{
		DirectX::XMMATRIX m;

		m.r[0].m128_f32[0] = matrix.a1;
		m.r[0].m128_f32[1] = matrix.a2;
		m.r[0].m128_f32[2] = matrix.a3;
		m.r[0].m128_f32[3] = matrix.a4;

		m.r[1].m128_f32[0] = matrix.b1;
		m.r[1].m128_f32[1] = matrix.b2;
		m.r[1].m128_f32[2] = matrix.b3;
		m.r[1].m128_f32[3] = matrix.b4;

		m.r[2].m128_f32[0] = matrix.c1;
		m.r[2].m128_f32[1] = matrix.c2;
		m.r[2].m128_f32[2] = matrix.c3;
		m.r[2].m128_f32[3] = matrix.c4;

		m.r[3].m128_f32[0] = matrix.d1;
		m.r[3].m128_f32[1] = matrix.d2;
		m.r[3].m128_f32[2] = matrix.d3;
		m.r[3].m128_f32[3] = matrix.d4;

		return m;
	}

	DirectX::XMVECTOR MathHelper::UnprojectScreenPointToWorldSpaceRay(DirectX::XMVECTOR screenPoint, DirectX::XMMATRIX viewProjection, DirectX::XMVECTOR viewportSize, DirectX::XMVECTOR viewportOrigin)
	{
		using namespace DirectX;

		// 스크린 스페이스를 NDC Range [-1, 1]로 조절.
		XMVECTOR point = XMVectorSet(
			(screenPoint.m128_f32[0] - viewportOrigin.m128_f32[0]) / viewportSize.m128_f32[0] * 2.0f - 1.0f,
			(screenPoint.m128_f32[1] - viewportOrigin.m128_f32[1]) / viewportSize.m128_f32[1] * -2.0f + 1.0f,
			screenPoint.m128_f32[2],
			1.0f
		);

		//스크린 스페이스에서 월드 스페이스 Ray로 "역투영".
		XMMATRIX viewProjectionInverse = XMMatrixInverse(nullptr, viewProjection);
		XMVECTOR rayOrigin = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
		XMVECTOR rayDir = XMVector3TransformCoord(point, viewProjectionInverse);
		rayDir = XMVector3Normalize(rayDir);

		return rayDir;
	}

	DirectX::XMVECTOR MathHelper::GetWorldPointFromScreenPoint_Depth(float screenPtX, float screenPtY, DirectX::XMMATRIX viewProjection, DirectX::XMMATRIX projectionMatrix, DirectX::XMVECTOR viewportSize, DirectX::XMVECTOR viewportOrigin, float depth)
	{
		using namespace DirectX;

		XMVECTOR screenPoint = XMVectorSet(screenPtX, screenPtY, 0.0f, 0.0f);
		XMVECTOR rayDir = UnprojectScreenPointToWorldSpaceRay(screenPoint, viewProjection, viewportSize, viewportOrigin);
		XMVECTOR rayOrigin = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

		//유효한 범위 안이어야 한다. 아닐 시 assert.
		assert(screenPtX >= 0 && screenPtX < Pg::Data::GameConstantData::WIDTH && 
			screenPtY >= 0 && screenPtY < Pg::Data::GameConstantData::HEIGHT);
		
		//Depth 값은 밖에서 연산해 온다.
		XMVECTOR projectedPoint = XMVectorSet(screenPoint.m128_f32[0], screenPoint.m128_f32[1], depth, 1.0f);
		XMVECTOR worldPoint = XMVector3TransformCoord(projectedPoint, XMMatrixInverse(nullptr, projectionMatrix));
		return worldPoint;

		//사용 용례:
		//XMVECTOR viewportSize = XMVectorSet(viewportWidth, viewportHeight, 1.0f, 1.0f);
		//XMVECTOR viewportOrigin = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		//
		//// View-projection matrix (combined from view and projection matrices)
		//XMMATRIX viewProjection = viewMatrix * projectionMatrix;
	}

}