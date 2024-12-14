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
	class SystemVertexShader;
	class SystemPixelShader;
}

/// <summary>
/// 선택된 오브젝트 아웃라인하는 Pass.
/// </summary>

namespace Pg::Graphics
{
	class OutlineRenderPass : public IRenderSinglePass
	{
	public:
		OutlineRenderPass();
		~OutlineRenderPass();

		virtual void Initialize() override;
		virtual void ReceiveRequiredElements(const D3DCarrier& carrier) override;
		virtual void BindPass() override;
		virtual void RenderPass(void* renderObjectList, Pg::Data::CameraData* camData) override;
		virtual void UnbindPass() override;
		virtual void ExecuteNextRenderRequirements() override;
		virtual void PassNextRequirements(D3DCarrier& gCarrier) override;

		void SetOutlinePlannedObjectID(unsigned int val);

	private:
		void BindObjectIndexConstantBuffer();
		void BindWidthHeightConstantBuffer();

		void BindVertexIndexBuffer();
		void CreateObjectIndexConstantBuffer();
		void CreateWidthHeightConstantBuffer();

	private:
		//Carrier에서 임시로 보관하고 있는 SRV. ObjMat 제외 Clipping.
		ID3D11ShaderResourceView* _objMatSaveSRV;
		//일단 출력할 QuadMainRenderTarget.
		ID3D11RenderTargetView* _quadMainSaveRTV;

		std::unique_ptr<GBufferRender> _outlineBufferRender;
		std::unique_ptr<GBufferDepthStencil> _outlineMaskingGDS;

		ID3D11DepthStencilState* _writeModeDSS = nullptr;

		std::unique_ptr<SystemVertexShader> _vs;
		std::unique_ptr<SystemPixelShader> _singleColorPs;
		std::unique_ptr<SystemPixelShader> _blurPs;

	private:
		//임시로 하이라이트할 오브젝트 아이디를 보관하는 변수.
		unsigned int _toHighlightID = NULL;
		DirectX::XMFLOAT2 _widthHeight;

		ID3D11Buffer* _constantBufferObjID = nullptr;
		ID3D11Buffer* _widthHeightObjID = nullptr;

	private:
		LowDX11Storage* _DXStorage;
	};
}


