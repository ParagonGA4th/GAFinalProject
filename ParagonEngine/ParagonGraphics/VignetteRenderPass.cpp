#include "VignetteRenderPass.h"
#include "LowDX11Storage.h"
#include "LayoutDefine.h"
#include "SystemVertexShader.h"
#include "SystemPixelShader.h"
#include "GeometryGenerator.h"
#include "GBufferRender.h"

namespace Pg::Graphics
{
	VignetteRenderPass::VignetteRenderPass(GBufferRender* from, GBufferRender* to) :
		_postProcessingFrom(from), _postProcessingTo(to)
	{
		_DXStorage = LowDX11Storage::GetInstance();
	}

	VignetteRenderPass::~VignetteRenderPass()
	{

	}

	void VignetteRenderPass::Initialize()
	{
		CreateShaders();
	}

	void VignetteRenderPass::ReceiveRequiredElements(const D3DCarrier& carrier)
	{
		_tempStoreCarrier = &carrier;
	}

	void VignetteRenderPass::BindPass()
	{
		//DepthStencil�� �����ϰ� ����Ѵ�. ������ ������� ���̱⿡.
		_DXStorage->_deviceContext->OMSetRenderTargets(1, &(_postProcessingTo->GetRTV()), nullptr);

		//Quad�� Vertex, Index ���ε�.
		BindVertexIndexBuffer();

		//���̴� ���ε�. VS�� Renderer �������� �̹� ���ε��Ǿ���.
		_ps->Bind();

		//Register T5�� �־���. PostProcessing SRV.
		_DXStorage->_deviceContext->PSSetShaderResources(5, 1, &(_postProcessingFrom->GetSRV()));
	}

	void VignetteRenderPass::RenderPass(void* renderObjectList, Pg::Data::CameraData* camData)
	{
		_DXStorage->_deviceContext->DrawIndexed(GeometryGenerator::QUAD_INDICE_COUNT, 0, 0);
	}

	void VignetteRenderPass::UnbindPass()
	{
		//Pixel Shader Unbind.
		_ps->Unbind();
	}

	void VignetteRenderPass::ExecuteNextRenderRequirements()
	{

	}

	void VignetteRenderPass::PassNextRequirements(D3DCarrier& gCarrier)
	{
		//�ʼ����� �ܰ�. Editor/World���� ������ ���� SRV������ �˷���� �ϴ�.
		gCarrier._toSendSRVToEngine = _postProcessingTo->GetSRV();
	}

	void VignetteRenderPass::CreateShaders()
	{
		using Pg::Util::Helper::ResourceHelper;
		using namespace Pg::Defines;
		//ResourceHelper::IfReleaseChangeDebugTextW(

		//���� Tonemapping Shader ����.
		_ps = std::make_unique<SystemPixelShader>(ResourceHelper::IfReleaseChangeDebugTextW(VIGNETTE_PS_DIRECTORY));
	}

	void VignetteRenderPass::BindVertexIndexBuffer()
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
