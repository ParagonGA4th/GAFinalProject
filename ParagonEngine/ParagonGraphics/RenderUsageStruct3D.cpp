#include "RenderUsageStruct3D.h"
#include "MathHelper.h"

namespace Pg::Graphics
{
	using Pg::Graphics::Helper::MathHelper;
	using namespace Pg::Math;

	RenderUsageStruct3D::RenderUsageStruct3D() :
		_worldMatrix3D(), _resourcePath3D()
	{
		//
	}

	RenderUsageStruct3D::RenderUsageStruct3D(Pg::Data::RenderTextData rTextData) :
		_worldMatrix3D(), _resourcePath3D()
	{
		//<Transform Copying>
		//3D Scale
		PGFLOAT3 tScale = rTextData.scale;
		PGFLOAT4X4 scaleMatrix =
		{
			tScale.x,	0,		0,		0,
			0,		tScale.y,	0,		0,
			0,		0,		tScale.z,	0,
			0,		0,		0,			1
		};

		//3D Rotation
		PGQuaternion tRotation = rTextData.rotation;
		PGFLOAT4X4 rotationMatrix =
		{
			1.0f - 2.0f * (tRotation.y * tRotation.y + tRotation.z * tRotation.z),
			2.0f * (tRotation.x * tRotation.y + tRotation.z * tRotation.w),
			2.0f * (tRotation.x * tRotation.z - tRotation.y * tRotation.w),
			0,

			2.0f * (tRotation.x * tRotation.y - tRotation.z * tRotation.w),
			1.0f - 2.0f * (tRotation.x * tRotation.x + tRotation.z * tRotation.z),
			2.0f * (tRotation.y * tRotation.z + tRotation.x * tRotation.w),
			0,

			2.0f * (tRotation.x * tRotation.z + tRotation.y * tRotation.w),
			2.0f * (tRotation.y * tRotation.z - tRotation.x * tRotation.w),
			1.0f - 2.0f * (tRotation.x * tRotation.x + tRotation.y * tRotation.y),
			0,

			0,
			0,
			0,
			1
		};

		//3D Translation
		PGFLOAT3 tPosition = rTextData.position;
		PGFLOAT4X4 translateMatrix =
		{
			1,				0,				0,				0,
			0,				1,				0,				0,
			0,				0,				1,				0,
			tPosition.x,	tPosition.y,	tPosition.z,	1
		};

		PGFLOAT4X4 tFinal = (scaleMatrix * rotationMatrix) * translateMatrix;
		this->_worldMatrix3D = MathHelper::PG2XM_FLOAT4X4(tFinal);
		//</Transform Copying>

		//<Resource Path Copying>
		this->_resourcePath3D = rTextData.resourcePath;
		//</Resource Path Copying>
	}

	void RenderUsageStruct3D::Clear()
	{
		_worldMatrix3D = {};
		_resourcePath3D = {};
	}

	bool RenderUsageStruct3D::IsResourcePathSame(const std::string& other)
	{
		return (_resourcePath3D.compare(other) == 0);
	}

	bool RenderUsageStruct3D::operator==(const RenderUsageStruct3D& rhs) const
	{
		bool tIsEqual =
			MathHelper::IsEqualXMFloat4X4(_worldMatrix3D, rhs._worldMatrix3D) &&
			(_resourcePath3D == rhs._resourcePath3D);
		return tIsEqual;
	}

	bool RenderUsageStruct3D::operator!=(const RenderUsageStruct3D& rhs) const
	{
		bool tIsEqual =
			MathHelper::IsEqualXMFloat4X4(_worldMatrix3D, rhs._worldMatrix3D) &&
			(_resourcePath3D == rhs._resourcePath3D);
		return !tIsEqual;
	}
}