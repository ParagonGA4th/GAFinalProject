#include "OpaqueShadowRenderPass.h"
#include "LowDX11Storage.h"
#include "LayoutDefine.h"
#include "SystemVertexShader.h"
#include "SystemPixelShader.h"
#include "GBufferRender.h"
#include "GeometryGenerator.h"

namespace Pg::Graphics
{
	OpaqueShadowRenderPass::OpaqueShadowRenderPass() : _DXStorage(LowDX11Storage::GetInstance())
	{
	
	}

	OpaqueShadowRenderPass::~OpaqueShadowRenderPass()
	{

	}

	void OpaqueShadowRenderPass::Initialize()
	{
		//CreateShaders();
		//
		//_shadowDepthBuffer = std::make_unique<GBufferRender>(DXGI_FORMAT_R32G32B32A32_TYPELESS, DXGI_FORMAT_R32G32B32A32_FLOAT,
		//	DirectX::XMFLOAT2(SIZED_UP_SHADOW_VP_SIZE, SIZED_UP_SHADOW_VP_SIZE));

	}

	void OpaqueShadowRenderPass::ReceiveRequiredElements(const D3DCarrier& carrier)
	{
		_storedCarrier = &carrier;
	}

	void OpaqueShadowRenderPass::BindPass()
	{
		//SetHugeViewport();
		////Deferred Quad ������� ��� : -> ��ġ�� �ʱ⿡ ������ Depth Buffer�� �ʿ� X.
		//_DXStorage->_deviceContext->OMSetRenderTargets(1, &(_shadowDepthBuffer->GetRTV()), nullptr);
		//
		//_lightDepthVS->Bind();
		//_lightDepthPS->Bind();
		
	}

	void OpaqueShadowRenderPass::RenderPass(void* renderObjectList, Pg::Data::CameraData* camData)
	{
		//BindVertexIndexBuffer();
		//
		//_DXStorage->_deviceContext->DrawIndexed(GeometryGenerator::QUAD_INDICE_COUNT, 0, 0);
	}

	void OpaqueShadowRenderPass::UnbindPass()
	{
		//ResetHugeViewport();
		//
		////���� Ÿ�� ����.
		//_DXStorage->_deviceContext->OMSetRenderTargets(0, nullptr, nullptr);
		//
		//_lightDepthVS->Unbind();
		//_lightDepthPS->Unbind();
	}

	void OpaqueShadowRenderPass::ExecuteNextRenderRequirements()
	{
		//���� SRV�� GPU�� �ø���! �̰� ���ø��ؼ� Ȱ���ؾ�.
		//_DXStorage->_deviceContext->PSSetShaderResources(23, 1, &(_shadowDepthBuffer->GetSRV()));
	}

	void OpaqueShadowRenderPass::PassNextRequirements(D3DCarrier& gCarrier)
	{

	}

	//Internal
	void OpaqueShadowRenderPass::CreateShaders()
	{
		//using Pg::Util::Helper::ResourceHelper;
		//using namespace Pg::Defines;
		//
		//_lightDepthVS = std::make_unique<SystemVertexShader>(ResourceHelper::IfReleaseChangeDebugTextW(SHADOW_RECORD_DEPTH_VS_DIRECTORY), LayoutDefine::GetDeferredQuadLayout(),
		//	LowDX11Storage::GetInstance()->_solidState, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//_lightDepthPS = std::make_unique<SystemPixelShader>(ResourceHelper::IfReleaseChangeDebugTextW(SHADOW_RECORD_DEPTH_PS_DIRECTORY));
	}

	void OpaqueShadowRenderPass::BindVertexIndexBuffer()
	{
		//assert(GeometryGenerator::_QUAD_VB != nullptr);
		//assert(GeometryGenerator::_QUAD_IB != nullptr);
		//
		//// Bind Buffers
		//UINT stride = sizeof(GeometryGenerator::GeomVertex_PosNormalTex);
		//UINT offset = 0;
		//_DXStorage->_deviceContext->IASetVertexBuffers(0, 1, &(GeometryGenerator::_QUAD_VB), &stride, &offset);
		//_DXStorage->_deviceContext->IASetIndexBuffer(GeometryGenerator::_QUAD_IB, DXGI_FORMAT_R32_UINT, 0);
	}

	void OpaqueShadowRenderPass::SetHugeViewport()
	{
		//D3D11_VIEWPORT vp;
		//vp.Width = SIZED_UP_SHADOW_VP_SIZE;
		//vp.Height = SIZED_UP_SHADOW_VP_SIZE;
		//vp.MinDepth = 0.0f;
		//vp.MaxDepth = 1.0f;
		//vp.TopLeftX = 0;
		//vp.TopLeftY = 0;
		//
		//_DXStorage->_deviceContext->RSSetViewports(1, &vp);
	}

	void OpaqueShadowRenderPass::ResetHugeViewport()
	{
		// Viewport ����ü ����
		//CD3D11_VIEWPORT defaultViewport(
		//	0.0f,
		//	0.0f,
		//	static_cast<float>(_DXStorage->_screenWidth),
		//	static_cast<float>(_DXStorage->_screenHeight)
		//);
		//
		//// Viewport ����
		//_DXStorage->_deviceContext->RSSetViewports(1, &defaultViewport);
	}
}