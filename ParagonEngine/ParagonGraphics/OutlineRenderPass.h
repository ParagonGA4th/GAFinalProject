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
	class SystemVertexShader;
	class SystemPixelShader;
}

/// <summary>
/// 선택된 오브젝트 아웃라인하는 Pass.
/// </summary>

namespace Pg::Graphics
{
	class OutlineRenderPass : public IRenderPass
	{
	public:
		OutlineRenderPass();
		~OutlineRenderPass();

		virtual void Initialize() override;
		virtual void ReceiveRequiredElements(const D3DCarrier& carrier) override;
		virtual void BindPass() override;
		virtual void RenderPass(void* renderObjectList, Pg::Data::CameraData* camData) override;
		virtual void UnbindPass() override;
		virtual void ExecuteNextRenderRequirements() override;
		virtual void PassNextRequirements(D3DCarrier& gCarrier) override;

	};
}


