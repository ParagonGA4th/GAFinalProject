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
		virtual void ReceiveRequiredElements(ID3D11RenderTargetView** rtvArray, unsigned int rtvCount, ID3D11ShaderResourceView** srvArray, unsigned int srvCount, ID3D11DepthStencilView* dsv) override;
		virtual void BindPass() override;
		virtual void RenderPass(RenderObject3DList* renderObjectList, Pg::Data::CameraData* camData) override;
		virtual void UnbindPass() override;
		virtual void ExecuteNextRenderRequirements() override;
		virtual void PassNextRequirements(ID3D11RenderTargetView**& rtvArray, unsigned int& rtvCount, ID3D11ShaderResourceView**& srvArray, unsigned int& srvCount, ID3D11DepthStencilView*& dsv)  override;

	private:
		void CreateShaders();
		void CreateVertexIndexBuffer();
		void BindVertexIndexBuffer();


	private:
		std::unique_ptr<SystemVertexShader> _vs;
		std::unique_ptr<SystemPixelShader> _ps;

		ID3D11Buffer* _quadVB;
		ID3D11Buffer* _quadIB;

	private:
		ID3D11ShaderResourceView* _finalQuadSRV;

	private:
		LowDX11Storage* _DXStorage;
	};
}


