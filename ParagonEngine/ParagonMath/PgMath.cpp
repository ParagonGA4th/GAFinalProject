#include "PgMath.h"
#include <cmath>

namespace Pg::Math
{
	//PGFLOAT2
	constexpr PGFLOAT2::PGFLOAT2(float _x, float _y) noexcept
		: x(_x), y(_y)
	{
		//
	}

	constexpr Pg::Math::PGFLOAT2 PGFLOAT2::operator-(const PGFLOAT2& rhs) const noexcept
	{
		return { x - rhs.x, y - rhs.y };
	}

	constexpr Pg::Math::PGFLOAT2 PGFLOAT2::operator*(float rhs) const noexcept
	{
		return { x * rhs,y * rhs };
	}

	constexpr Pg::Math::PGFLOAT2 PGFLOAT2::operator+(const PGFLOAT2& rhs) const noexcept
	{
		return { x + rhs.x, y + rhs.y };
	}

	//PGFLOAT3
	constexpr PGFLOAT3::PGFLOAT3(float _x, float _y, float _z) noexcept
		: x(_x), y(_y), z(_z)
	{
		//
	}

	Pg::Math::PGFLOAT3 PGFLOAT3::operator-(const PGFLOAT3& rhs) const
	{
		return PGFLOAT3(x - rhs.x, y - rhs.y, z - rhs.z);
	}

	Pg::Math::PGFLOAT3 PGFLOAT3::operator*(float rhs) const
	{
		return PGFLOAT3(x * rhs, y * rhs, z * rhs);
	}

	float PGFLOAT3::operator*(PGFLOAT3 rhs) const
	{
		return (x * rhs.x + y * rhs.y + z * rhs.z);
	}

	Pg::Math::PGFLOAT3& PGFLOAT3::operator+=(const PGFLOAT3& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}

	Pg::Math::PGFLOAT3 PGFLOAT3::operator+(const PGFLOAT3& rhs)
	{
		return PGFLOAT3(x + rhs.x, y + rhs.y, z + rhs.z);
	}

	bool PGFLOAT3::operator<(const PGFLOAT3& rhs) const
	{
		return
		{
			x < rhs.x
			&& y < rhs.y
			&& z < rhs.z
		};
	}

	bool PGFLOAT3::operator>(const PGFLOAT3& rhs) const
	{
		return
		{
			x > rhs.x
			&& y > rhs.y
			&& z > rhs.z
		};
	}

	bool PGFLOAT3::operator>=(const PGFLOAT3& rhs) const
	{
		return
		{
			x >= rhs.x
			&& y >= rhs.y
			&& z >= rhs.z
		};
	}

	bool PGFLOAT3::operator<=(const PGFLOAT3& rhs) const
	{
		return
		{
			x <= rhs.x
			&& y <= rhs.y
			&& z <= rhs.z
		};
	}

	//PGFLOAT4

	PGFLOAT4::PGFLOAT4()
		: x(0.0f), y(0.0f), z(0.0f), w(0.0f)
	{
		//
	}

	constexpr PGFLOAT4::PGFLOAT4(float _x, float _y, float _z, float _w) noexcept
		: x(_x), y(_y), z(_z), w(_w)
	{
		r = x;
		g = y;
		b = z;
		a = w;
	}

	constexpr PGFLOAT4::PGFLOAT4(const PGFLOAT3& rhs, float _w) noexcept
		: x(rhs.x), y(rhs.y), z(rhs.z), w(_w)
	{
		r = x;
		g = y;
		b = z;
		a = w;
	}

	bool PGFLOAT4::operator<(const PGFLOAT4& rhs) const
	{
		return
		{
			x < rhs.x
			&& y < rhs.y
			&& z < rhs.z
		};
	}

	bool PGFLOAT4::operator>(const PGFLOAT4& rhs) const
	{
		return
		{
			x > rhs.x
			&& y > rhs.y
			&& z > rhs.z
		};
	}

	bool PGFLOAT4::operator>=(const PGFLOAT4& rhs) const
	{
		return
		{
			x >= rhs.x
			&& y >= rhs.y
			&& z >= rhs.z
		};
	}

	bool PGFLOAT4::operator<=(const PGFLOAT4& rhs) const
	{
		return
		{
			x <= rhs.x
			&& y <= rhs.y
			&& z <= rhs.z
		};
	}

