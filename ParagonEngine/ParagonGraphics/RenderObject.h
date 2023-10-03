#pragma once
#include "../ParagonCore/IRenderObject.h"
#include "GraphicsDLLExporter.h"
#include <DirectXMath.h> //GraphicEngine 내부적으로만 활용.

/// <summary>
/// GameObject가 그래픽엔진의 렌더링 로직이랑 1대1 연동될 수 있게 하는 클래스이다.
/// 
/// Core에서 BaseRenderer가 붙은 GameObject들은 무조건
/// IRenderInfo를 통해서 RenderObject를 가지고 있다.
/// RenderObject는 그래픽엔진에 독자적으로 관리되지 않는다.
/// </summary>

namespace Pg::Graphics
{
	class RenderObject : public Pg::Core::IRenderObject
	{
	public:
		PARAGON_GRAPHICS_DLL RenderObject();
		PARAGON_GRAPHICS_DLL virtual ~RenderObject();

		PARAGON_GRAPHICS_DLL virtual void UpdateObjectRenderData(Pg::Core::RenderTextData rTextData) override;

	private:


	private:
		bool _is3D;

		//RenderTextData를 기반으로 실제 렌더링에 필요한 정보 구성. (3D)
		struct RenderUsageStruct3D
		{
			RenderUsageStruct3D();
			DirectX::XMFLOAT4X4 worldMatrix3D;
			
		};

		//RenderTextData를 기반으로 실제 렌더링에 필요한 정보 구성. (2D)
		struct RenderUsageStruct2D
		{
			RenderUsageStruct2D();
			DirectX::XMFLOAT3X3 worldMatrix2D;

		};
	};


	inline RenderObject::RenderUsageStruct3D::RenderUsageStruct3D()
	{

	}

	inline RenderObject::RenderUsageStruct2D::RenderUsageStruct2D()
	{

	}


}


