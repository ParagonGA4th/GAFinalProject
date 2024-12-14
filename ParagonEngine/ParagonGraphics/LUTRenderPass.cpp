#include "LUTRenderPass.h"
#include "LowDX11Storage.h"
#include "LayoutDefine.h"
#include "SystemVertexShader.h"
#include "SystemPixelShader.h"
#include "GeometryGenerator.h"
#include "GBufferRender.h"
#include "RenderTexture2D.h"
#include "GraphicsResourceManager.h"

namespace Pg::Graphics
{
	LUTRenderPass::LUTRenderPass(GBufferRender* from, GBufferRender* to) :
		_postProcessingFrom(from), _postProcessingTo(to)
	{
		_DXStorage = LowDX11Storage::GetInstance();
	}

	LUTRenderPass::~LUTRenderPass()
	{

	}

	void LUTRenderPass::Initialize()
	{
		CreateShaders();
		_cbLutWidthHeight = std::make_unique<ConstantBuffer<DirectX::XMFLOAT4>>();
	}

	void LUTRenderPass::ReceiveRequiredElements(const D3DCarrier& carrier)
	{
		_tempStoreCarrier = &carrier;
	}

	void LUTRenderPass::BindPass()
	{
		//DepthStencil�� �����ϰ� ����Ѵ�. ������ ������� ���̱⿡.
		_DXStorage->_deviceContext->OMSetRenderTargets(1, &(_postProcessingTo->GetRTV()), nullptr);

		//Quad�� Vertex, Index ���ε�.
		BindVertexIndexBuffer();

		//���̴� ���ε�. VS�� Renderer �������� �̹� ���ε��Ǿ���.
		_ps->Bind();

		//Register T5�� �־���. PostProcessing SRV.
		_DXStorage->_deviceContext->PSSetShaderResources(5, 1, &(_postProcessingFrom->GetSRV()));

		//���� Constant Buffer (Width/Height)
		{
			_cbLutWidthHeight->GetDataStruct()->x = _storedLUT->GetFileWidth();
			_cbLutWidthHeight->GetDataStruct()->y = _storedLUT->GetFileHeight();
			_cbLutWidthHeight->Update();
			_cbLutWidthHeight->BindPS(0);
		}

		//���� LUT ��ü SRV ���ε�.
		{
			_DXStorage->_deviceContext->PSSetShaderResources(1, 1, &(_storedLUT->GetSRV()));
		}
	}

	void LUTRenderPass::RenderPass(void* renderObjectList, Pg::Data::CameraData* camData)
	{
		_DXStorage->_deviceContext->DrawIndexed(GeometryGenerator::QUAD_INDICE_COUNT, 0, 0);
	}

	void LUTRenderPass::UnbindPass()
	{
		//Pixel Shader Unbind.
		_ps->Unbind();

		//_cbLutWidthHeight Unbind.
		_cbLutWidthHeight->UnbindPS(0);
		// LUT SRV Unbind.
		ID3D11ShaderResourceView* tNullSRV = nullptr;
		_DXStorage->_deviceContext->PSSetShaderResources(1, 1, &tNullSRV);
	}

	void LUTRenderPass::ExecuteNextRenderRequirements()
	{

	}

	void LUTRenderPass::PassNextRequirements(D3DCarrier& gCarrier)
	{
		//�ʼ����� �ܰ�. Editor/World���� ������ ���� SRV������ �˷���� �ϴ�.
		gCarrier._toSendSRVToEngine = _postProcessingTo->GetSRV();
	}

	void LUTRenderPass::CreateShaders()
	{
		using Pg::Util::Helper::ResourceHelper;
		using namespace Pg::Defines;
		//ResourceHelper::IfReleaseChangeDebugTextW(

		//���� Tonemapping Shader ����.
		_ps = std::make_unique<SystemPixelShader>(ResourceHelper::IfReleaseChangeDebugTextW(LUT_PS_DIRECTORY));
	}

	void LUTRenderPass::BindVertexIndexBuffer()
	{
		assert(GeometryGenerator::_QUAD_VB != nullptr);
		assert(GeometryGenerator::_QUAD_IB != nullptr);

		// Bind Buffers
		UINT stride = sizeof(GeometryGenerator::GeomVertex_PosNormalTex);
		UINT offset = 0;
		_DXStorage->_deviceContext->IASetVertexBuffers(0, 1, &(GeometryGenerator::_QUAD_VB), &stride, &offset);
		_DXStorage->_deviceContext->IASetIndexBuffer(GeometryGenerator::_QUAD_IB, DXGI_FORMAT_R32_UINT, 0);
	}

	void LUTRenderPass::ConnectDefaultResources()
	{
		auto it = Pg::Graphics::Manager::GraphicsResourceManager::Instance()->GetResource(
			Pg::Defines::ASSET_DEFAULT_PRELOADED_LUT_PATH, Pg::Data::Enums::eAssetDefine::_TEXTURE2D);
		_storedLUT = static_cast<RenderTexture2D*>(it.get());
	}

}
