#pragma once
#include "../ParagonData/IRenderObject.h"
#include "GraphicsDLLExporter.h"
#include "RenderUsageStruct3D.h"

/// <summary>
/// GameObject가 그래픽엔진의 렌더링 로직이랑 1대1 연동될 수 있게 하는 클래스이다.
/// 3D랑 호환된다.
/// 
/// Core에서 BaseRenderer가 붙은 GameObject들은 무조건
/// IRenderInfo를 통해서 RenderObject를 가지고 있다.
/// RenderObject는 그래픽엔진에 독자적으로 관리되지 않는다.
/// </summary>

namespace Pg::Graphics
{
	namespace Manager
	{
		class GraphicsResourceManager;
	}
}

namespace Pg::Graphics
{
	class RenderObject3D : public Pg::Data::IRenderObject
	{
	public:
		PARAGON_GRAPHICS_DLL RenderObject3D();
		PARAGON_GRAPHICS_DLL virtual ~RenderObject3D();

		//렌더링을 위한 오브젝트의 데이터를 큰 갈래에서 업데이트
		PARAGON_GRAPHICS_DLL virtual void UpdateObjectRenderData(const Pg::Data::RenderTextData rTextData) override;

	private:

		//3D 오브젝트 렌더링을 위한 정보 업데이트
		void Update3DObjectRenderData(const Pg::Data::RenderTextData& recent);

	private:
		RenderUsageStruct3D _renderUsageStruct3D;

		Pg::Graphics::Manager::GraphicsResourceManager* _graphicsResourceManager;
	};
}