	Pg::Math::PGFLOAT4& PGFLOAT4::operator*=(const PGFLOAT4X4& rhs)
	{
		PGFLOAT4 temp;

		temp.x = x * rhs.m[0][0] + y * rhs.m[1][0] + z * rhs.m[2][0] + w * rhs.m[3][0];
		temp.y = x * rhs.m[0][1] + y * rhs.m[1][1] + z * rhs.m[2][1] + w * rhs.m[3][1];
		temp.z = x * rhs.m[0][2] + y * rhs.m[1][2] + z * rhs.m[2][2] + w * rhs.m[3][2];
		temp.w = x * rhs.m[0][3] + y * rhs.m[1][3] + z * rhs.m[2][3] + w * rhs.m[3][3];

		*this = temp;
		return *this;
	}

	Pg::Math::PGFLOAT4 PGFLOAT4::operator*(const PGFLOAT4X4& rhs)
	{
		PGFLOAT4 temp;

		temp.x = x * rhs.m[0][0] + y * rhs.m[1][0] + z * rhs.m[2][0] + w * rhs.m[3][0];
		temp.y = x * rhs.m[0][1] + y * rhs.m[1][1] + z * rhs.m[2][1] + w * rhs.m[3][1];
		temp.z = x * rhs.m[0][2] + y * rhs.m[1][2] + z * rhs.m[2][2] + w * rhs.m[3][2];
		temp.w = x * rhs.m[0][3] + y * rhs.m[1][3] + z * rhs.m[2][3] + w * rhs.m[3][3];

		return temp;
	}

	Pg::Math::PGFLOAT4 PGFLOAT4::operator+(const PGFLOAT4& rhs)
	{
		PGFLOAT4 result;
		result.x = x + rhs.x;
		result.y = y + rhs.y;
		result.z = z + rhs.z;
		result.w = w + rhs.w;

		return result;
	}

	Pg::Math::PGFLOAT4 PGFLOAT4::operator-(const PGFLOAT4& rhs)
	{
		PGFLOAT4 result;
		result.x = x - rhs.x;
		result.y = y - rhs.y;
		result.z = z - rhs.z;
		result.w = w - rhs.w;

		return result;
	}

	//PGQuaternion

	PGQuaternion::PGQuaternion() : w(0.0f), x(0.0f), y(0.0f), z(0.0f)
	{
		//
	}

	constexpr PGQuaternion::PGQuaternion(float _w, float _x, float _y, float _z) noexcept
		:w(_w), x(_x), y(_y), z(_z)
	{
		//
	}

	Pg::Math::PGQuaternion PGQuaternion::Conjugate() const
	{
		return PGQuaternion(w, -x, -y, -z);
	}

	//PGFLOAT4X4
	constexpr PGFLOAT4X4::PGFLOAT4X4() noexcept
		: _11(0), _12(0), _13(0), _14(0),
		_21(0), _22(0), _23(0), _24(0),
		_31(0), _32(0), _33(0), _34(0),
		_41(0), _42(0), _43(0), _44(0)
	{
		//
	}

	constexpr PGFLOAT4X4::PGFLOAT4X4(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33) noexcept
		: _11(m00), _12(m01), _13(m02), _14(m03),
		_21(m10), _22(m11), _23(m12), _24(m13),
		_31(m20), _32(m21), _33(m22), _34(m23),
		_41(m30), _42(m31), _43(m32), _44(m33)
	{
		//
	}

	constexpr PGFLOAT4X4::PGFLOAT4X4(float mArr[16]) noexcept :
		_11(mArr[0]), _12(mArr[1]), _13(mArr[2]), _14(mArr[3]),
		_21(mArr[4]), _22(mArr[5]), _23(mArr[6]), _24(mArr[7]),
		_31(mArr[8]), _32(mArr[9]), _33(mArr[10]), _34(mArr[11]),
		_41(mArr[12]), _42(mArr[13]), _43(mArr[14]), _44(mArr[15])
	{

	}

	Pg::Math::PGFLOAT4X4 PGFLOAT4X4::operator*(const PGFLOAT4X4& rhs)
	{
		PGFLOAT4X4 result;

		for (int row = 0; row < 4; row++)
		{
			for (int col = 0; col < 4; col++)
			{
				result.m[row][col] =
					m[row][0] * rhs.m[0][col] +
					m[row][1] * rhs.m[1][col] +
					m[row][2] * rhs.m[2][col] +
					m[row][3] * rhs.m[3][col];
			}
		}

		return result;
	}

