#pragma once
#include "IRenderSinglePass.h"
#include "ConstantBuffer.h"
#include "ConstantBufferDefine.h"
#include "DX11Headers.h"
#include <vector>
#include <memory>

/// <summary>
/// First Instanced Render Pass (Static)
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
	class FirstInstancedRenderPass : public IRenderSinglePass
	{
	public:
		FirstInstancedRenderPass();
		~FirstInstancedRenderPass();

		virtual void Initialize() override;
		virtual void ReceiveRequiredElements(const D3DCarrier& carrier) override;
		virtual void BindPass() override;
		virtual void RenderPass(void* renderObjectList, Pg::Data::CameraData* camData) override;
		virtual void UnbindPass() override;
		virtual void ExecuteNextRenderRequirements() override;
		virtual void PassNextRequirements(D3DCarrier& gCarrier) override;

	private:
		void CreateShaders();
		
		void RenderNormalInstanced(void* renderObjectList, Pg::Data::CameraData* camData);
		void RenderCulledOppositeInstanced(void* renderObjectList, Pg::Data::CameraData* camData);

		void RenderAlphaClippedInstanced(void* renderObjectList, Pg::Data::CameraData* camData);
		void RenderAlphaClippedCulledOppositeInstanced(void* renderObjectList, Pg::Data::CameraData* camData);
	private:
		//Normal
		std::unique_ptr<SystemVertexShader> _vs;
		std::unique_ptr<SystemPixelShader> _ps;
		std::unique_ptr<SystemPixelShader> _depthRecordOnlyPS;

		//Alpha-Clipped.
		std::unique_ptr<SystemPixelShader> _alphaClippedPS;
		std::unique_ptr<SystemPixelShader> _alphaClippedDepthRecordOnlyPS;

	private:
		std::unique_ptr<ConstantBuffer<ConstantBufferDefine::cbLightmapCollection>> _lightmapCBuffer{ nullptr };
		std::unique_ptr<ConstantBuffer<ConstantBufferDefine::cbSwitchableViewProj>> _switchableViewProjCBuffer{ nullptr };
	private:
		LowDX11Storage* _DXStorage;
		const D3DCarrier* _d3dCarrierTempStorage;
	};
}
