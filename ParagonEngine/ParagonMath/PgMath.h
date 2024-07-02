#pragma once
#include <functional>
#include <DirectXMath.h>
#include <limits>

/// ParagonEngine에서 쓰일 Math 기본 형태.
/// 추가적인 종속성을 막기 위해서, 커스텀 구조체로 관리.
/// 

namespace Pg::Math
{
	struct PGFLOAT3;
	struct PGFLOAT4X4;
}

namespace Pg::Math
{
	struct PGFLOAT2
	{
		float x;
		float y;

		PGFLOAT2() = default;

		PGFLOAT2(const PGFLOAT2&) = default;
		PGFLOAT2& operator=(const PGFLOAT2&) = default;

		PGFLOAT2(PGFLOAT2&&) = default;
		PGFLOAT2& operator=(PGFLOAT2&&) = default;

		constexpr PGFLOAT2(float _x, float _y) noexcept;

		constexpr PGFLOAT2 operator-(const PGFLOAT2& rhs) const noexcept;
		
		constexpr PGFLOAT2 operator*(float rhs) const noexcept;

		constexpr PGFLOAT2 operator+(const PGFLOAT2& rhs) const noexcept;

		// Unary Minus Operator Overload
		PGFLOAT2 operator-() const {
			PGFLOAT2 v;
			v.x = -x;
			v.y = -y;
			return v;
		}

		bool operator==(const PGFLOAT2& rhs) const
		{
			return (std::abs(x - rhs.x) < std::numeric_limits<float>::epsilon() &&
				std::abs(y - rhs.y) < std::numeric_limits<float>::epsilon());
		}

		bool operator!=(const PGFLOAT2& rhs) const
		{
			return !(*this == rhs);
		}
	};

	struct PGFLOAT3
	{
		float x;
		float y;
		float z;

		PGFLOAT3() = default;

		PGFLOAT3(const PGFLOAT3&) = default;
		PGFLOAT3& operator=(const PGFLOAT3&) = default;

		PGFLOAT3(PGFLOAT3&&) = default;
		PGFLOAT3& operator=(PGFLOAT3&&) = default;

		constexpr PGFLOAT3(float _x, float _y, float _z) noexcept;

		PGFLOAT3 operator-(const PGFLOAT3& rhs) const;

		PGFLOAT3 operator*(float rhs) const;

		float operator*(PGFLOAT3 rhs) const;

		PGFLOAT3& operator+=(const PGFLOAT3& rhs);

		PGFLOAT3 operator+(const PGFLOAT3& rhs);

		bool operator<(const PGFLOAT3& rhs) const;

		bool operator>(const PGFLOAT3& rhs) const;

		bool operator>=(const PGFLOAT3& rhs) const;

		bool operator<=(const PGFLOAT3& rhs) const;

		// Unary Minus Operator Overload
		PGFLOAT3 operator-() const {
			PGFLOAT3 v;
			v.x = -x;
			v.y = -y;
			v.z = -z;
			return v;
		}

		bool operator==(const PGFLOAT3& rhs) const
		{
			return (std::abs(x - rhs.x) < std::numeric_limits<float>::epsilon() &&
				std::abs(y - rhs.y) < std::numeric_limits<float>::epsilon() &&
				std::abs(z - rhs.z) < std::numeric_limits<float>::epsilon());
		}

		bool operator!=(const PGFLOAT3& rhs) const 
		{
			return !(*this == rhs);
		}

		//단위 행렬
		static PGFLOAT3 GlobalUp();
		static PGFLOAT3 GlobalRight();
		static PGFLOAT3 GlobalForward();
	};

	struct PGFLOAT4
	{
		union
		{
			struct
			{
				float x;
				float y;
				float z;
				float w;
			};
			struct
			{
				float r;
				float g;
				float b;
				float a;
			};
		};

		PGFLOAT4();
		PGFLOAT4(const PGFLOAT4&) = default;
		PGFLOAT4& operator=(const PGFLOAT4&) = default;

		PGFLOAT4(PGFLOAT4&&) = default;
		PGFLOAT4& operator=(PGFLOAT4&&) = default;

		constexpr PGFLOAT4(float _x, float _y, float _z, float _w) noexcept;

		constexpr PGFLOAT4(const PGFLOAT3& rhs, float _w) noexcept;
			
		//W를 포함하는 연산
		PGFLOAT4& operator*=(const PGFLOAT4X4& rhs);
		//W를 포함하는 연산
		PGFLOAT4 operator*(const PGFLOAT4X4& rhs);
		//W를 포함하는 연산
		PGFLOAT4 operator+(const PGFLOAT4& rhs);
		//W를 포함하는 연산
		PGFLOAT4 operator-(const PGFLOAT4& rhs);

