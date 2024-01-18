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
		virtual void ReceiveRequiredElements(const GraphicsCarrier& carrier) override;
		virtual void BindPass() override;
		virtual void RenderPass(RenderObject3DList* renderObjectList, Pg::Data::CameraData* camData) override;
		virtual void UnbindPass() override;
		virtual void ExecuteNextRenderRequirements() override;
		virtual void PassNextRequirements(GraphicsCarrier& gCarrier) override;

	private:
		std::unique_ptr<GBufferRender> _gBufferRender;

		//[구상했던 것, 취소됨]
		//자체적인 DepthStencil을 쓰지 않는다.
		//FirstStatic/Skinned가 Depth을 활용한 판정을 할 수 있다면,
		//여기서는 바로 MainDepthStencil을 바인딩을 한다. (ObjMatSkinned와 같이)
		//-> 아니다.. 다른 방법을 갖고 올 것.
		std::unique_ptr<GBufferDepthStencil> _gBufferDepthStencil;

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