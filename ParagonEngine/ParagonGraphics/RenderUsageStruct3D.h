#pragma once
#include "../ParagonData/RenderTextData.h"

#include <DirectXMath.h> //GraphicEngine 내부적으로만 활용.
#include <string> 

/// <summary>
/// 실제 렌더링에 필요한 정보 구성 (3D) / RenderTextData 기반, 업데이트마다 수정되어야!
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