		//W를 포함하지 않는 연산
		bool operator<(const PGFLOAT4& rhs) const;
		//W를 포함하지 않는 연산
		bool operator>(const PGFLOAT4& rhs) const;
		//W를 포함하지 않는 연산
		bool operator>=(const PGFLOAT4& rhs) const;
		//W를 포함하지 않는 연산
		bool operator<=(const PGFLOAT4& rhs) const;

		bool operator==(const PGFLOAT4& rhs) const
		{
			return (std::abs(x - rhs.x) < std::numeric_limits<float>::epsilon() &&
				std::abs(y - rhs.y) < std::numeric_limits<float>::epsilon() &&
				std::abs(z - rhs.z) < std::numeric_limits<float>::epsilon() &&
				std::abs(w - rhs.w) < std::numeric_limits<float>::epsilon());
		}

		bool operator!=(const PGFLOAT4& rhs) const
		{
			return !(*this == rhs);
		}
	};

	struct PGQuaternion
	{
		float w;
		float x;
		float y;
		float z;

		PGQuaternion();

		PGQuaternion(const PGQuaternion&) = default;
		PGQuaternion& operator=(const PGQuaternion&) = default;

		PGQuaternion(PGQuaternion&&) = default;
		PGQuaternion& operator=(PGQuaternion&&) = default;

		constexpr PGQuaternion(float _w, float _x, float _y, float _z) noexcept;

		PGQuaternion Conjugate() const;

		bool operator==(const PGQuaternion& rhs) const
		{
			return (std::abs(x - rhs.x) < std::numeric_limits<float>::epsilon() &&
				std::abs(y - rhs.y) < std::numeric_limits<float>::epsilon() &&
				std::abs(z - rhs.z) < std::numeric_limits<float>::epsilon() &&
				std::abs(w - rhs.w) < std::numeric_limits<float>::epsilon());
		}

		bool operator!=(const PGQuaternion& rhs) const
		{
			return !(*this == rhs);
		}
	};

	struct PGFLOAT4X4
	{
		union
		{
			struct
			{
				float _11, _12, _13, _14;
				float _21, _22, _23, _24;
				float _31, _32, _33, _34;
				float _41, _42, _43, _44;
			};
			float m[4][4];
		};

		constexpr PGFLOAT4X4() noexcept;

		PGFLOAT4X4(const PGFLOAT4X4&) = default;
		PGFLOAT4X4& operator=(const PGFLOAT4X4&) = default;

		PGFLOAT4X4(PGFLOAT4X4&&) = default;
		PGFLOAT4X4& operator=(PGFLOAT4X4&&) = default;

		constexpr PGFLOAT4X4(float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33) noexcept;

		//DX처럼 Float4x4를 세팅할 수 있게.
		constexpr PGFLOAT4X4(float mArr[16]) noexcept;

		PGFLOAT4X4 operator*(const PGFLOAT4X4& rhs);
		PGFLOAT4X4& operator*=(const PGFLOAT4X4& rhs);

		float& operator()(size_t row, size_t column) noexcept;

		//단위 행렬
		static PGFLOAT4X4 Identity();

		// 역행렬
		PGFLOAT4X4 Inverse() const;

		PGFLOAT4X4 Transpose() const;
	};

	struct PGFLOAT3X3
	{
		union
		{
			struct
			{
				float _11, _12, _13;
				float _21, _22, _23;
				float _31, _32, _33;
			};
			float m[3][3];
		};

		PGFLOAT3X3();

		PGFLOAT3X3(const PGFLOAT3X3&) = default;
		PGFLOAT3X3& operator=(const PGFLOAT3X3&) = default;

		PGFLOAT3X3(PGFLOAT3X3&&) = default;
		PGFLOAT3X3& operator=(PGFLOAT3X3&&) = default;

		constexpr PGFLOAT3X3(float m00, float m01, float m02,
			float m10, float m11, float m12,
			float m20, float m21, float m22) noexcept;

		static PGFLOAT3X3 Identity();

		PGFLOAT3X3 operator*(const PGFLOAT3X3& rhs);

		PGFLOAT3X3& operator*=(const PGFLOAT3X3& rhs);
	};

	constexpr float PG_PI = 3.141592654f;
	constexpr float PG_2PI = 6.283185307f;
	constexpr float PG_1DIVPI = 0.318309886f;
	constexpr float PG_1DIV2PI = 0.159154943f;
	constexpr float PG_PIDIV2 = 1.570796327f;
	constexpr float PG_PIDIV4 = 0.785398163f;

	inline float PGConvertToRadians(float fDegrees) noexcept { return fDegrees * (PG_PI / 180.0f); }
	inline float PGConvertToDegrees(float fRadians) noexcept { return fRadians * (180.0f / PG_PI); }