	Pg::Math::PGFLOAT4X4& PGFLOAT4X4::operator*=(const PGFLOAT4X4& rhs)
	{
		PGFLOAT4X4 temp;

		for (int row = 0; row < 4; row++)
		{
			for (int col = 0; col < 4; col++)
			{
				temp.m[row][col] =
					m[row][0] * rhs.m[0][col] +
					m[row][1] * rhs.m[1][col] +
					m[row][2] * rhs.m[2][col] +
					m[row][3] * rhs.m[3][col];
			}
		}

		*this = temp;
		return *this;
	}

	float& PGFLOAT4X4::operator()(size_t row, size_t column) noexcept
	{
		return m[row][column];
	}

	Pg::Math::PGFLOAT4X4 PGFLOAT4X4::Identity()
	{
		return
		{
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};
	}

	Pg::Math::PGFLOAT4X4 PGFLOAT4X4::Inverse() const
	{
		Pg::Math::PGFLOAT4X4 inv;

		float det =
			_11 * (_22 * (_33 * _44 - _34 * _43) - _23 * (_32 * _44 - _34 * _42) + _24 * (_32 * _43 - _33 * _42)) -
			_12 * (_21 * (_33 * _44 - _34 * _43) - _23 * (_31 * _44 - _34 * _41) + _24 * (_31 * _43 - _33 * _41)) +
			_13 * (_21 * (_32 * _44 - _34 * _42) - _22 * (_31 * _44 - _34 * _41) + _24 * (_31 * _42 - _32 * _41)) -
			_14 * (_21 * (_32 * _43 - _33 * _42) - _22 * (_31 * _43 - _33 * _41) + _23 * (_31 * _42 - _32 * _41));

		if (std::abs(det) < 1e-8)
		{
			// 행렬식이 0에 가까우면 역행렬이 존재하지 않습니다.
			return Pg::Math::PGFLOAT4X4::Identity();
		}

		float invDet = 1.0f / det;

		inv._11 = (_22 * (_33 * _44 - _34 * _43) - _23 * (_32 * _44 - _34 * _42) + _24 * (_32 * _43 - _33 * _42)) * invDet;
		inv._12 = -(_12 * (_33 * _44 - _34 * _43) - _13 * (_32 * _44 - _34 * _42) + _14 * (_32 * _43 - _33 * _42)) * invDet;
		inv._13 = (_12 * (_23 * _44 - _24 * _43) - _13 * (_22 * _44 - _24 * _42) + _14 * (_22 * _43 - _23 * _42)) * invDet;
		inv._14 = -(_12 * (_23 * _34 - _24 * _33) - _13 * (_22 * _34 - _24 * _32) + _14 * (_22 * _33 - _23 * _32)) * invDet;
		inv._21 = -(_21 * (_33 * _44 - _34 * _43) - _23 * (_31 * _44 - _34 * _41) + _24 * (_31 * _43 - _33 * _41)) * invDet;
		inv._22 = (_11 * (_33 * _44 - _34 * _43) - _13 * (_31 * _44 - _34 * _41) + _14 * (_31 * _43 - _33 * _41)) * invDet;
		inv._23 = -(_11 * (_23 * _44 - _24 * _43) - _13 * (_21 * _44 - _24 * _41) + _14 * (_21 * _43 - _23 * _41)) * invDet;
		inv._24 = (_11 * (_23 * _34 - _24 * _33) - _13 * (_21 * _34 - _24 * _31) + _14 * (_21 * _33 - _23 * _31)) * invDet;
		inv._31 = (_21 * (_32 * _44 - _34 * _42) - _22 * (_31 * _44 - _34 * _41) + _24 * (_31 * _42 - _32 * _41)) * invDet;
		inv._32 = -(_11 * (_32 * _44 - _34 * _42) - _12 * (_31 * _44 - _34 * _41) + _14 * (_31 * _42 - _32 * _41)) * invDet;
		inv._33 = (_11 * (_22 * _44 - _24 * _42) - _12 * (_21 * _44 - _24 * _41) + _14 * (_21 * _42 - _22 * _41)) * invDet;
		inv._34 = -(_11 * (_22 * _34 - _24 * _32) - _12 * (_21 * _34 - _24 * _31) + _14 * (_21 * _32 - _22 * _31)) * invDet;
		inv._41 = -(_21 * (_32 * _43 - _33 * _42) - _22 * (_31 * _43 - _33 * _41) + _23 * (_31 * _42 - _32 * _41)) * invDet;
		inv._42 = (_11 * (_32 * _43 - _33 * _42) - _12 * (_31 * _43 - _33 * _41) + _13 * (_31 * _42 - _32 * _41)) * invDet;
		inv._43 = -(_11 * (_22 * _43 - _23 * _42) - _12 * (_21 * _43 - _23 * _41) + _13 * (_21 * _42 - _22 * _41)) * invDet;
		inv._44 = (_11 * (_22 * _33 - _23 * _32) - _12 * (_21 * _33 - _23 * _31) + _13 * (_21 * _32 - _22 * _31)) * invDet;

		return inv;
	}

