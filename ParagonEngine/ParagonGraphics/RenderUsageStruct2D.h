#pragma once
#include "../ParagonData/RenderTextData.h"

#include <DirectXMath.h> //GraphicEngine 내부적으로만 활용.
#include <string> 

/// <summary>
/// 실제 렌더링에 필요한 정보 구성 (2D) / RenderTextData 기반, 업데이트마다 수정되어야!
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