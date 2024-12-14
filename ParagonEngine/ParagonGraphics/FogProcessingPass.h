#pragma once
#include "IRenderSinglePass.h"
#include "DX11Headers.h"
#include <vector>
#include <memory>

/// <summary>
/// Final Render Pass : Quad������ ���� MainRenderTarget�� �ѷ��ش�.
/// ��� �������� �����ؼ� �������� ������ ����� �� ��.
/// </summary>

namespace Pg::Graphics
{
	class LowDX11Storage;
	class SystemVertexShader;
	class SystemPixelShader;
}

namespace Pg::Graphics
{
	class FogProcessingPass : public IRenderSinglePass
	{
	public:
		FogProcessingPass();
		~FogProcessingPass();

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
	};
}