	//PGFLOAT3X3

	PGFLOAT3X3::PGFLOAT3X3()
	{
		_11 = 0.0f;
		_12 = 0.0f;
		_13 = 0.0f;
		_21 = 0.0f;
		_22 = 0.0f;
		_23 = 0.0f;
		_31 = 0.0f;
		_32 = 0.0f;
		_33 = 0.0f;
	}

	constexpr PGFLOAT3X3::PGFLOAT3X3(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22) noexcept
		: _11(m00), _12(m01), _13(m02),
		_21(m10), _22(m11), _23(m12),
		_31(m20), _32(m21), _33(m22)
	{
		//
	}

	Pg::Math::PGFLOAT3X3 PGFLOAT3X3::Identity()
	{
		return PGFLOAT3X3
		(
			1, 0, 0,
			0, 1, 0,
			0, 0, 1
		);
	}

	Pg::Math::PGFLOAT3X3 PGFLOAT3X3::operator*(const PGFLOAT3X3& rhs)
	{
		PGFLOAT3X3 result;

		for (int row = 0; row < 3; row++)
		{
			for (int col = 0; col < 3; col++)
			{
				result.m[row][col] =
					m[row][0] * rhs.m[0][col] +
					m[row][1] * rhs.m[1][col] +
					m[row][2] * rhs.m[2][col];
			}
		}

		return result;
	}

	Pg::Math::PGFLOAT3X3& PGFLOAT3X3::operator*=(const PGFLOAT3X3& rhs)
	{
		PGFLOAT3X3 result;
		for (int row = 0; row < 3; row++)
		{
			for (int col = 0; col < 3; col++)
			{
				result.m[row][col] =
					m[row][0] * rhs.m[0][col] +
					m[row][1] * rhs.m[1][col] +
					m[row][2] * rhs.m[2][col];
			}
		}
		*this = result;
		return *this;
	}

	inline float PGFloat3Length(const PGFLOAT3& f) noexcept
	{
		return sqrtf(f.x * f.x + f.y * f.y + f.z * f.z);
	}

	inline Pg::Math::PGFLOAT3 PGFloat3Normalize(const PGFLOAT3& f) noexcept
	{
		float len = PGFloat3Length(f);
		return Pg::Math::PGFLOAT3(f.x / len, f.y / len, f.z / len);
	}

	inline Pg::Math::PGFLOAT4 PGFloat4Normalize(const PGFLOAT4& f) noexcept
	{
		float length = std::sqrt(f.x * f.x + f.y * f.y + f.z * f.z + f.w * f.w);
		return { f.x / length, f.y / length, f.z / length, f.w / length };
	}

	Pg::Math::PGQuaternion PGQuaternionNormalize(const PGQuaternion& f) noexcept
	{
		float length = std::sqrt(f.x * f.x + f.y * f.y + f.z * f.z + f.w * f.w);
		return { f.x / length, f.y / length, f.z / length, f.w / length };
	}

	float PGFloat3Dot(const PGFLOAT3& lhs, const PGFLOAT3& rhs)
	{
		return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
	}

	Pg::Math::PGFLOAT3 PGFloat3Cross(const PGFLOAT3& lhs, const PGFLOAT3& rhs)
	{
		return Pg::Math::PGFLOAT3(lhs.y * rhs.z - lhs.z * rhs.y, lhs.z * rhs.x - lhs.x * rhs.z, lhs.x * rhs.y - lhs.y * rhs.x);
	}

