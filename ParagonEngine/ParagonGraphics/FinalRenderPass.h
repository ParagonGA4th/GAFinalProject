#pragma once
#include "IRenderPass.h"
#include "DX11Headers.h"
#include <vector>
#include <memory>

/// <summary>
/// Final Render Pass : Quad에서의 값을 MainRenderTarget에 뿌려준다.
/// </summary>

namespace Pg::Graphics
{
	class LowDX11Storage;
	class SystemVertexShader;
	class SystemPixelShader;
}

namespace Pg::Graphics
{
	class FinalRenderPass : public IRenderPass
	{
	public:
		FinalRenderPass();
		~FinalRenderPass();

		virtual void Initialize() override;
		virtual void ReceiveRequiredElements(const GraphicsCarrier& carrier) override;
		virtual void BindPass() override;
		virtual void RenderPass(RenderObject3DList* renderObjectList, Pg::Data::CameraData* camData) override;
		virtual void UnbindPass() override;
		virtual void ExecuteNextRenderRequirements() override;
		virtual void PassNextRequirements(GraphicsCarrier& gCarrier) override;

	private:
		void CreateShaders();
		void BindVertexIndexBuffer();


	private:
		std::unique_ptr<SystemVertexShader> _vs;
		std::unique_ptr<SystemPixelShader> _ps;

	private:
		ID3D11ShaderResourceView* _finalQuadSRV;
		ID3D11ShaderResourceView* _depthObjMatSRV;

	private:
		LowDX11Storage* _DXStorage;
	};
}


