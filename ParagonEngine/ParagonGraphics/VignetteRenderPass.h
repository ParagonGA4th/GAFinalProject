#pragma once
#include "IRenderSinglePass.h"
#include "DX11Headers.h"
#include <vector>
#include <memory>


namespace Pg::Graphics
{
	class LowDX11Storage;
	class SystemVertexShader;
	class SystemPixelShader;
	class GBufferRender;
}

namespace Pg::Graphics
{
	class VignetteRenderPass : public IRenderSinglePass
	{
	public:
		VignetteRenderPass(GBufferRender* from, GBufferRender* to);
		~VignetteRenderPass();

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

	private:
		std::unique_ptr<SystemVertexShader> _vs;
		std::unique_ptr<SystemPixelShader> _ps;

	private:
		LowDX11Storage* _DXStorage;

	private:
		const D3DCarrier* _tempStoreCarrier{ nullptr };

		GBufferRender* _postProcessingFrom{ nullptr };
		GBufferRender* _postProcessingTo{ nullptr };
	};
}