	Pg::Math::PGFLOAT3 PGFloat3MultiplyMatrix(const PGFLOAT3& lhs, const PGFLOAT4X4& rhs)
	{
		Pg::Math::PGFLOAT4 result = PGFloat4MultiplyMatrix({ lhs,1.0f }, rhs);
		return { result.x,result.y,result.z };
	}

	Pg::Math::PGFLOAT4 PGFloat4MultiplyMatrix(const PGFLOAT4& lhs, const PGFLOAT4X4& rhs)
	{
		Pg::Math::PGFLOAT4 result;

		result.x =
			lhs.x * rhs.m[0][0] +
			lhs.y * rhs.m[1][0] +
			lhs.z * rhs.m[2][0] +
			lhs.w * rhs.m[3][0];

		result.y =
			lhs.x * rhs.m[0][1] +
			lhs.y * rhs.m[1][1] +
			lhs.z * rhs.m[2][1] +
			lhs.w * rhs.m[3][1];

		result.z =
			lhs.x * rhs.m[0][2] +
			lhs.y * rhs.m[1][2] +
			lhs.z * rhs.m[2][2] +
			lhs.w * rhs.m[3][2];

		result.w =
			lhs.x * rhs.m[0][3] +
			lhs.y * rhs.m[1][3] +
			lhs.z * rhs.m[2][3] +
			lhs.w * rhs.m[3][3];

		return result;
	}

	Pg::Math::PGQuaternion PGRotateQuaternionY(const PGQuaternion& quaternion, float radian)
	{
		float halfAngleRad = radian * 0.5f;
		float sinHalfAngle = std::sin(halfAngleRad);
		float cosHalfAngle = std::cos(halfAngleRad);

		Pg::Math::PGQuaternion rotation;
		rotation.x = 0.0f;
		rotation.y = sinHalfAngle;
		rotation.z = 0.0f;
		rotation.w = cosHalfAngle;

		// 회전 쿼터니언을 정규화하여 사용
		rotation = PGQuaternionNormalize(rotation);

		// 원본 쿼터니언과 회전 쿼터니언의 곱으로 결과 쿼터니언 계산
		Pg::Math::PGQuaternion result;
		//	result = RMQuaternionMultiply(quaternion, rotation);
		result.x = quaternion.w * rotation.x + quaternion.x * rotation.w + quaternion.y * rotation.z - quaternion.z * rotation.y;
		result.y = quaternion.w * rotation.y - quaternion.x * rotation.z + quaternion.y * rotation.w + quaternion.z * rotation.x;
		result.z = quaternion.w * rotation.z + quaternion.x * rotation.y - quaternion.y * rotation.x + quaternion.z * rotation.w;
		result.w = quaternion.w * rotation.w - quaternion.x * rotation.x - quaternion.y * rotation.y - quaternion.z * rotation.z;

		return result;
	}

	Pg::Math::PGQuaternion PGRotateQuaternion(const PGQuaternion& quaternion, const PGFLOAT3& axis, float radian)
	{
		// 축 벡터를 정규화합니다.
		Pg::Math::PGFLOAT3 normalizedAxis = PGFloat3Normalize(axis);

		// 회전 각도의 반을 구합니다.
		float halfAngleRad = radian * 0.5f;

		// 회전 각도의 부호를 쿼터니언에 반영합니다.
		float cosHalfAngle = std::cos(halfAngleRad);
		float sinHalfAngle = std::sin(halfAngleRad);

		float x = normalizedAxis.x * sinHalfAngle;
		float y = normalizedAxis.y * sinHalfAngle;
		float z = normalizedAxis.z * sinHalfAngle;
		float w = cosHalfAngle;

		// 회전 쿼터니언을 정규화하여 사용합니다.
		Pg::Math::PGQuaternion rotation = { x, y, z, w };
		rotation = PGQuaternionNormalize(rotation);

		// 원본 쿼터니언과 회전 쿼터니언의 곱으로 결과 쿼터니언을 계산합니다.
		Pg::Math::PGQuaternion result;
		//	result = PGQuaternionMultiply(quaternion, rotation);
		result.x = rotation.w * quaternion.x + rotation.x * quaternion.w + rotation.y * quaternion.z - rotation.z * quaternion.y;
		result.y = rotation.w * quaternion.y - rotation.x * quaternion.z + rotation.y * quaternion.w + rotation.z * quaternion.x;
		result.z = rotation.w * quaternion.z + rotation.x * quaternion.y - rotation.y * quaternion.x + rotation.z * quaternion.w;
		result.w = rotation.w * quaternion.w - rotation.x * quaternion.x - rotation.y * quaternion.y - rotation.z * quaternion.z;

		return result;
	}

