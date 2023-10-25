#pragma once
#include "GraphicsDLLExporter.h"
#include "RenderUsageStruct2D.h"
#include "RenderObjectBase.h"

/// <summary>
/// GameObject가 그래픽엔진의 렌더링 로직이랑 1대1 연동될 수 있게 하는 클래스이다.
/// 2D 모두와 호환될 것이다.
/// 
/// Core에서 BaseRenderer가 붙은 GameObject들 :
/// RenderObject는 그래픽엔진에 독자적으로 관리되지 않는다.
/// </summary>

namespace Pg::Graphics
{
	class RenderObject2D : public Pg::Graphics::RenderObjectBase
	{
	public:
		RenderObject2D(Pg::Data::BaseRenderer* baseRenderer);
		virtual ~RenderObject2D();

		virtual void Render() override;

	};
}


