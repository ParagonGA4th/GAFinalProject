#include "TonemappingRenderPass.h"
#include "LowDX11Storage.h"
#include "LayoutDefine.h"
#include "SystemVertexShader.h"
#include "SystemPixelShader.h"
#include "GeometryGenerator.h"

namespace Pg::Graphics
{
	TonemappingRenderPass::TonemappingRenderPass(GBufferRender* from, GBufferRender* to) :
		_postProcessingFrom(from), _postProcessingTo(to)
	{
		_DXStorage = LowDX11Storage::GetInstance();
	}

	TonemappingRenderPass::~TonemappingRenderPass()
	{

	}

	void TonemappingRenderPass::Initialize()
	{
		CreateShaders();
	}

	void TonemappingRenderPass::ReceiveRequiredElements(const D3DCarrier& carrier)
	{
		_tempStoreCarrier = &carrier;
	}

	void TonemappingRenderPass::BindPass()
	{
		//DepthStencil은 무시하고 기록한다. 어차피 덧씌우는 것이기에.
		_DXStorage->_deviceContext->OMSetRenderTargets(1, &(_postProcessingTo->GetRTV()), nullptr);

		//Quad의 Vertex, Index 바인딩.
		BindVertexIndexBuffer();

		//셰이더 바인딩. VS는 Renderer 레벨에서 이미 바인딩되었다.
		_ps->Bind();

		//Register T5에 넣어줌. PostProcessing SRV.
		_DXStorage->_deviceContext->PSSetShaderResources(5, 1, &(_postProcessingFrom->GetSRV()));
	}

	void TonemappingRenderPass::RenderPass(void* renderObjectList, Pg::Data::CameraData* camData)
	{
		_DXStorage->_deviceContext->DrawIndexed(GeometryGenerator::QUAD_INDICE_COUNT, 0, 0);
	}

	void TonemappingRenderPass::UnbindPass()
	{
		//Pixel Shader Unbind.
		_ps->Unbind();
	}

	void TonemappingRenderPass::ExecuteNextRenderRequirements()
	{

	}

	void TonemappingRenderPass::PassNextRequirements(D3DCarrier& gCarrier)
	{
		//필수적인 단계. Editor/World에게 무엇이 최종 SRV인지를 알려줘야 하니.
		gCarrier._toSendSRVToEngine = _postProcessingTo->GetSRV();
	}

	void TonemappingRenderPass::CreateShaders()
	{
		using Pg::Util::Helper::ResourceHelper;
		using namespace Pg::Defines;
		//ResourceHelper::IfReleaseChangeDebugTextW(
		
		//별도 Tonemapping Shader 적용.
		_ps = std::make_unique<SystemPixelShader>(ResourceHelper::IfReleaseChangeDebugTextW(TONEMAPPING_PS_DIRECTORY));
	}

	void TonemappingRenderPass::BindVertexIndexBuffer()
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