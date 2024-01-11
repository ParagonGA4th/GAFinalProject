#pragma once
#include "IRenderPass.h"
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
	class FirstStaticRenderPass : public IRenderPass
	{
	public:
		FirstStaticRenderPass();
		~FirstStaticRenderPass();

		virtual void Initialize() override;
		virtual void ReceiveRequiredElements(ID3D11RenderTargetView** rtvArray, unsigned int rtvCount, ID3D11ShaderResourceView** srvArray, unsigned int srvCount, ID3D11DepthStencilView* dsv) override;
		virtual void BindPass() override;
		virtual void RenderPass(RenderObject3DList* renderObjectList, Pg::Data::CameraData* camData) override;
		virtual void UnbindPass() override;
		virtual void ExecuteNextRenderRequirements() override;
		virtual void PassNextRequirements(ID3D11RenderTargetView**& rtvArray, unsigned int& rtvCount, ID3D11ShaderResourceView**& srvArray, unsigned int& srvCount, ID3D11DepthStencilView*& dsv)  override;

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


