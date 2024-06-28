#pragma once
#include "IRenderSinglePass.h"
#include "DX11Headers.h"
#include <vector>
#include <memory>

/// <summary>
/// 
/// </summary>

namespace Pg::Graphics
{
	class LowDX11Storage;
	class SystemVertexShader;
	class SystemPixelShader;
	class GBufferRender;
}

namespace Pg::Graphics
{
	class OpaqueShadowRenderPass : public IRenderSinglePass
	{
	public:
		enum { SIZED_UP_SHADOW_VP_SIZE = 4096 };

	public:
		OpaqueShadowRenderPass();
		~OpaqueShadowRenderPass();

		virtual void Initialize() override;
		virtual void ReceiveRequiredElements(const D3DCarrier& carrier) override;
		virtual void BindPass() override;
		virtual void RenderPass(void* renderObjectList, Pg::Data::CameraData* camData) override;
		virtual void UnbindPass() override;
		virtual void ExecuteNextRenderRequirements() override;
		virtual void PassNextRequirements(D3DCarrier& gCarrier) override;

	private:
		void CreateShaders();
		void BindVertexIndexBuffer();
		void SetHugeViewport();
		void ResetHugeViewport();

	private:
		std::unique_ptr<SystemVertexShader> _lightDepthVS;
		std::unique_ptr<SystemPixelShader> _lightDepthPS;

	private:
		std::unique_ptr<GBufferRender> _shadowDepthBuffer;

	private:
		const D3DCarrier* _storedCarrier{ nullptr };


	private:
		LowDX11Storage* _DXStorage;
	};
}
