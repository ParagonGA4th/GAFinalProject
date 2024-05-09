#include "FirstInstancedRenderPass.h"
#include "GBufferRender.h"
#include "GBufferDepthStencil.h"
#include "LowDX11Storage.h"
#include "LayoutDefine.h"
#include "SystemVertexShader.h"
#include "SystemPixelShader.h"

namespace Pg::Graphics
{
	FirstInstancedRenderPass::FirstInstancedRenderPass() : _DXStorage(LowDX11Storage::GetInstance())
	{

	}

	FirstInstancedRenderPass::~FirstInstancedRenderPass()
	{

	}

	void FirstInstancedRenderPass::Initialize()
	{
		CreateShaders();
	}

	void FirstInstancedRenderPass::ReceiveRequiredElements(const D3DCarrier& carrier)
	{
		_d3dCarrierTempStorage = &carrier;
	}

	void FirstInstancedRenderPass::BindPass()
	{
		//자체적인 DSV Clear, Depth Stencil State 리셋, OMSetRenderTargets.
		_DXStorage->_deviceContext->ClearDepthStencilView(_d3dCarrierTempStorage->_gBufRequiredInfoDSV->GetDSV(), D3D11_CLEAR_DEPTH, 1.0f, 0.0f);
		_DXStorage->_deviceContext->OMSetDepthStencilState(_d3dCarrierTempStorage->_gBufRequiredInfoDSV->GetDSState(), 0);

		for (auto& e : _d3dCarrierTempStorage->_gBufRequiredRTVArray)
		{
			_DXStorage->_deviceContext->ClearRenderTargetView(e, _DXStorage->_backgroundColor);
		}

		//자신을 위한 Bind.
		_DXStorage->_deviceContext->OMSetRenderTargets(_d3dCarrierTempStorage->_gBufRequiredRTVArray.size(),
			_d3dCarrierTempStorage->_gBufRequiredRTVArray.data(), _d3dCarrierTempStorage->_gBufRequiredInfoDSV->GetDSV());

		// 셰이더 바인딩.
		_vs->Bind();
		_ps->Bind();

	}

	void FirstInstancedRenderPass::RenderPass(void* renderObjectList, Pg::Data::CameraData* camData)
	{
		RenderObject3DList* tRenderObjectList = reinterpret_cast<RenderObject3DList*>(renderObjectList);



	}

	void FirstInstancedRenderPass::UnbindPass()
	{
		// Unbind RenderTarget
		_DXStorage->_deviceContext->OMSetRenderTargets(_d3dCarrierTempStorage->_gBufRequiredRTVArray.size(), _d3dCarrierTempStorage->NullRTV.data(), nullptr);

		// Unbind Shaders
		_vs->Unbind();
		_ps->Unbind();
	}

	void FirstInstancedRenderPass::ExecuteNextRenderRequirements()
	{

	}

	void FirstInstancedRenderPass::PassNextRequirements(D3DCarrier& gCarrier)
	{

	}

	void FirstInstancedRenderPass::CreateShaders()
	{
		using Pg::Util::Helper::ResourceHelper;

		// Instanced Pass
		_vs = std::make_unique<SystemVertexShader>(ResourceHelper::IfReleaseChangeDebugTextW(Pg::Defines::FIRST_INSTANCED_VS_DIRECTORY), LayoutDefine::GetInstanced1stLayout(),
			LowDX11Storage::GetInstance()->_solidState, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		_ps = std::make_unique<SystemPixelShader>(ResourceHelper::IfReleaseChangeDebugTextW(Pg::Defines::FIRST_INSTANCED_STAGE_PS_DIRECTORY));
	}

}
