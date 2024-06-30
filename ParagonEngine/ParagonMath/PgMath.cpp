#include "PgMath.h"
#include <cmath>
#include <limits>
#include <dxtk/SimpleMath.h>

using namespace DirectX;

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

	Pg::Math::PGFLOAT3 PGFLOAT3::GlobalUp()
	{
		return { 0,1,0 };
	}

	Pg::Math::PGFLOAT3 PGFLOAT3::GlobalRight()
	{
		return { 1,0,0 };
	}

	Pg::Math::PGFLOAT3 PGFLOAT3::GlobalForward()
	{
		return { 0,0,1 };
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
		using namespace DirectX;

		XMFLOAT4 tSelf = { x,y,z,w };
		*this = XM2PG_FLOAT4_VECTOR(XMVector4Transform(XMLoadFloat4(&tSelf), PG2XM_MATRIX4X4(rhs)));

		return *this;
	}

	Pg::Math::PGFLOAT4 PGFLOAT4::operator*(const PGFLOAT4X4& rhs)
	{
		XMFLOAT4 tSelf = { x,y,z,w };
		return XM2PG_FLOAT4_VECTOR(XMVector4Transform(XMLoadFloat4(&tSelf), PG2XM_MATRIX4X4(rhs)));
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

	PGQuaternion::PGQuaternion() : w(1.0f), x(0.0f), y(0.0f), z(0.0f)
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
		return XM2PG_MATRIX4X4(XMMatrixMultiply(PG2XM_MATRIX4X4(*this), PG2XM_MATRIX4X4(rhs)));
	}

	Pg::Math::PGFLOAT4X4& PGFLOAT4X4::operator*=(const PGFLOAT4X4& rhs)
	{
		*this = XM2PG_MATRIX4X4(XMMatrixMultiply(PG2XM_MATRIX4X4(*this), PG2XM_MATRIX4X4(rhs)));
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
		XMVECTOR tDet;
		XMMATRIX tInv = XMMatrixInverse(&tDet, PG2XM_MATRIX4X4(*this));

		//만약 Inverse가 불가? -> Identity 반환.
		if (XMVectorGetX(tDet) <= std::numeric_limits<float>::epsilon())
		{
			return PGFLOAT4X4::Identity();
		}

		return XM2PG_MATRIX4X4(tInv);
	}

	Pg::Math::PGFLOAT4X4 PGFLOAT4X4::Transpose() const
	{
		auto tDX = PG2XM_FLOAT4X4(*this);
		XMMATRIX tXMMat = DirectX::XMMatrixTranspose(XMLoadFloat4x4(&tDX));
		return XM2PG_MATRIX4X4(tXMMat);
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
		return XM2PG_MATRIX3X3(XMMatrixMultiply(PG2XM_MATRIX3X3(*this), PG2XM_MATRIX3X3(rhs)));
	}

	Pg::Math::PGFLOAT3X3& PGFLOAT3X3::operator*=(const PGFLOAT3X3& rhs)
	{
		*this = XM2PG_MATRIX3X3(XMMatrixMultiply(PG2XM_MATRIX3X3(*this), PG2XM_MATRIX3X3(rhs)));
		return *this;
	}

	float PGFloat3Length(const PGFLOAT3& f) noexcept
	{
		return XMVectorGetX(XMVector3Length(PG2XM_FLOAT3_VECTOR(f)));
	}

	float PGFloat4Length(const PGFLOAT4& f) noexcept
	{
		return XMVectorGetX(XMVector4Length(PG2XM_FLOAT4_VECTOR(f)));
	}

	Pg::Math::PGFLOAT3 PGFloat3Normalize(const PGFLOAT3& f) noexcept
	{
		return XM2PG_FLOAT3_VECTOR(XMVector3Normalize(PG2XM_FLOAT3_VECTOR(f)));
	}

	Pg::Math::PGFLOAT4 PGFloat4Normalize(const PGFLOAT4& f) noexcept
	{
		return XM2PG_FLOAT4_VECTOR(XMVector4Normalize(PG2XM_FLOAT4_VECTOR(f)));
	}

	Pg::Math::PGQuaternion PGQuaternionNormalize(const PGQuaternion& f) noexcept
	{
		return XM2PG_QUATERNION(XMQuaternionNormalize(PG2XM_QUATERNION_VECTOR(f)));
	}

	float PGFloat3Dot(const PGFLOAT3& lhs, const PGFLOAT3& rhs)
	{
		//복제된 Dot Product 반환.
		return XM2PG_FLOAT3_VECTOR(XMVector3Dot(PG2XM_FLOAT3_VECTOR(lhs), PG2XM_FLOAT3_VECTOR(rhs))).x;
	}

	Pg::Math::PGFLOAT3 PGFloat3Cross(const PGFLOAT3& lhs, const PGFLOAT3& rhs)
	{
		return XM2PG_FLOAT3_VECTOR(XMVector3Cross(PG2XM_FLOAT3_VECTOR(lhs), PG2XM_FLOAT3_VECTOR(rhs)));
	}

	Pg::Math::PGFLOAT3 PGFloat3MultiplyMatrix(const PGFLOAT3& lhs, const PGFLOAT4X4& rhs)
	{
		Pg::Math::PGFLOAT4 result = PGFloat4MultiplyMatrix({ lhs,1.0f }, rhs);
		return { result.x,result.y,result.z };
	}

	Pg::Math::PGFLOAT4 PGFloat4MultiplyMatrix(const PGFLOAT4& lhs, const PGFLOAT4X4& rhs)
	{
		return XM2PG_FLOAT4_VECTOR(XMVector4Transform(PG2XM_FLOAT4_VECTOR(lhs), PG2XM_MATRIX4X4(rhs)));
	}

	Pg::Math::PGQuaternion PGRotateQuaternionY(const PGQuaternion& quaternion, float radian)
	{
		return PGRotateQuaternion(quaternion, { 0.f, 1.f,0.f }, radian);
	}

	Pg::Math::PGQuaternion PGRotateQuaternion(const PGQuaternion& quaternion, const PGFLOAT3& axis, float radian)
	{
		XMVECTOR tPre = XMQuaternionRotationAxis(PG2XM_FLOAT3_VECTOR(axis), radian);
		return XM2PG_QUATERNION(XMQuaternionMultiply(PG2XM_QUATERNION_VECTOR(quaternion), tPre));
	}

	Pg::Math::PGQuaternion PGQuaternionMultiply(const PGQuaternion& lhs, const PGQuaternion& rhs)
	{
		return XM2PG_QUATERNION(XMQuaternionMultiply(PG2XM_QUATERNION_VECTOR(lhs), PG2XM_QUATERNION_VECTOR(rhs)));
	}

	Pg::Math::PGFLOAT4X4 PGMatrixRotationX(float angle)
	{
		return XM2PG_MATRIX4X4(XMMatrixRotationX(angle));
	}

	Pg::Math::PGFLOAT4X4 PGMatrixRotationY(float angle)
	{
		return XM2PG_MATRIX4X4(XMMatrixRotationY(angle));
	}

	Pg::Math::PGFLOAT3X3 PGInverseMatrix(const PGFLOAT3X3& mat)
	{
		XMVECTOR tDet;
		XMMATRIX tInv = XMMatrixInverse(&tDet, PG2XM_MATRIX3X3(mat));

		//만약 Inverse가 불가? -> Identity 반환.
		if (XMVectorGetX(tDet) <= std::numeric_limits<float>::epsilon())
		{
			return PGFLOAT3X3::Identity();
		}

		return XM2PG_MATRIX3X3(tInv);
	}

	Pg::Math::PGFLOAT4 PGQuaternionToFloat4(const PGQuaternion& quaternion)
	{
		return { quaternion.x,quaternion.y,quaternion.z,quaternion.w };
	}

	Pg::Math::PGQuaternion PGFloat4ToQuaternion(const PGFLOAT4& f4)
	{
		return { f4.w, f4.x, f4.y, f4.z };
	}

	Pg::Math::PGQuaternion PGMatrixToQuaternion(const PGFLOAT4X4& matrix)
	{
		return XM2PG_QUATERNION(XMQuaternionRotationMatrix(PG2XM_MATRIX4X4(matrix)));
	}

	Pg::Math::PGFLOAT4X4 PGScaleMatrix(const PGFLOAT3 scale)
	{
		return XM2PG_MATRIX4X4(XMMatrixScaling(scale.x, scale.y, scale.z));
	}

	Pg::Math::PGFLOAT4X4 PGRotationMatrix(const PGQuaternion rotation)
	{
		XMFLOAT4 tXMFF = PG2XM_QUATERNION(rotation);
		XMVECTOR tXMQuat = XMLoadFloat4(&tXMFF);
		XMMATRIX tMat = XMMatrixRotationQuaternion(tXMQuat);

		return XM2PG_MATRIX4X4(tMat);
	}

	Pg::Math::PGFLOAT4X4 PGTranslateMatrix(const PGFLOAT3 position)
	{
		XMFLOAT4X4 tRet;
		XMStoreFloat4x4(&tRet, XMMatrixTranslation(position.x, position.y, position.z));
		return XM2PG_FLOAT4X4(tRet);
	}

	Pg::Math::PGFLOAT4X4 PGGetWorldMatrixFromValues(const PGFLOAT3 position, const PGQuaternion rotation, const PGFLOAT3 scale)
	{
		XMMATRIX tPosMat = XMMatrixTranslation(position.x, position.y, position.z);

		XMFLOAT4 tXMFF = PG2XM_QUATERNION(rotation);
		XMVECTOR tXMQuat = XMLoadFloat4(&tXMFF);
		XMMATRIX tRotMat = XMMatrixRotationQuaternion(tXMQuat);

		XMMATRIX tSclMat = XMMatrixScaling(scale.x, scale.y, scale.z);

		return XM2PG_MATRIX4X4(XMMatrixMultiply(XMMatrixMultiply(tSclMat, tRotMat), tPosMat));
	}

	//												Field Of View Y각			종횡비			Near Z Plane	Far Z Plane
	Pg::Math::PGFLOAT4X4 PGMatrixPerspectiveFovLH(float fovAngleY, float aspectRatio, float nearZ, float farZ)
	{
		XMFLOAT4X4 tRet;
		XMStoreFloat4x4(&tRet, XMMatrixPerspectiveFovLH(fovAngleY, aspectRatio, nearZ, farZ));
		return XM2PG_FLOAT4X4(tRet);
	}

	Pg::Math::PGFLOAT4X4 PGMatrixOrthographicLH(float viewWidth, float viewHeight, float nearZ, float farZ)
	{
		XMFLOAT4X4 tRet;
		XMStoreFloat4x4(&tRet, XMMatrixOrthographicLH(viewWidth, viewHeight, nearZ, farZ));
		return XM2PG_FLOAT4X4(tRet);
	}

	Pg::Math::PGQuaternion PGEulerToQuaternion(float x, float y, float z)
	{
		XMFLOAT3 tVal = { x,y,z };
		DirectX::SimpleMath::Quaternion tQuat = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(tVal);
		return XM2PG_QUATERNION(tQuat);
	}

	Pg::Math::PGQuaternion PGEulerToQuaternion(const Pg::Math::PGFLOAT3& euler)
	{
		return PGEulerToQuaternion(euler.x, euler.y, euler.z);
	}

	Pg::Math::PGFLOAT3 PGQuaternionToEuler(const Pg::Math::PGQuaternion& quaternion)
	{
		return PGQuaternionToEuler(quaternion.w, quaternion.x, quaternion.y, quaternion.z);
	}

	Pg::Math::PGFLOAT3 PGQuaternionToEuler(float w, float x, float y, float z)
	{
		DirectX::SimpleMath::Quaternion tQuat = { x,y,z,w };
		return XM2PG_FLOAT3(tQuat.ToEuler());
		//XMMATRIX rotationMatrix = XMMatrixRotationQuaternion(XMLoadFloat4(&tQuat));
		//
		//// Extracting the euler angles from the rotation matrix
		//float pitch, yaw, roll;
		//XMVECTOR rotation = XMQuaternionRotationMatrix(rotationMatrix);
		//
		//XMFLOAT3 tRetPre;
		//XMStoreFloat3(&tRetPre, rotation);
		//
		//return XM2PG_FLOAT3(tRetPre);
		//tQuat.CreateFromYawPitchRoll()
	}

	Pg::Math::PGFLOAT3 PGRotateVectorAroundAxis(Pg::Math::PGFLOAT3 vecToRotate, Pg::Math::PGFLOAT3 rotAxis, float angleInRad)
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

	Pg::Math::PGFLOAT2 XM2PG_FLOAT2(const DirectX::XMFLOAT2& val)
	{
		return { val.x, val.y };
	}

	Pg::Math::PGFLOAT3 XM2PG_FLOAT3(const DirectX::XMFLOAT3& val)
	{
		return { val.x, val.y, val.z };
	}

	Pg::Math::PGFLOAT3 XM2PG_FLOAT3_VECTOR(const DirectX::XMVECTOR& val)
	{
		XMFLOAT3 tVal;
		XMStoreFloat3(&tVal, val);
		return { tVal.x, tVal.y, tVal.z };
	}

	Pg::Math::PGFLOAT4 XM2PG_FLOAT4(const DirectX::XMFLOAT4& val)
	{
		return { val.x, val.y, val.z, val.w };
	}

	Pg::Math::PGFLOAT4 XM2PG_FLOAT4_VECTOR(const DirectX::XMVECTOR& val)
	{
		XMFLOAT4 tVal;
		XMStoreFloat4(&tVal, val);
		return { tVal.x, tVal.y, tVal.z, tVal.w };
	}

	Pg::Math::PGQuaternion XM2PG_QUATERNION(const DirectX::XMFLOAT4& val)
	{
		return { val.w, val.x, val.y, val.z };
	}

	Pg::Math::PGQuaternion XM2PG_QUATERNION(const DirectX::XMVECTOR& val)
	{
		DirectX::XMFLOAT4 tVal;
		XMStoreFloat4(&tVal, val);
		return { tVal.w, tVal.x, tVal.y, tVal.z };
	}

	Pg::Math::PGFLOAT3X3 XM2PG_FLOAT3X3(const DirectX::XMFLOAT3X3& val)
	{
		Pg::Math::PGFLOAT3X3 tRet;
		memcpy(&tRet, &val, sizeof(DirectX::XMFLOAT3X3));
		return tRet;
	}

	Pg::Math::PGFLOAT3X3 XM2PG_MATRIX3X3(const DirectX::XMMATRIX& val)
	{
		XMFLOAT3X3 tVal;
		XMStoreFloat3x3(&tVal, val);

		Pg::Math::PGFLOAT3X3 tRet;
		memcpy(&tRet, &tVal, sizeof(DirectX::XMFLOAT3X3));
		return tRet;
	}

	Pg::Math::PGFLOAT4X4 XM2PG_FLOAT4X4(const DirectX::XMFLOAT4X4& val)
	{
		Pg::Math::PGFLOAT4X4 tRet;
		memcpy(&tRet, &val, sizeof(DirectX::XMFLOAT4X4));
		return tRet;
	}

	Pg::Math::PGFLOAT4X4 XM2PG_MATRIX4X4(const DirectX::XMMATRIX& val)
	{
		XMFLOAT4X4 tVal;
		XMStoreFloat4x4(&tVal, val);

		Pg::Math::PGFLOAT4X4 tRet;
		memcpy(&tRet, &tVal, sizeof(DirectX::XMFLOAT4X4));
		return tRet;
	}

	DirectX::XMFLOAT2 PG2XM_FLOAT2(const PGFLOAT2& val)
	{
		return { val.x, val.y };
	}

	DirectX::XMFLOAT3 PG2XM_FLOAT3(const PGFLOAT3& val)
	{
		return { val.x, val.y, val.z };
	}

	DirectX::XMVECTOR PG2XM_FLOAT3_VECTOR(const PGFLOAT3& val)
	{
		DirectX::XMFLOAT3 tVal = { val.x, val.y, val.z };
		return XMLoadFloat3(&tVal);
	}

	DirectX::XMFLOAT4 PG2XM_FLOAT4(const PGFLOAT4& val)
	{
		return { val.x, val.y, val.z, val.w };
	}

	DirectX::XMVECTOR PG2XM_FLOAT4_VECTOR(const PGFLOAT4& val)
	{
		DirectX::XMFLOAT4 tVal = { val.x, val.y, val.z, val.w };
		return XMLoadFloat4(&tVal);
	}

	DirectX::XMFLOAT4 PG2XM_QUATERNION(const PGQuaternion& val)
	{
		return { val.x, val.y, val.z, val.w };
	}

	DirectX::XMVECTOR PG2XM_QUATERNION_VECTOR(const PGQuaternion& val)
	{
		XMFLOAT4 tVal = { val.x, val.y, val.z, val.w };
		return XMLoadFloat4(&tVal);
	}

	DirectX::XMFLOAT3X3 PG2XM_FLOAT3X3(const PGFLOAT3X3& val)
	{
		DirectX::XMFLOAT3X3 tRet;
		memcpy(&tRet, &val, sizeof(DirectX::XMFLOAT3X3));
		return tRet;
	}

	DirectX::XMFLOAT4X4 PG2XM_FLOAT4X4(const PGFLOAT4X4& val)
	{
		DirectX::XMFLOAT4X4 tRet;
		memcpy(&tRet, &val, sizeof(DirectX::XMFLOAT4X4));
		return tRet;
	}

	DirectX::XMMATRIX PG2XM_MATRIX3X3(const PGFLOAT3X3& val)
	{
		DirectX::XMFLOAT3X3 tRet;
		memcpy(&tRet, &val, sizeof(DirectX::XMFLOAT3X3));
		return XMLoadFloat3x3(&tRet);
	}

	DirectX::XMMATRIX PG2XM_MATRIX4X4(const PGFLOAT4X4& val)
	{
		DirectX::XMFLOAT4X4 tRet;
		memcpy(&tRet, &val, sizeof(DirectX::XMFLOAT4X4));
		return XMLoadFloat4x4(&tRet);
	}

	Pg::Math::PGQuaternion FromToRotation(const PGFLOAT3& fromDirection, const PGFLOAT3& toDirection)
	{
		using namespace DirectX;

		//Quaternion
		//DXTK 기반, 방향 뒤집힐 때 막아버린 구현체.
		XMFLOAT4 result;

		//DX 자료형 이동.
		XMVECTOR fromDir = PG2XM_FLOAT3_VECTOR(fromDirection);
		XMVECTOR toDir = PG2XM_FLOAT3_VECTOR(toDirection);

		const XMVECTOR F = XMVector3Normalize(fromDir);
		const XMVECTOR T = XMVector3Normalize(toDir);

		const float dot = XMVectorGetX(XMVector3Dot(F, T));
		if (dot >= 1.f)
		{
			//Identity.
			result = { 0,0,0,1 };
		}
		else if (dot <= -1.f)
		{
			XMFLOAT3 tRight = { 1,0,0 };
			XMVECTOR tRightVec = XMLoadFloat3(&tRight);

			XMVECTOR axis = XMVector3Cross(F, tRightVec);
			if (XMVector3NearEqual(XMVector3LengthSq(axis), g_XMZero, g_XMEpsilon))
			{
				XMFLOAT3 tUp = { 0,1,0 };
				XMVECTOR tUpVec = XMLoadFloat3(&tUp);

				axis = XMVector3Cross(F, tUpVec);
			}

			const XMVECTOR Q = XMQuaternionRotationAxis(axis, XM_PI);
			XMStoreFloat4(&result, Q);
		}
		else
		{
			const XMVECTOR C = XMVector3Cross(F, T);
			XMStoreFloat4(&result, C);

			const float s = sqrtf((1.f + dot) * 2.f);
			result.x /= s;
			result.y /= s;
			result.z /= s;
			result.w = s * 0.5f;
		}

		return XM2PG_QUATERNION(result);

	}

	Pg::Math::PGQuaternion PGLookRotation(const Pg::Math::PGFLOAT3& forward, const Pg::Math::PGFLOAT3& up)
	{
		//PGQuaternion q1;
		////GlobalForward 
		//q1 = FromToRotation({0,0,1}, forward);
		//
		//const XMVECTOR C = XMVector3Cross(PG2XM_FLOAT3_VECTOR(forward), PG2XM_FLOAT3_VECTOR(up));
		//
		//if (XMVector3NearEqual(XMVector3LengthSq(C), g_XMZero, g_XMEpsilon))
		//{
		//	// forward and up are co-linear
		//	result = q1;
		//	return;
		//}
		//
		//XMFLOAT3 tgUp = { 0,1,0 };
		//XMVECTOR tGlobalUp = XMLoadFloat3(&tgUp);
		//const XMVECTOR U = XMQuaternionMultiply(PG2XM_QUATERNION_VECTOR(q1), tGlobalUp);
		//
		//PGQuaternion q2;
		//FromToRotation(U, up, );
		//
		//XMStoreFloat4(&result, XMQuaternionMultiply(q2, q1));

		DirectX::SimpleMath::Quaternion tQuat = DirectX::SimpleMath::Quaternion::LookRotation(PG2XM_FLOAT3_VECTOR(forward), PG2XM_FLOAT3_VECTOR(up));
		return XM2PG_QUATERNION(tQuat);
	}

	Pg::Math::PGQuaternion PGQuaternionSlerp(PGQuaternion a, PGQuaternion b, float t)
	{
		using namespace DirectX::SimpleMath;
		Quaternion tA = PG2XM_QUATERNION_VECTOR(a);
		Quaternion tB = PG2XM_QUATERNION_VECTOR(b);
		return XM2PG_QUATERNION(Quaternion::Slerp(tA, tB, t));
	}

	Pg::Math::PGFLOAT3 PGFloat3Lerp(const PGFLOAT3& a, const PGFLOAT3& b, float t)
	{
		DirectX::XMVECTOR tA = PG2XM_FLOAT3_VECTOR(a);
		DirectX::XMVECTOR tB = PG2XM_FLOAT3_VECTOR(b);
		return XM2PG_FLOAT3_VECTOR(DirectX::XMVectorLerp(tA, tB, t));
	}

	Pg::Math::PGFLOAT3 PGReflectVectorAgainstAxis(const PGFLOAT3& toFlip, const PGFLOAT3& baseAxis)
	{
		using namespace DirectX;
		XMMATRIX reflectionMatrix = XMMatrixReflect(PG2XM_FLOAT3_VECTOR(baseAxis));
		XMVECTOR reflectedVector = XMVector3TransformCoord(PG2XM_FLOAT3_VECTOR(toFlip), reflectionMatrix);
		return XM2PG_FLOAT3_VECTOR(reflectedVector);
	}

	Pg::Math::PGFLOAT4X4 GetViewMatrixFromTransformValues(PGFLOAT3 right, PGFLOAT3 up, PGFLOAT3 forward, PGFLOAT3 pos)
	{
		PGFLOAT3 R = right;
		PGFLOAT3 U = up;
		PGFLOAT3 L = forward;
		PGFLOAT3 P = pos;

		// Keep camera's axes orthogonal to each other and of unit length.
		L = PGFloat3Normalize(L);
		U = PGFloat3Normalize(PGFloat3Cross(L, R));

		// U, L already ortho-normal, so no need to normalize cross product.
		R = PGFloat3Cross(U, L);

		// Fill in the view matrix entries.
		float x = -PGFloat3Dot(P, R);
		float y = -PGFloat3Dot(P, U);
		float z = -PGFloat3Dot(P, L);

		Pg::Math::PGFLOAT4X4 tRet;

		tRet.m[0][0] = R.x;
		tRet.m[1][0] = R.y;
		tRet.m[2][0] = R.z;
		tRet.m[3][0] = x;

		tRet.m[0][1] = U.x;
		tRet.m[1][1] = U.y;
		tRet.m[2][1] = U.z;
		tRet.m[3][1] = y;

		tRet.m[0][2] = L.x;
		tRet.m[1][2] = L.y;
		tRet.m[2][2] = L.z;
		tRet.m[3][2] = z;

		tRet.m[0][3] = 0.0f;
		tRet.m[1][3] = 0.0f;
		tRet.m[2][3] = 0.0f;
		tRet.m[3][3] = 1.0f;

		return tRet;
	}

	Pg::Math::PGFLOAT3 PGConvertD3DVec3RotToPhysX(PGFLOAT3 val)
	{
		return PGFLOAT3(-val.x, val.y, val.z);
	}

	Pg::Math::PGFLOAT3 PGConvertPhysXVec3RotToD3D(PGFLOAT3 val)
	{
		return PGFLOAT3(-val.x, val.z, val.y);
	}

	Pg::Math::PGQuaternion PGConvertD3DQuatRotToPhysX(PGQuaternion val)
	{
		return PGQuaternion(val.w, -val.x, -val.z, val.y);
	}

	Pg::Math::PGQuaternion PGConvertPhysXQuatRotToD3D(PGQuaternion val)
	{
		return PGQuaternion(val.w, -val.x, val.z, val.y);
	}

	float PGFloat3LengthSquared(PGFLOAT3 val)
	{
		return DirectX::XMVectorGetX(DirectX::XMVector3LengthSq(PG2XM_FLOAT3_VECTOR(val)));
	}

	Pg::Math::PGFLOAT3 GetRightVectorFromQuat(PGQuaternion val)
	{
		PGFLOAT4 result = PGFloat4MultiplyMatrix(PGFLOAT4(1.f, 0.f, 0.f, 0.f), PGRotationMatrix(val));
		return PGFloat3Normalize(PGFLOAT3(result.x, result.y, result.z));
	}

	Pg::Math::PGFLOAT3 GetForwardVectorFromQuat(PGQuaternion val)
	{
		PGFLOAT4 result = PGFloat4MultiplyMatrix(PGFLOAT4(0.f, 0.f, 1.f, 0.f), PGRotationMatrix(val));
		return PGFloat3Normalize(PGFLOAT3(result.x, result.y, result.z));
	}

	Pg::Math::PGFLOAT3 GetUpVectorFromQuat(PGQuaternion val)
	{
		PGFLOAT4 result = PGFloat4MultiplyMatrix(PGFLOAT4(0.f, 1.f, 0.f, 0.f), PGRotationMatrix(val));
		return PGFloat3Normalize(PGFLOAT3(result.x, result.y, result.z));
	}

	Pg::Math::PGQuaternion PGQuaternionInverse(PGQuaternion val)
	{
		DirectX::SimpleMath::Quaternion tOrig = PG2XM_QUATERNION_VECTOR(val);
		DirectX::SimpleMath::Quaternion tQuat;
		tOrig.Inverse(tQuat);
		return XM2PG_QUATERNION(tQuat);
	}
}

