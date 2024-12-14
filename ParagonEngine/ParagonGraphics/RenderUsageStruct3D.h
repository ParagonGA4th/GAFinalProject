#pragma once
#include "../ParagonData/RenderTextData.h"

#include <DirectXMath.h> //GraphicEngine ���������θ� Ȱ��.
#include <string> 

/// <summary>
/// ���� �������� �ʿ��� ���� ���� (3D) / RenderTextData ���, ������Ʈ���� �����Ǿ��!
/// </summary>

namespace Pg::Graphics
{
	struct RenderUsageStruct3D
	{
		friend class RenderObject3D;
	public:
		RenderUsageStruct3D();
		RenderUsageStruct3D(Pg::Data::RenderTextData rTextData);

		void Clear();
		bool IsResourcePathSame(const std::string& other);
		bool operator==(const RenderUsageStruct3D& rhs) const;
		bool operator!=(const RenderUsageStruct3D& rhs) const;

	private:
		DirectX::XMFLOAT4X4 _worldMatrix3D;
		std::string _resourcePath3D;
		//Graphics Resource -> Render();
	};
}