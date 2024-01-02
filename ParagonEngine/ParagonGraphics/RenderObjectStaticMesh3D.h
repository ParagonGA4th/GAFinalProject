#pragma once
#include "RenderObject3D.h"
#include "ConstantBuffer.h"
#include "ConstantBufferDefine.h"
#include <memory>

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

		virtual void UpdateConstantBuffers(Pg::Data::CameraData* camData) override;
		virtual void BindBuffers() override;
		virtual void Render() override;
		virtual void UnbindBuffers() override;

	private:
		void BindVertexIndexBuffer();
		std::unique_ptr<ConstantBuffer<ConstantBufferDefine::cbPerObjectBase>> _cBuffer;
	};
}



