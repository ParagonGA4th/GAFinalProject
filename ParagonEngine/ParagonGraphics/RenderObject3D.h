#pragma once
#include "GraphicsDLLExporter.h"
#include "RenderUsageStruct3D.h"
#include "RenderObjectBase.h"

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

		//렌더링을 위한 오브젝트의 데이터를 큰 갈래에서 업데이트
		virtual void UpdateObjectRenderData(const Pg::Data::RenderTextData rTextData) override;

	private:

		//3D 오브젝트 렌더링을 위한 정보 업데이트
		void Update3DObjectRenderData(const Pg::Data::RenderTextData& recent);

	private:
		RenderUsageStruct3D _renderUsageStruct3D;
	};
}


