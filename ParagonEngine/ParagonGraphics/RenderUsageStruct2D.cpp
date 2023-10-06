#include "RenderUsageStruct2D.h"
#include "MathHelper.h"

namespace Pg::Graphics
{
	using Pg::Graphics::Helper::MathHelper;
	using namespace Pg::Math;

	RenderUsageStruct2D::RenderUsageStruct2D() :
		_worldMatrix2D(), _resourcePath2D()
	{
		//
	}

	RenderUsageStruct2D::RenderUsageStruct2D(Pg::Data::RenderTextData rTextData) :
		_worldMatrix2D(), _resourcePath2D()
	{
		//<Transform Copying>
		//2D Scale
		PGFLOAT3X3 scaleMatrix =
		{
			rTextData.scale.x,		0,			0,
			0,				rTextData.scale.y,	0,
			0,						0,			1
		};

		//2D Rotation
		PGQuaternion tRotation = rTextData.rotation;
		PGFLOAT3X3 rotationMatrix =
		{
			1.0f - 2.0f * (tRotation.y * tRotation.y + tRotation.z * tRotation.z),
			2.0f * (tRotation.x * tRotation.y + tRotation.z * tRotation.w),
			0,

			2.0f * (tRotation.x * tRotation.y - tRotation.z * tRotation.w),
			1.0f - 2.0f * (tRotation.x * tRotation.x + tRotation.z * tRotation.z),
			0,

			0,
			0,
			1
		};

		//2D Translation
		PGFLOAT3 tPosition = rTextData.position;
		PGFLOAT3X3 translateMatrix =
		{
			1,				0,				0,
			0,				1,				0,
			tPosition.x,	tPosition.y,	1
		};

		PGFLOAT3X3 tFinal = (scaleMatrix * rotationMatrix) * translateMatrix;
		this->_worldMatrix2D = MathHelper::PG2XM_FLOAT3X3(tFinal);
		//</Transform Copying>

		//<Resource Path Copying>
		this->_resourcePath2D = rTextData.resourcePath;
		//</Resource Path Copying>
	}

	void RenderUsageStruct2D::Clear()
	{
		_worldMatrix2D = {};
		_resourcePath2D = {};
	}

	bool RenderUsageStruct2D::IsResourcePathSame(const std::string& other)
	{
		return (_resourcePath2D.compare(other) == 0);
	}

	bool RenderUsageStruct2D::operator==(const RenderUsageStruct2D& rhs) const
	{
		bool tIsEqual =
			MathHelper::IsEqualXMFloat3X3(_worldMatrix2D, rhs._worldMatrix2D) &&
			(_resourcePath2D == rhs._resourcePath2D);
		return tIsEqual;
	}

	bool RenderUsageStruct2D::operator!=(const RenderUsageStruct2D& rhs) const
	{
		bool tIsEqual =
			MathHelper::IsEqualXMFloat3X3(_worldMatrix2D, rhs._worldMatrix2D) &&
			(_resourcePath2D == rhs._resourcePath2D);
		return !tIsEqual;
	}

	

}