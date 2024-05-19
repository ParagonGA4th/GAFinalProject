#pragma once
#include "IRenderSinglePass.h"
#include "DX11Headers.h"
#include <vector>
#include <memory>

/// <summary>
/// Final Render Pass : Quad에서의 값을 MainRenderTarget에 뿌려준다.
/// 모든 렌더링을 포함해서 최종적인 렌더링 결과가 될 것.
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
	class BloomRenderPass : public IRenderSinglePass
	{
	public:
		BloomRenderPass(GBufferRender* from, GBufferRender* to);
		~BloomRenderPass();

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


