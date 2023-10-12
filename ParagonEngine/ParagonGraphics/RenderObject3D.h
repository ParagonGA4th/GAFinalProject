#pragma once
#include "GraphicsDLLExporter.h"
#include "RenderUsageStruct3D.h"
#include "RenderObjectBase.h"

#include <memory>

//#ForwardTemp : 헤더.
#include <dxtk/GeometricPrimitive.h>

/// <summary>
/// GameObject가 그래픽엔진의 렌더링 로직이랑 1대1 연동될 수 있게 하는 클래스이다.
/// 3D랑 호환된다.
/// 
/// Core에서 BaseRenderer가 붙은 GameObject들 :
/// RenderObject는 그래픽엔진에 독자적으로 관리되지 않는다.
/// </summary>

namespace Pg::Graphics
{
	class RenderObject3D : public Pg::Graphics::RenderObjectBase
	{
	public:
		RenderObject3D(Pg::Data::BaseRenderer* baseRenderer);
		virtual ~RenderObject3D();

		//3D 오브젝트 렌더링을 위한 정보 업데이트
		virtual void UpdateObjectRenderData(const Pg::Data::RenderTextData rTextData) override;

		// #ForwardTemp : ForwardRendering 테스트를 위해 놔둠. WorkSpace 검사 위해.
		std::unique_ptr<DirectX::GeometricPrimitive> _tempPrimitive;

	private:
		RenderUsageStruct3D _renderUsageStruct3D;
	};
}


