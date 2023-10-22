#pragma once
#include <functional>

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

		//단위 행렬
		static PGFLOAT4X4 Identity();

		// 역행렬
		PGFLOAT4X4 Inverse() const;
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

	inline float PGFloat3Length(const Pg::Math::PGFLOAT3& f) noexcept;
	inline Pg::Math::PGFLOAT3 PGFloat3Normalize(const Pg::Math::PGFLOAT3& f) noexcept;
	inline Pg::Math::PGFLOAT4 PGFloat4Normalize(const Pg::Math::PGFLOAT4& f) noexcept;
	inline Pg::Math::PGQuaternion PGQuaternionNormalize(const Pg::Math::PGQuaternion& f) noexcept;
	constexpr float PGFloat3Dot(const Pg::Math::PGFLOAT3& lhs, const Pg::Math::PGFLOAT3& rhs);
	constexpr Pg::Math::PGFLOAT3 PGFloat3Cross(const Pg::Math::PGFLOAT3& lhs, const Pg::Math::PGFLOAT3& rhs);
	Pg::Math::PGFLOAT3 PGFloat3MultiplyMatrix(const Pg::Math::PGFLOAT3& lhs, const Pg::Math::PGFLOAT4X4& rhs);
	Pg::Math::PGFLOAT4 PGFloat4MultiplyMatrix(const Pg::Math::PGFLOAT4& lhs, const Pg::Math::PGFLOAT4X4& rhs);
	Pg::Math::PGQuaternion PGRotateQuaternionY(const Pg::Math::PGQuaternion& quaternion, float radian);
	Pg::Math::PGQuaternion PGRotateQuaternion(const Pg::Math::PGQuaternion& quaternion, const Pg::Math::PGFLOAT3& axis, float radian);
	Pg::Math::PGQuaternion PGQuaternionMultiply(const Pg::Math::PGQuaternion& lhs, const Pg::Math::PGQuaternion& rhs);
	Pg::Math::PGFLOAT4X4 PGMatrixRotationX(float angle);
	Pg::Math::PGFLOAT4X4 PGMatrixRotationY(float angle);
	Pg::Math::PGFLOAT3X3 PGInverseMatrix(const Pg::Math::PGFLOAT3X3& mat);
	Pg::Math::PGFLOAT4 PGQuaternionToFloat4(const Pg::Math::PGQuaternion& quaternion);
	Pg::Math::PGFLOAT4 PGFloat4ToQuaternion(const Pg::Math::PGFLOAT4& f4);
	Pg::Math::PGQuaternion PGMatrixToQuaternion(const Pg::Math::PGFLOAT4X4& matrix);

	Pg::Math::PGFLOAT4X4 PGScaleMatrix(const Pg::Math::PGFLOAT3 scale);
	Pg::Math::PGFLOAT4X4 PGRotationMatrix(const Pg::Math::PGQuaternion rotation);
	Pg::Math::PGFLOAT4X4 PGTranslateMatrix(const Pg::Math::PGFLOAT3 position);

	Pg::Math::PGFLOAT4X4 PGMatrixPerspectiveFovLH(float fovAngleY, float aspectRatio, float nearZ, float farZ);
	Pg::Math::PGFLOAT4X4 PGMatrixOrthographicLH(float viewWidth, float viewHeight, float nearZ, float farZ);
}





