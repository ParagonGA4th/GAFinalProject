#pragma once
#include "../ParagonData/RenderTextData.h"

#include <DirectXMath.h> //GraphicEngine ���������θ� Ȱ��.
#include <string> 

/// <summary>
/// ���� �������� �ʿ��� ���� ���� (2D) / RenderTextData ���, ������Ʈ���� �����Ǿ��!
/// </summary>

namespace Pg::Graphics
{
	struct RenderUsageStruct2D
	{
		friend class RenderObject2D;
	public:
		RenderUsageStruct2D();
		RenderUsageStruct2D(Pg::Data::RenderTextData rTextData);

		void Clear();
		bool IsResourcePathSame(const std::string& other);
		bool operator==(const RenderUsageStruct2D& rhs) const;
		bool operator!=(const RenderUsageStruct2D& rhs) const;

	private:
		DirectX::XMFLOAT3X3 _worldMatrix2D;
		std::string _resourcePath2D;
	};
}