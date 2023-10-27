#pragma once
#include "RenderObject3D.h"


//#ForwardTemp : 헤더.
#include <dxtk/GeometricPrimitive.h>

/// <summary>
/// StaticMeshRenderer 컴포넌트와 1대1 대응하는 렌더오브젝트.
/// </summary>

namespace Pg::Data
{
	class StaticMeshRenderer;
}

namespace Pg::Graphics
{
	class RenderObjectStaticMesh3D : public RenderObject3D
	{
	public:
		RenderObjectStaticMesh3D(Pg::Data::BaseRenderer* baseRenderer);
		virtual ~RenderObjectStaticMesh3D();

		virtual void Render(Pg::Data::CameraData* camData) override;

	private:
		// #ForwardTemp : ForwardRendering 테스트를 위해 놔둠. WorkSpace 검사 위해.
		//std::unique_ptr<DirectX::GeometricPrimitive> _tempPrimitive;

	private:
		virtual void BindBuffers() override;
	};
}



