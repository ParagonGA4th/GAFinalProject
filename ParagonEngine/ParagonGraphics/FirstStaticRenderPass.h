#pragma once
#include "IRenderSinglePass.h"
#include "DX11Headers.h"
#include <vector>
#include <memory>

/// <summary>
/// First Render Pass. World Space -> Screen Space.
/// </summary>

namespace Pg::Graphics
{
	class GBufferRender;
	class GBufferDepthStencil;
	class LowDX11Storage;
	class SystemVertexShader;
	class SystemPixelShader;
}

namespace Pg::Graphics
{
	class FirstStaticRenderPass : public IRenderSinglePass
	{
	public:
		FirstStaticRenderPass();
		~FirstStaticRenderPass();

		virtual void Initialize() override;
		virtual void ReceiveRequiredElements(const D3DCarrier& carrier) override;
		virtual void BindPass() override;
		virtual void RenderPass(void* renderObjectList, Pg::Data::CameraData* camData) override;
		virtual void UnbindPass() override;
		virtual void ExecuteNextRenderRequirements() override;
		virtual void PassNextRequirements(D3DCarrier& gCarrier) override;

	public:

	private:
		std::vector<std::unique_ptr<GBufferRender>> _gBufferRenderList;
		std::unique_ptr<GBufferDepthStencil> _gBufferDepthStencil;

		std::vector<ID3D11RenderTargetView*> _RTVs;
		std::vector<ID3D11ShaderResourceView*> _SRVs;

		std::vector<ID3D11RenderTargetView*> NullRTV;
		std::vector<ID3D11ShaderResourceView*> NullSRV;
	private:
		void CreateD3DViews();
		void CreateShaders();


	private:
		std::unique_ptr<SystemVertexShader> _vs;
		std::unique_ptr<SystemPixelShader> _ps;

	private:
		LowDX11Storage* _DXStorage;
	};
}


