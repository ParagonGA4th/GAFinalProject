#pragma once
#include "IRenderSinglePass.h"
#include "DX11Headers.h"
#include "ConstantBuffer.h"
#include "ConstantBufferDefine.h"
#include <vector>
#include <memory>

/// <summary>
/// First Render Pass. World Space -> Screen Space. (Static)
/// </summary>

namespace Pg::Graphics
{
	class GBufferRender;
	class GBufferDepthStencil;
	class LowDX11Storage;
	class SystemVertexShader;
	class SystemInterfacedVertexShader;
	class SystemPixelShader;
	class D3DCarrier;
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

	private:
		void CreateShaders();

	private:
		std::unique_ptr<SystemVertexShader> _vs;
		std::unique_ptr<SystemPixelShader> _ps;
		std::unique_ptr<SystemPixelShader> _depthRecordOnlyPS;

	private:
		LowDX11Storage* _DXStorage;
		const D3DCarrier* _d3dCarrierTempStorage;
		std::unique_ptr<ConstantBuffer<ConstantBufferDefine::cbSwitchableViewProj>> _switchableViewProjCBuffer{ nullptr };
	};
}


