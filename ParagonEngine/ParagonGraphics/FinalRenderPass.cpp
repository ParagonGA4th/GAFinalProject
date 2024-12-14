#include "FinalRenderPass.h"
#include "LowDX11Storage.h"
#include "LayoutDefine.h"
#include "SystemVertexShader.h"
#include "SystemPixelShader.h"
#include "GeometryGenerator.h"

namespace Pg::Graphics
{
	FinalRenderPass::FinalRenderPass()// : _toSampleBuffer(from->GetSRV())
	{
		_DXStorage = LowDX11Storage::GetInstance();
	}

	FinalRenderPass::~FinalRenderPass()
	{

	}

	void FinalRenderPass::Initialize()
	{
		CreateShaders();
	}

	void FinalRenderPass::ReceiveRequiredElements(const D3DCarrier& carrier)
{
		//FinalQuadSRV 기록.
		_toSampleBuffer = carrier._toSendSRVToEngine;
	}

	void FinalRenderPass::BindPass()
	{
		//Depth Stencil State 디폴트 상태로 바꾸기. 
		_DXStorage->_deviceContext->OMSetDepthStencilState(_DXStorage->_depthStencilState, 0);

		//<MAIN> ClearRenderTargetView. RenderTargetClear.
		_DXStorage->_deviceContext->ClearRenderTargetView(_DXStorage->_mainRTV, _DXStorage->_backgroundColor);

		//<MAIN> ClearDepthStencilView. Depth Buffer // Stencil Buffer 지우기.
		_DXStorage->_deviceContext->ClearDepthStencilView(_DXStorage->_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0.0f);

		_DXStorage->_deviceContext->OMSetRenderTargets(1, &_DXStorage->_mainRTV, _DXStorage->_depthStencilView);

		//Quad의 Vertex, Index 바인딩.
		BindVertexIndexBuffer();

		// 셰이더 바인딩.
		_vs->Bind();
		_ps->Bind();

		//Register T5에 넣어줌. Final Quad SRV.
		_DXStorage->_deviceContext->PSSetShaderResources(5, 1, &_toSampleBuffer);
	}

	void FinalRenderPass::RenderPass(void* renderObjectList, Pg::Data::CameraData* camData)
	{
		//RenderPass로 받아야 하지만, 
		//Quad 전체를 MainRenderTarget으로 옮기기만 하는 얘는 상관 없다.
		//Quad만큼 마지막으로 MainRenderTarget에 렌더한다.
		_DXStorage->_deviceContext->DrawIndexed(GeometryGenerator::QUAD_INDICE_COUNT, 0, 0);
	}

	void FinalRenderPass::UnbindPass()
	{
		// Unbind Shaders
		_vs->Unbind();
		_ps->Unbind();

		//Final Quad SRV - GPU 매핑 리셋.
		{
			ID3D11ShaderResourceView* pSRV = nullptr;
			_DXStorage->_deviceContext->PSSetShaderResources(5, 1, &pSRV);
		}
	}

	void FinalRenderPass::CreateShaders()
	{
		using Pg::Util::Helper::ResourceHelper;
		using namespace Pg::Defines;
		//ResourceHelper::IfReleaseChangeDebugTextW(

		// 1st Pass
		_vs = std::make_unique<SystemVertexShader>(ResourceHelper::IfReleaseChangeDebugTextW(FINAL_STAGE_VS_DIRECTORY), LayoutDefine::GetDeferredQuadLayout(),
			LowDX11Storage::GetInstance()->_solidState, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		_ps = std::make_unique<SystemPixelShader>(ResourceHelper::IfReleaseChangeDebugTextW(FINAL_STAGE_PS_DIRECTORY));
	}

	void FinalRenderPass::ExecuteNextRenderRequirements()
	{
		
	}

	void FinalRenderPass::PassNextRequirements(D3DCarrier& gCarrier)
{
		//마지막 Render Pass, 세팅할 이유가 없다.
	}

	void FinalRenderPass::BindVertexIndexBuffer()
	{
		assert(GeometryGenerator::_QUAD_VB != nullptr);
		assert(GeometryGenerator::_QUAD_IB != nullptr);

		// Bind Buffers
		UINT stride = sizeof(GeometryGenerator::GeomVertex_PosNormalTex);
		UINT offset = 0;
		_DXStorage->_deviceContext->IASetVertexBuffers(0, 1, &(GeometryGenerator::_QUAD_VB), &stride, &offset);
		_DXStorage->_deviceContext->IASetIndexBuffer(GeometryGenerator::_QUAD_IB, DXGI_FORMAT_R32_UINT, 0);
	}

	


	

}