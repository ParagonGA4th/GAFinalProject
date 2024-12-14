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
	class RenderMaterial; //�ϳ��� RenderMaterial�� �������� Pass�� ���е� ��. (��ü ���� ����)
	class SystemVertexShader;
	class SystemPixelShader;
}

/// <summary>
/// Default�� ���� ó��.
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
		//FilePath�� RenderMaterial ���� GraphicsResource�� �����Ǿ� �ִ�.

	private:
		////�� RenderTarget���ٰ� Set�ؾ� �Ѵ�. -> �ۿ��� ���ش�.
		//ID3D11RenderTargetView* _passRenderTargetView;
		//ID3D11DepthStencilView* _passDepthStencilView;

		std::unique_ptr<SystemVertexShader> _defaultVS;
		std::unique_ptr<SystemPixelShader> _defaultPS;
	private:
		LowDX11Storage* _DXStorage;
		UINT _defaultMatID{ 1 }; //����!
	};
}


