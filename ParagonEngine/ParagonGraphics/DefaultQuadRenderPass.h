#pragma once
#include "IRenderSinglePass.h"
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
	class RenderMaterial; //하나의 RenderMaterial을 기준으로 Pass가 구분될 것. (객체 생성 단위)
	class SystemVertexShader;
	class SystemPixelShader;
}

/// <summary>
/// Default는 따로 처리.
/// </summary>

namespace Pg::Graphics
{
	class DefaultQuadRenderPass : public IRenderSinglePass
	{
	public:
		DefaultQuadRenderPass();
		~DefaultQuadRenderPass();

		virtual void Initialize() override;
		virtual void ReceiveRequiredElements(const D3DCarrier& carrier) override;
		virtual void BindPass() override;
		virtual void RenderPass(void* renderObjectList, Pg::Data::CameraData* camData) override;
		virtual void UnbindPass() override;
		virtual void ExecuteNextRenderRequirements() override;
		virtual void PassNextRequirements(D3DCarrier& gCarrier) override;

	private:
		void BindVertexIndexBuffer();
		void BindMaterialIndexConstantBuffer();
		void CreateMaterialIndexConstantBuffer();
		void CreateShaders();
	private:
		ID3D11Buffer* _cbMatID;
		//FilePath는 RenderMaterial 상위 GraphicsResource에 보관되어 있다.

	private:
		////이 RenderTarget에다가 Set해야 한다. -> 밖에서 해준다.
		//ID3D11RenderTargetView* _passRenderTargetView;
		//ID3D11DepthStencilView* _passDepthStencilView;

		std::unique_ptr<SystemVertexShader> _defaultVS;
		std::unique_ptr<SystemPixelShader> _defaultPS;
	private:
		LowDX11Storage* _DXStorage;
		UINT _defaultMatID{ 1 }; //고정!
	};
}


