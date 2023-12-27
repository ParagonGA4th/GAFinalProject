#pragma once
#include "IRenderPass.h"
#include "DX11Headers.h"
#include <vector>
#include <memory>

namespace Pg::Graphics
{
	class GBufferRender;
	class GBufferDepthStencil;
	class LowDX11Storage;
	class RenderVertexShader;
	class RenderPixelShader;
}

namespace Pg::Graphics
{
	class OpaqueQuadRenderPass : public IRenderPass
	{
	public:
		OpaqueQuadRenderPass();
		~OpaqueQuadRenderPass();

		virtual void Initialize() override;
		virtual void BindPass() override;
		virtual void RenderPass(RenderObject3DList* renderObjectList, Pg::Data::CameraData* camData) override;
		virtual void UnbindPass() override;
		virtual void SetupNextRequirements() override;

	private:
		void GenerateQuadBuffer();
		void BindBuffers();
		void BindShaders();

	private:
		ID3D11Buffer* _quadVB;
		ID3D11Buffer* _quadIB;

	private:
		LowDX11Storage* _DXStorage;
	};
}


