#pragma once
#include "IRenderPass.h"
#include "DX11Headers.h"
#include <vector>
#include <memory>

/// <summary>
/// Object && Material이 Screen Space에서 어디 있는지 표시해주기 위해
/// (== ClipUnfit을 위해) 존재하는 RenderPass.
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
	class ObjMatStaticRenderPass : public IRenderPass
	{
	public:
		ObjMatStaticRenderPass();
		~ObjMatStaticRenderPass();

		virtual void Initialize() override;
		virtual void ReceiveRequiredElements(const D3DCarrier& carrier) override;
		virtual void BindPass() override;
		virtual void RenderPass(void* renderObjectList, Pg::Data::CameraData* camData) override;
		virtual void UnbindPass() override;
		virtual void ExecuteNextRenderRequirements() override;
		virtual void PassNextRequirements(D3DCarrier& gCarrier) override;

	private:
		std::unique_ptr<GBufferRender> _gBufferRender;

		//잠시 Quad에 렌더링할 DSV를 저장해놓는다 (QuadMain)
		ID3D11DepthStencilView* _quadSaveDSV = nullptr;

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