	float PGFloat3Length(const Pg::Math::PGFLOAT3& f) noexcept;
	float PGFloat4Length(const Pg::Math::PGFLOAT4& f) noexcept;
	Pg::Math::PGFLOAT3 PGFloat3Normalize(const Pg::Math::PGFLOAT3& f) noexcept;
	Pg::Math::PGFLOAT4 PGFloat4Normalize(const Pg::Math::PGFLOAT4& f) noexcept;
	Pg::Math::PGQuaternion PGQuaternionNormalize(const Pg::Math::PGQuaternion& f) noexcept;
	float PGFloat3Dot(const Pg::Math::PGFLOAT3& lhs, const Pg::Math::PGFLOAT3& rhs);
	Pg::Math::PGFLOAT3 PGFloat3Cross(const Pg::Math::PGFLOAT3& lhs, const Pg::Math::PGFLOAT3& rhs);
	Pg::Math::PGFLOAT3 PGFloat3MultiplyMatrix(const Pg::Math::PGFLOAT3& lhs, const Pg::Math::PGFLOAT4X4& rhs);
	Pg::Math::PGFLOAT4 PGFloat4MultiplyMatrix(const Pg::Math::PGFLOAT4& lhs, const Pg::Math::PGFLOAT4X4& rhs);
	Pg::Math::PGQuaternion PGRotateQuaternionY(const Pg::Math::PGQuaternion& quaternion, float radian);
	Pg::Math::PGQuaternion PGRotateQuaternion(const Pg::Math::PGQuaternion& quaternion, const Pg::Math::PGFLOAT3& axis, float radian);
	Pg::Math::PGQuaternion PGQuaternionMultiply(const Pg::Math::PGQuaternion& lhs, const Pg::Math::PGQuaternion& rhs);
	Pg::Math::PGFLOAT4X4 PGMatrixRotationX(float angle);
	Pg::Math::PGFLOAT4X4 PGMatrixRotationY(float angle);
	Pg::Math::PGFLOAT3X3 PGInverseMatrix(const Pg::Math::PGFLOAT3X3& mat);
	Pg::Math::PGFLOAT4 PGQuaternionToFloat4(const Pg::Math::PGQuaternion& quaternion);
	Pg::Math::PGQuaternion PGFloat4ToQuaternion(const Pg::Math::PGFLOAT4& f4);
	Pg::Math::PGQuaternion PGMatrixToQuaternion(const Pg::Math::PGFLOAT4X4& matrix);

	Pg::Math::PGFLOAT4X4 PGScaleMatrix(const Pg::Math::PGFLOAT3 scale);
	Pg::Math::PGFLOAT4X4 PGRotationMatrix(const Pg::Math::PGQuaternion rotation);
	Pg::Math::PGFLOAT4X4 PGTranslateMatrix(const Pg::Math::PGFLOAT3 position);

	Pg::Math::PGFLOAT4X4 PGGetWorldMatrixFromValues(const Pg::Math::PGFLOAT3 position, const Pg::Math::PGQuaternion rotation, const Pg::Math::PGFLOAT3 scale);
	Pg::Math::PGFLOAT4X4 PGMatrixPerspectiveFovLH(float fovAngleY, float aspectRatio, float nearZ, float farZ);
	Pg::Math::PGFLOAT4X4 PGMatrixOrthographicLH(float viewWidth, float viewHeight, float nearZ, float farZ);

	//Euler<->Quaternion
	Pg::Math::PGQuaternion PGEulerToQuaternion(const Pg::Math::PGFLOAT3& euler);
	Pg::Math::PGQuaternion PGEulerToQuaternion(float x, float y, float z);
	Pg::Math::PGFLOAT3 PGQuaternionToEuler(const Pg::Math::PGQuaternion& quaternion);
	Pg::Math::PGFLOAT3 PGQuaternionToEuler(float w, float x, float y, float z);

	//RotateVectorAroundAxis 
	Pg::Math::PGFLOAT3 PGRotateVectorAroundAxis(Pg::Math::PGFLOAT3 vecToRotate, Pg::Math::PGFLOAT3 rotAxis, float angleInRad);