	Pg::Math::PGQuaternion PGQuaternionMultiply(const PGQuaternion& lhs, const PGQuaternion& rhs)
	{
		Pg::Math::PGQuaternion lhsNorm = PGQuaternionNormalize(lhs);
		Pg::Math::PGQuaternion rhsNorm = PGQuaternionNormalize(rhs);

		// 원본 쿼터니언과 회전 쿼터니언의 곱으로 결과 쿼터니언을 계산합니다.
		Pg::Math::PGQuaternion result;

		result.x = rhsNorm.w * lhsNorm.x + rhsNorm.x * lhsNorm.w + rhsNorm.y * lhsNorm.z - rhsNorm.z * lhsNorm.y;
		result.y = rhsNorm.w * lhsNorm.y - rhsNorm.x * lhsNorm.z + rhsNorm.y * lhsNorm.w + rhsNorm.z * lhsNorm.x;
		result.z = rhsNorm.w * lhsNorm.z + rhsNorm.x * lhsNorm.y - rhsNorm.y * lhsNorm.x + rhsNorm.z * lhsNorm.w;
		result.w = rhsNorm.w * lhsNorm.w - rhsNorm.x * lhsNorm.x - rhsNorm.y * lhsNorm.y - rhsNorm.z * lhsNorm.z;

		// 	result.w = lhsNorm.w * rhsNorm.w - lhsNorm.x * rhsNorm.x - lhsNorm.y * rhsNorm.y - lhsNorm.z * rhsNorm.z;
		// 	result.x = lhsNorm.w * rhsNorm.x + lhsNorm.x * rhsNorm.w + lhsNorm.y * rhsNorm.z - lhsNorm.z * rhsNorm.y;
		// 	result.y = lhsNorm.w * rhsNorm.y - lhsNorm.x * rhsNorm.z + lhsNorm.y * rhsNorm.w + lhsNorm.z * rhsNorm.x;
		// 	result.z = lhsNorm.w * rhsNorm.z + lhsNorm.x * rhsNorm.y - lhsNorm.y * rhsNorm.x + lhsNorm.z * rhsNorm.w;

		result = PGQuaternionNormalize(result);

		return result;
	}

	Pg::Math::PGFLOAT4X4 PGMatrixRotationX(float angle)
	{
		float    fSinAngle = std::sin(angle);
		float    fCosAngle = std::cos(angle);
		//XMScalarSinCos(&fSinAngle, &fCosAngle, Angle);

		Pg::Math::PGFLOAT4X4 M;
		M.m[0][0] = 1.0f;
		M.m[0][1] = 0.0f;
		M.m[0][2] = 0.0f;
		M.m[0][3] = 0.0f;

		M.m[1][0] = 0.0f;
		M.m[1][1] = fCosAngle;
		M.m[1][2] = -fSinAngle;
		M.m[1][3] = 0.0f;

		M.m[2][0] = 0.0f;
		M.m[2][1] = fSinAngle;
		M.m[2][2] = fCosAngle;
		M.m[2][3] = 0.0f;

		M.m[3][0] = 0.0f;
		M.m[3][1] = 0.0f;
		M.m[3][2] = 0.0f;
		M.m[3][3] = 1.0f;

		return M;
	}

	Pg::Math::PGFLOAT4X4 PGMatrixRotationY(float angle)
	{
		float    fSinAngle = std::sin(angle);
		float    fCosAngle = std::cos(angle);
		//XMScalarSinCos(&fSinAngle, &fCosAngle, Angle);

		Pg::Math::PGFLOAT4X4 M;
		M.m[0][0] = fCosAngle;
		M.m[0][1] = 0.0f;
		M.m[0][2] = -fSinAngle;
		M.m[0][3] = 0.0f;

		M.m[1][0] = 0.0f;
		M.m[1][1] = 1.0f;
		M.m[1][2] = 0.0f;
		M.m[1][3] = 0.0f;

		M.m[2][0] = fSinAngle;
		M.m[2][1] = 0.0f;
		M.m[2][2] = fCosAngle;
		M.m[2][3] = 0.0f;

		M.m[3][0] = 0.0f;
		M.m[3][1] = 0.0f;
		M.m[3][2] = 0.0f;
		M.m[3][3] = 1.0f;

		return M;
	}

