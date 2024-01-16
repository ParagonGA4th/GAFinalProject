#include "FinalRenderPass.h"
#include "LowDX11Storage.h"
#include "LayoutDefine.h"
#include "SystemVertexShader.h"
#include "SystemPixelShader.h"
#include "GeometryGenerator.h"

namespace Pg::Graphics
{
	FinalRenderPass::FinalRenderPass()
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

	void FinalRenderPass::ReceiveRequiredElements(ID3D11RenderTargetView** rtvArray, unsigned int rtvCount, 
		ID3D11ShaderResourceView** srvArray, unsigned int srvCount, ID3D11DepthStencilView* dsv)
	{
		//FinalQuadSRV 기록.
		_finalQuadSRV = srvArray[0];
	}

	void FinalRenderPass::BindPass()
	{
		//이미 MainRenderTarget 관련된 Clear 등 상호작용은 ParagonRenderer의 시작에서 실행되었다.
		_DXStorage->_deviceContext->OMSetRenderTargets(1, &_DXStorage->_mainRTV, _DXStorage->_depthStencilView);

		//Quad의 Vertex, Index 바인딩.
		BindVertexIndexBuffer();

		// 셰이더 바인딩.
		_vs->Bind();
		_ps->Bind();

		//Register T5에 넣어줌.
		_DXStorage->_deviceContext->PSSetShaderResources(5, 1, &_finalQuadSRV);
	}

	void FinalRenderPass::RenderPass(RenderObject3DList* renderObjectList, Pg::Data::CameraData* camData)
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
	}

	void FinalRenderPass::CreateShaders()
	{
		// 1st Pass
		_vs = std::make_unique<SystemVertexShader>(L"../Builds/x64/debug/FinalStage_VS.cso", LayoutDefine::GetDeferredQuadLayout(),
			LowDX11Storage::GetInstance()->_solidState, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		_ps = std::make_unique<SystemPixelShader>(L"../Builds/x64/debug/FinalStage_PS.cso");
	}

	void FinalRenderPass::ExecuteNextRenderRequirements()
	{

		//더 이상 안쓰이는 Resource Slot들 -> nullptr로 설정.
		ID3D11ShaderResourceView* pSRV = nullptr;
		for (int i = 0; i < 6; i++)
		{
			_DXStorage->_deviceContext->PSSetShaderResources(i, 1, &pSRV);
		}
	}

	void FinalRenderPass::PassNextRequirements(ID3D11RenderTargetView**& rtvArray, unsigned int& rtvCount, ID3D11ShaderResourceView**& srvArray, unsigned int& srvCount, ID3D11DepthStencilView*& dsv)
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