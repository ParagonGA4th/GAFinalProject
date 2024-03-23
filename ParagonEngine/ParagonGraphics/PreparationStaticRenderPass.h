#pragma once
#include "IRenderSinglePass.h"
#include "DX11Headers.h"
#include "ConstantBuffer.h"
#include "ConstantBufferDefine.h"

#include <vector>
#include <memory>
#include <array>

/// <summary>
/// Object && Material이 Screen Space에서 어디 있는지 표시해주기 위해
/// (== ClipUnfit을 위해) 존재하는 RenderPass. (Static)
/// </summary>

namespace Pg::Graphics
{
	class GBufferRender;
	class GBufferDepthStencil;
	class LowDX11Storage;
	class SystemVertexShader;
	class SystemPixelShader;
	class RenderCubemap;
	class RenderTexture2D;
	class D3DCarrier;
}

namespace Pg::Graphics
{
	class PreparationStaticRenderPass : public IRenderSinglePass
	{
	public:
		PreparationStaticRenderPass();
		~PreparationStaticRenderPass();

		virtual void Initialize() override;
		virtual void ReceiveRequiredElements(const D3DCarrier& carrier) override;
		virtual void BindPass() override;
		virtual void RenderPass(void* renderObjectList, Pg::Data::CameraData* camData) override;
		virtual void UnbindPass() override;
		virtual void ExecuteNextRenderRequirements() override;
		virtual void PassNextRequirements(D3DCarrier& gCarrier) override;

	private:

		//잠시 CameraData 구조체를 보관해놓는다.
		Pg::Data::CameraData* _savedCamData = nullptr;

	private:
		const D3DCarrier* _d3dCarrierStorage;

		//OMSetRenderTarget을 위해서 하나의 Array를 두고 관리.
		

		//SceneInfo Constant Buffer.
		std::unique_ptr<ConstantBuffer<ConstantBufferDefine::cbSceneInfo>> _cbSceneInfo;

		

	private:
		void CreateShaders();
		void CreateBuffers();
		

	private:
		std::unique_ptr<SystemVertexShader> _vs;
		std::unique_ptr<SystemPixelShader> _ps;

	private:
		LowDX11Storage* _DXStorage;
	};
}