	Pg::Math::PGFLOAT3X3 PGInverseMatrix(const PGFLOAT3X3& mat)
	{
		// 3x3 행렬의 행렬식 계산
		float det = mat.m[0][0] * (mat.m[1][1] * mat.m[2][2] - mat.m[1][2] * mat.m[2][1]) -
			mat.m[0][1] * (mat.m[1][0] * mat.m[2][2] - mat.m[1][2] * mat.m[2][0]) +
			mat.m[0][2] * (mat.m[1][0] * mat.m[2][1] - mat.m[1][1] * mat.m[2][0]);

		// 행렬식이 0인 경우 역행렬이 존재하지 않음
		if (det == 0)
		{
			return Pg::Math::PGFLOAT3X3::Identity();
		}

		Pg::Math::PGFLOAT3X3 inv;

		// 역행렬 계산
		float invDet = 1.0f / det;
		inv.m[0][0] = (mat.m[1][1] * mat.m[2][2] - mat.m[1][2] * mat.m[2][1]) * invDet;
		inv.m[0][1] = (mat.m[0][2] * mat.m[2][1] - mat.m[0][1] * mat.m[2][2]) * invDet;
		inv.m[0][2] = (mat.m[0][1] * mat.m[1][2] - mat.m[0][2] * mat.m[1][1]) * invDet;
		inv.m[1][0] = (mat.m[1][2] * mat.m[2][0] - mat.m[1][0] * mat.m[2][2]) * invDet;
		inv.m[1][1] = (mat.m[0][0] * mat.m[2][2] - mat.m[0][2] * mat.m[2][0]) * invDet;
		inv.m[1][2] = (mat.m[0][2] * mat.m[1][0] - mat.m[0][0] * mat.m[1][2]) * invDet;
		inv.m[2][0] = (mat.m[1][0] * mat.m[2][1] - mat.m[1][1] * mat.m[2][0]) * invDet;
		inv.m[2][1] = (mat.m[0][1] * mat.m[2][0] - mat.m[0][0] * mat.m[2][1]) * invDet;
		inv.m[2][2] = (mat.m[0][0] * mat.m[1][1] - mat.m[0][1] * mat.m[1][0]) * invDet;

		return inv;
	}

	Pg::Math::PGFLOAT4 PGQuaternionToFloat4(const PGQuaternion& quaternion)
	{
		return { quaternion.x,quaternion.y,quaternion.z,quaternion.w };
	}

	Pg::Math::PGFLOAT4 PGFloat4ToQuaternion(const PGFLOAT4& f4)
	{
		return { f4.x, f4.y, f4.z, f4.w };
	}

	Pg::Math::PGQuaternion PGMatrixToQuaternion(const PGFLOAT4X4& matrix)
	{
		Pg::Math::PGQuaternion quaternion;

		float trace = matrix.m[0][0] + matrix.m[1][1] + matrix.m[2][2];
		if (trace > 0)
		{
			float s = 0.5f / sqrtf(trace + 1.0f);
			quaternion.x = (matrix.m[1][2] - matrix.m[2][1]) * s;
			quaternion.y = (matrix.m[2][0] - matrix.m[0][2]) * s;
			quaternion.z = (matrix.m[0][1] - matrix.m[1][0]) * s;
			quaternion.w = 0.25f / s;
		}
		else
		{
			if (matrix.m[0][0] > matrix.m[1][1] && matrix.m[0][0] > matrix.m[2][2])
			{
				float s = 2.0f * sqrtf(1.0f + matrix.m[0][0] - matrix.m[1][1] - matrix.m[2][2]);
				quaternion.x = 0.25f * s;
				quaternion.y = (matrix.m[1][0] + matrix.m[0][1]) / s;
				quaternion.z = (matrix.m[2][0] + matrix.m[0][2]) / s;
				quaternion.w = (matrix.m[1][2] - matrix.m[2][1]) / s;
			}
			else if (matrix.m[1][1] > matrix.m[2][2])
			{
				float s = 2.0f * sqrtf(1.0f + matrix.m[1][1] - matrix.m[0][0] - matrix.m[2][2]);
				quaternion.x = (matrix.m[1][0] + matrix.m[0][1]) / s;
				quaternion.y = 0.25f * s;
				quaternion.z = (matrix.m[2][1] + matrix.m[1][2]) / s;
				quaternion.w = (matrix.m[2][0] - matrix.m[0][2]) / s;
			}
			else
			{
				float s = 2.0f * sqrtf(1.0f + matrix.m[2][2] - matrix.m[0][0] - matrix.m[1][1]);
				quaternion.x = (matrix.m[2][0] + matrix.m[0][2]) / s;
				quaternion.y = (matrix.m[2][1] + matrix.m[1][2]) / s;
				quaternion.z = 0.25f * s;
				quaternion.w = (matrix.m[0][1] - matrix.m[1][0]) / s;
			}
		}

		return quaternion;
	}

