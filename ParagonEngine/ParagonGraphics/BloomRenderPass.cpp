#include "BloomRenderPass.h"
#include "LowDX11Storage.h"
#include "LayoutDefine.h"
#include "SystemVertexShader.h"
#include "SystemPixelShader.h"
#include "GeometryGenerator.h"
#include "GBufferRender.h"

namespace Pg::Graphics
{
	BloomRenderPass::BloomRenderPass(GBufferRender* from, GBufferRender* to) :
		_postProcessingFrom(from), _postProcessingTo(to)
	{
		_DXStorage = LowDX11Storage::GetInstance();
	}

	BloomRenderPass::~BloomRenderPass()
	{

	}

	void BloomRenderPass::Initialize()
	{
		CreateShaders();
	}

	void BloomRenderPass::ReceiveRequiredElements(const D3DCarrier& carrier)
	{
		_tempStoreCarrier = &carrier;
	}

	void BloomRenderPass::BindPass()
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

	void BloomRenderPass::RenderPass(void* renderObjectList, Pg::Data::CameraData* camData)
	{
		_DXStorage->_deviceContext->DrawIndexed(GeometryGenerator::QUAD_INDICE_COUNT, 0, 0);
	}

	void BloomRenderPass::UnbindPass()
	{
		//Pixel Shader Unbind.
		_ps->Unbind();
	}

	void BloomRenderPass::ExecuteNextRenderRequirements()
	{

	}

	void BloomRenderPass::PassNextRequirements(D3DCarrier& gCarrier)
	{
		//�ʼ����� �ܰ�. Editor/World���� ������ ���� SRV������ �˷���� �ϴ�.
		gCarrier._toSendSRVToEngine = _postProcessingTo->GetSRV();
	}

	void BloomRenderPass::CreateShaders()
	{
		using Pg::Util::Helper::ResourceHelper;
		using namespace Pg::Defines;
		//ResourceHelper::IfReleaseChangeDebugTextW(

		//���� Tonemapping Shader ����.
		_ps = std::make_unique<SystemPixelShader>(ResourceHelper::IfReleaseChangeDebugTextW(BLOOM_PS_DIRECTORY));
	}

	void BloomRenderPass::BindVertexIndexBuffer()
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
