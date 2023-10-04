#pragma once
#include "../ParagonCore/IRenderObject.h"
#include "GraphicsDLLExporter.h"
#include "RenderUsageStruct2D.h"
#include "RenderUsageStruct3D.h"

/// <summary>
/// GameObject가 그래픽엔진의 렌더링 로직이랑 1대1 연동될 수 있게 하는 클래스이다.
/// 2D / 3D 모두와 호환될 것이다.
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
	class RenderObject : public Pg::Core::IRenderObject
	{
	public:
		PARAGON_GRAPHICS_DLL RenderObject();
		PARAGON_GRAPHICS_DLL virtual ~RenderObject();

		//렌더링을 위한 오브젝트의 데이터를 큰 갈래에서 업데이트
		PARAGON_GRAPHICS_DLL virtual void UpdateObjectRenderData(const Pg::Core::RenderTextData rTextData) override;

	private:
		//2D 오브젝트 렌더링을 위한 정보 업데이트
		void Update2DObjectRenderData(const Pg::Core::RenderTextData& recent);

		//3D 오브젝트 렌더링을 위한 정보 업데이트
		void Update3DObjectRenderData(const Pg::Core::RenderTextData& recent);

	private:
		bool _is3D;
		RenderUsageStruct2D _renderUsageStruct2D;
		RenderUsageStruct3D _renderUsageStruct3D;

		Pg::Graphics::Manager::GraphicsResourceManager* _graphicsResourceManager;



	};
}


