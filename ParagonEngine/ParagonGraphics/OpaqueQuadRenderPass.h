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
}

/// <summary>
/// �� RenderPass�� ���Ǵ� �ϳ��ϳ��� Material�� �þ��,
/// ���õ� ������Ʈ�� �������� �������� �����Ѵ�.
/// RenderPass�� ������Ʈ�� �������� ���� ��� ���� / SRV �Ҵ��� ���þƼ� �Ѵ�.
/// </summary>
namespace Pg::Graphics
{
	class OpaqueQuadRenderPass : public IRenderSinglePass
	{
	public:
		OpaqueQuadRenderPass(RenderMaterial* renderMat);
		~OpaqueQuadRenderPass();

		virtual void Initialize() override;
		virtual void ReceiveRequiredElements(const D3DCarrier& carrier) override;
		virtual void BindPass() override;
		virtual void RenderPass(void* renderObjectList, Pg::Data::CameraData* camData) override;
		virtual void UnbindPass() override;
		virtual void ExecuteNextRenderRequirements() override;
		virtual void PassNextRequirements(D3DCarrier& gCarrier) override;

		bool GetIsOpaque();

	private:
		void BindVertexIndexBuffer();
		void BindMaterialIndexConstantBuffer();
		void CreateMaterialIndexConstantBuffer();
	private:
		ID3D11Buffer* _cbMatID;
		RenderMaterial* _renderMaterial; //1 OpaqueQuadRenderPass = 1 Material Used.
		//FilePath�� RenderMaterial ���� GraphicsResource�� �����Ǿ� �ִ�.

	private:
		////�� RenderTarget���ٰ� Set�ؾ� �Ѵ�. -> �ۿ��� ���ش�.
		//ID3D11RenderTargetView* _passRenderTargetView;
		//ID3D11DepthStencilView* _passDepthStencilView;

	private:
		LowDX11Storage* _DXStorage;
	};
}