	//SimpleMath <-> PGMath
	Pg::Math::PGFLOAT2 XM2PG_FLOAT2(const DirectX::XMFLOAT2& val);
	Pg::Math::PGFLOAT3 XM2PG_FLOAT3(const DirectX::XMFLOAT3& val);
	Pg::Math::PGFLOAT3 XM2PG_FLOAT3_VECTOR(const DirectX::XMVECTOR& val);
	Pg::Math::PGFLOAT4 XM2PG_FLOAT4(const DirectX::XMFLOAT4& val);
	Pg::Math::PGFLOAT4 XM2PG_FLOAT4_VECTOR(const DirectX::XMVECTOR& val);
	Pg::Math::PGQuaternion XM2PG_QUATERNION(const DirectX::XMFLOAT4& val);
	Pg::Math::PGQuaternion XM2PG_QUATERNION(const DirectX::XMVECTOR& val);
	Pg::Math::PGFLOAT3X3 XM2PG_FLOAT3X3(const DirectX::XMFLOAT3X3& val);
	Pg::Math::PGFLOAT4X4 XM2PG_FLOAT4X4(const DirectX::XMFLOAT4X4& val);
	Pg::Math::PGFLOAT3X3 XM2PG_MATRIX3X3(const DirectX::XMMATRIX& val);
	Pg::Math::PGFLOAT4X4 XM2PG_MATRIX4X4(const DirectX::XMMATRIX& val);
	
	DirectX::XMFLOAT2 PG2XM_FLOAT2(const Pg::Math::PGFLOAT2& val);
	DirectX::XMFLOAT3 PG2XM_FLOAT3(const Pg::Math::PGFLOAT3& val);
	DirectX::XMVECTOR PG2XM_FLOAT3_VECTOR(const Pg::Math::PGFLOAT3& val);
	DirectX::XMFLOAT4 PG2XM_FLOAT4(const Pg::Math::PGFLOAT4& val);
	DirectX::XMVECTOR PG2XM_FLOAT4_VECTOR(const Pg::Math::PGFLOAT4& val);
	DirectX::XMFLOAT4 PG2XM_QUATERNION(const Pg::Math::PGQuaternion& val);
	DirectX::XMVECTOR PG2XM_QUATERNION_VECTOR(const Pg::Math::PGQuaternion& val);
	DirectX::XMFLOAT3X3 PG2XM_FLOAT3X3(const Pg::Math::PGFLOAT3X3& val);
	DirectX::XMMATRIX PG2XM_MATRIX3X3(const Pg::Math::PGFLOAT3X3& val);
	DirectX::XMFLOAT4X4 PG2XM_FLOAT4X4(const Pg::Math::PGFLOAT4X4& val);
	DirectX::XMMATRIX PG2XM_MATRIX4X4(const Pg::Math::PGFLOAT4X4& val);

	//UtilityFunctions - Unity, SimpleMath 모델링.
	Pg::Math::PGQuaternion FromToRotation(const Pg::Math::PGFLOAT3& fromDirection, const Pg::Math::PGFLOAT3& toDirection);

	Pg::Math::PGQuaternion PGLookRotation(const Pg::Math::PGFLOAT3& forward, const Pg::Math::PGFLOAT3& up);
	Pg::Math::PGQuaternion PGQuaternionSlerp(Pg::Math::PGQuaternion a, Pg::Math::PGQuaternion b, float t);
	Pg::Math::PGFLOAT3 PGFloat3Lerp(const Pg::Math::PGFLOAT3& a, const Pg::Math::PGFLOAT3& b, float t);

	Pg::Math::PGFLOAT3 PGReflectVectorAgainstAxis(const Pg::Math::PGFLOAT3& toFlip, const Pg::Math::PGFLOAT3& baseAxis);

	Pg::Math::PGFLOAT4X4 GetViewMatrixFromTransformValues(Pg::Math::PGFLOAT3 right, Pg::Math::PGFLOAT3 up, Pg::Math::PGFLOAT3 forward, Pg::Math::PGFLOAT3 pos);

	//PhysX 좌표계를 DirectX 좌표계로 맞추기. 위치는 같고 Rotation은 다르다!!
	Pg::Math::PGFLOAT3 PGConvertD3DVec3RotToPhysX(Pg::Math::PGFLOAT3 val);
	Pg::Math::PGFLOAT3 PGConvertPhysXVec3RotToD3D(Pg::Math::PGFLOAT3 val);

	//Quaternion은 검증해야 할것!
	Pg::Math::PGQuaternion PGConvertD3DQuatRotToPhysX(Pg::Math::PGQuaternion val);
	Pg::Math::PGQuaternion PGConvertPhysXQuatRotToD3D(Pg::Math::PGQuaternion val);

	//벡터의 길이의 제곱 리턴.
	float PGFloat3LengthSquared(Pg::Math::PGFLOAT3 val);
	
	Pg::Math::PGFLOAT3 GetRightVectorFromQuat(Pg::Math::PGQuaternion val);
	Pg::Math::PGFLOAT3 GetForwardVectorFromQuat(Pg::Math::PGQuaternion val);
	Pg::Math::PGFLOAT3 GetUpVectorFromQuat(Pg::Math::PGQuaternion val);
	Pg::Math::PGQuaternion PGQuaternionInverse(Pg::Math::PGQuaternion val);
}





