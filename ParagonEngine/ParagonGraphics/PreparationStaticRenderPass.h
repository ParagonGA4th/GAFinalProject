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
		//잠시 Quad에 렌더링할 DSV를 저장해놓는다 (QuadMain)
		ID3D11DepthStencilView* _quadSaveDSV = nullptr;
		//잠시 Quad에 렌더링할 ObjMat GBufRender를 저장해놓는다. (ObjMat)
		GBufferRender* _quadSaveObjMatGBuffer = nullptr;

		//잠시 CameraData 구조체를 보관해놓는다.
		Pg::Data::CameraData* _savedCamData;

	private:
		//PBR G-Buffer들 투입.
		std::unique_ptr<GBufferRender> _albedoAmbiBuffer;
		std::unique_ptr<GBufferRender> _normalRoughBuffer;
		std::unique_ptr<GBufferRender> _specularMetalBuffer;

		//OMSetRenderTarget을 위해서 하나의 Array를 두고 관리.
		std::array<ID3D11RenderTargetView*, 4> _rtBindArray;
		std::array<ID3D11RenderTargetView*, 4> _rtNullBindArray;

		//SceneInfo Constant Buffer.
		std::unique_ptr<ConstantBuffer<ConstantBufferDefine::cbSceneInfo>> _cbSceneInfo;
	private:
		void CreateD3DViews();
		void CreateShaders();
		void CreateBuffers();

	private:
		std::unique_ptr<SystemVertexShader> _vs;
		std::unique_ptr<SystemPixelShader> _ps;

	private:
		LowDX11Storage* _DXStorage;
	};
}