	Pg::Math::PGFLOAT4X4 PGScaleMatrix(const PGFLOAT3 scale)
	{
		Pg::Math::PGFLOAT4X4 scaleMatrix =
		{
			scale.x,	0,		0,		0,
			0,		scale.y,	0,		0,
			0,		0,		scale.z,	0,
			0,		0,		0,			1
		};

		return scaleMatrix;
	}

	Pg::Math::PGFLOAT4X4 PGRotationMatrix(const PGQuaternion rotation)
	{
		Pg::Math::PGFLOAT4X4 rotationMatrix =
		{
			1.0f - 2.0f * (rotation.y * rotation.y + rotation.z * rotation.z),
			2.0f * (rotation.x * rotation.y + rotation.z * rotation.w),
			2.0f * (rotation.x * rotation.z - rotation.y * rotation.w),
			0,

			2.0f * (rotation.x * rotation.y - rotation.z * rotation.w),
			1.0f - 2.0f * (rotation.x * rotation.x + rotation.z * rotation.z),
			2.0f * (rotation.y * rotation.z + rotation.x * rotation.w),
			0,

			2.0f * (rotation.x * rotation.z + rotation.y * rotation.w),
			2.0f * (rotation.y * rotation.z - rotation.x * rotation.w),
			1.0f - 2.0f * (rotation.x * rotation.x + rotation.y * rotation.y),
			0,

			0,
			0,
			0,
			1
		};

		return rotationMatrix;
	}

	Pg::Math::PGFLOAT4X4 PGTranslateMatrix(const PGFLOAT3 position)
	{
		Pg::Math::PGFLOAT4X4 translateMatrix =
		{
			1,				0,				0,				0,
			0,				1,				0,				0,
			0,				0,				1,				0,
			position.x,		position.y,		position.z,	1
		};

		return translateMatrix;
	}

	//												Field Of View Y각			종횡비			Near Z Plane	Far Z Plane
	Pg::Math::PGFLOAT4X4 PGMatrixPerspectiveFovLH(float fovAngleY, float aspectRatio, float nearZ, float farZ)
	{
		//tan(theta/2)의 역수 
		//여기서 theta는 FOV의 Y각이다.
		float scaleY = 1.0f / tanf(fovAngleY / 2.0f);
		float scaleX = scaleY / aspectRatio;

		float tempPers[16] =
		{
			scaleX, 0, 0, 0,
			0, scaleY, 0, 0,
			0, 0, farZ / (farZ - nearZ), 1, //이건 Z Divide 위해! Depth 보존.
			0, 0, -nearZ * farZ / (farZ - nearZ), 0
		};

		return Pg::Math::PGFLOAT4X4(tempPers);
	}

	Pg::Math::PGFLOAT4X4 PGMatrixOrthographicLH(float viewWidth, float viewHeight, float nearZ, float farZ)
	{
		float tempOrtho[16] =
		{
			2.0f / viewWidth, 0, 0, 0,
			0, 2.0f / viewHeight, 0, 0,
			0, 0, 1.0f / farZ - nearZ, 0,
			0, 0, -nearZ / farZ - nearZ, 1
		};
		return Pg::Math::PGFLOAT4X4(tempOrtho);
	}

}

