#include "OutlineRenderPass.h"
#include "GBufferDepthStencil.h"
#include "GBufferRender.h"
#include "SystemVertexShader.h"
#include "SystemPixelShader.h"
#include "LowDX11Storage.h"
#include "LayoutDefine.h"
#include "GeometryGenerator.h"

#include <DirectXColors.h>

namespace Pg::Graphics
{
	//Stencil ����� ���µ�, 
	//1. �Ʊ� ���õ� ObjID�� Quad�� �о����.
	//2. Stencil Masking, Texture�� ���.
	//3. Screen Space Blurring, Gaussian�� ������ ����.
	//4. �׷��ٸ�, ����� �� �� Blur�� ������ ��. (StencilMask�� ����� �߶��� ��)
	//5. �̸� Depth ���� �����ϸ� Outline�� �����ϴ�.

	OutlineRenderPass::OutlineRenderPass()
	{
		_DXStorage = LowDX11Storage::GetInstance();

		//Outline�� �ʿ��� ���� �ޱ�.
		_outlineBufferRender = std::make_unique<GBufferRender>(DXGI_FORMAT_R32G32B32A32_TYPELESS, DXGI_FORMAT_R32G32B32A32_FLOAT);

		//Masking Mode�� GBufferDepthStencil ���ο� ����ִ�. (DSS)
		D3D11_DEPTH_STENCIL_DESC tMaskingDesc = CD3D11_DEPTH_STENCIL_DESC{ D3D11_DEFAULT };
		tMaskingDesc.StencilEnable = TRUE;
		tMaskingDesc.StencilWriteMask = 0xFF;
		tMaskingDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		tMaskingDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
		tMaskingDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		tMaskingDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;

		_outlineMaskingGDS = std::make_unique<GBufferDepthStencil>(&tMaskingDesc);

		//Write Mode: Depth�� ���� ����Ѵ�.
		D3D11_DEPTH_STENCIL_DESC tWriteDesc = CD3D11_DEPTH_STENCIL_DESC{ D3D11_DEFAULT };
		tWriteDesc.DepthEnable = FALSE;
		tWriteDesc.StencilEnable = TRUE;
		tWriteDesc.StencilWriteMask = 0xFF;
		tWriteDesc.FrontFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
		tWriteDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		tWriteDesc.BackFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
		tWriteDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;

		HR(_DXStorage->_device->CreateDepthStencilState(&tWriteDesc, &_writeModeDSS));
	}

	OutlineRenderPass::~OutlineRenderPass()
	{

	}

	void OutlineRenderPass::Initialize()
	{
		using Pg::Util::Helper::ResourceHelper;
		using namespace Pg::Defines;
		//ResourceHelper::IfReleaseChangeDebugTextW(
		
		// Selected Outline Passes.
		_vs = std::make_unique<SystemVertexShader>(ResourceHelper::IfReleaseChangeDebugTextW(SELECTED_OUTLINE_VS_DIRECTORY), LayoutDefine::GetDeferredQuadLayout(),
			LowDX11Storage::GetInstance()->_solidState, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		_singleColorPs = std::make_unique<SystemPixelShader>(ResourceHelper::IfReleaseChangeDebugTextW(SELECTED_OUTLINE_SINGLE_COLOR_PS_DIRECTORY));
		_blurPs = std::make_unique<SystemPixelShader>(ResourceHelper::IfReleaseChangeDebugTextW(SELECTED_OUTLINE_BLUR_PS_DIRECTORY));

		//Width Height�� �Ҵ�.
		_widthHeight = { static_cast<float>(_DXStorage->_screenWidth), 
			 static_cast<float>(_DXStorage->_screenHeight) };

		CreateObjectIndexConstantBuffer();
		CreateWidthHeightConstantBuffer();
	}

	void OutlineRenderPass::ReceiveRequiredElements(const D3DCarrier& carrier)
	{
		//RT5 : ObjMatAoR
		_objMatSaveSRV = carrier._gBufRequiredInfoRT.at(5)->GetSRV();
		_quadMainSaveRTV = carrier._quadMainRT->GetRTV();
	}

	void OutlineRenderPass::BindPass()
	{
		FLOAT tTransparentColor[4] = {0.f, 0.f, 0.f, 0.f};
		_DXStorage->_deviceContext->ClearRenderTargetView(_outlineBufferRender->GetRTV(), tTransparentColor);
		_DXStorage->_deviceContext->ClearDepthStencilView(_outlineMaskingGDS->GetDSV(),
			D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0.0f);

		_DXStorage->_deviceContext->OMSetRenderTargets(1, &(_outlineBufferRender->GetRTV()), _outlineMaskingGDS->GetDSV());
		_DXStorage->_deviceContext->OMSetDepthStencilState(_outlineMaskingGDS->GetDSState(), 0xFF);

		_vs->Bind();
		_singleColorPs->Bind();

		//Vertex-Index Buffer ���ε�.
		BindVertexIndexBuffer();

		//B3 Slot ���ε�. (Chosen Input ID)
		BindObjectIndexConstantBuffer();

		//3�� Slot�� ObjMat SRV�� �ִ´�. (�ٽñ� ClipUnfits�� ���� ���ؼ�)
		_DXStorage->_deviceContext->PSSetShaderResources(3, 1, &_objMatSaveSRV);
	}

	void OutlineRenderPass::RenderPass(void* renderObjectList, Pg::Data::CameraData* camData)
	{
		//Masking Pass ����, Screen Space Writing Blur�� �ؾ� �Ѵ�!
		
		//�ϴ� SingleColor�� ����.
		_DXStorage->_deviceContext->DrawIndexed(GeometryGenerator::QUAD_INDICE_COUNT, 0, 0);

		//PS�� �ٽ� ���ε�.
		_singleColorPs->Unbind();
		_blurPs->Bind();

		// Unbind RenderTarget -> Outline Buffer Render�� Ȱ���ϱ� ����!
		_DXStorage->_deviceContext->OMSetRenderTargets(0, nullptr, nullptr);

		//�ٽñ� OMSetRenderTargets. -> Main Quad.
		_DXStorage->_deviceContext->OMSetRenderTargets(1, &(_quadMainSaveRTV), _outlineMaskingGDS->GetDSV());

		//7�� �������� (SRV)
		_DXStorage->_deviceContext->PSSetShaderResources(7, 1, &(_outlineBufferRender->GetSRV()));

		//7�� �������� (Constant Buffer)
		BindWidthHeightConstantBuffer();

		//DepthStencilState
		_DXStorage->_deviceContext->OMSetDepthStencilState(_writeModeDSS, 0xFF);

		//�׸���.
		_DXStorage->_deviceContext->DrawIndexed(GeometryGenerator::QUAD_INDICE_COUNT, 0, 0);
	}

	void OutlineRenderPass::UnbindPass()
	{
		// Unbind RenderTarget
		//�� �̻� ���� �������� ���� �� �̷� ������ �Ҵ� �������ָ� �ȴ�.
		_DXStorage->_deviceContext->OMSetRenderTargets(0, nullptr, nullptr);

		_vs->Unbind();
		_blurPs->Unbind();

		//3�� ���� ����. (InputID ����)
		ID3D11Buffer* tNullCB = nullptr;
		_DXStorage->_deviceContext->PSSetConstantBuffers(3, 1, &tNullCB);

		//3�� Slot�� SRV Nullptr�� �ִ´�. (ObjMat ����)
		ID3D11ShaderResourceView* tNullSRV = nullptr;
		_DXStorage->_deviceContext->PSSetShaderResources(3, 1, &tNullSRV);

		//7�� Slot�� SRV Nullptr�� �ִ´�. (Blur GBuffer SRV ����)
		_DXStorage->_deviceContext->PSSetShaderResources(7, 1, &tNullSRV);

		_DXStorage->_deviceContext->OMSetDepthStencilState(nullptr, 0xFF);
	}

	void OutlineRenderPass::ExecuteNextRenderRequirements()
	{

	}

	void OutlineRenderPass::PassNextRequirements(D3DCarrier& gCarrier)
	{

	}

	void OutlineRenderPass::SetOutlinePlannedObjectID(unsigned int val)
	{
		_toHighlightID = val;
	}

	void OutlineRenderPass::BindObjectIndexConstantBuffer()
	{
		//Mapped Subresource �޸� Ŭ��.
		D3D11_MAPPED_SUBRESOURCE res;
		ZeroMemory(&res, sizeof(D3D11_MAPPED_SUBRESOURCE));

		HR(_DXStorage->_deviceContext->Map(_constantBufferObjID, 0, D3D11_MAP_WRITE_DISCARD, 0, &res));

		//�ڽ��� Data ���� ������ �޾ƿ���.
		UINT* data = reinterpret_cast<UINT*>(res.pData);

		//Material �ο�.
		*(data) = _toHighlightID;

		_DXStorage->_deviceContext->Unmap(_constantBufferObjID, 0);

		//���� Binding. (Pixel Shader 3�� �������ͷ� ����)
		_DXStorage->_deviceContext->PSSetConstantBuffers(3, 1, &_constantBufferObjID);
	}

	void OutlineRenderPass::BindWidthHeightConstantBuffer()
	{
		//Mapped Subresource �޸� Ŭ��.
		D3D11_MAPPED_SUBRESOURCE res;
		ZeroMemory(&res, sizeof(D3D11_MAPPED_SUBRESOURCE));

		HR(_DXStorage->_deviceContext->Map(_widthHeightObjID, 0, D3D11_MAP_WRITE_DISCARD, 0, &res));

		//�ڽ��� Data ���� ������ �޾ƿ���.
		DirectX::XMFLOAT2* data = reinterpret_cast<DirectX::XMFLOAT2*>(res.pData);

		//Material �ο�.
		*(data) = _widthHeight;

		_DXStorage->_deviceContext->Unmap(_widthHeightObjID, 0);

		//���� Binding. (Pixel Shader 3�� �������ͷ� ����)
		_DXStorage->_deviceContext->PSSetConstantBuffers(7, 1, &_widthHeightObjID);
	}

	void OutlineRenderPass::BindVertexIndexBuffer()
	{
		assert(GeometryGenerator::_QUAD_VB != nullptr);
		assert(GeometryGenerator::_QUAD_IB != nullptr);

		// Bind Buffers
		UINT stride = sizeof(GeometryGenerator::GeomVertex_PosNormalTex);
		UINT offset = 0;
		_DXStorage->_deviceContext->IASetVertexBuffers(0, 1, &(GeometryGenerator::_QUAD_VB), &stride, &offset);
		_DXStorage->_deviceContext->IASetIndexBuffer(GeometryGenerator::_QUAD_IB, DXGI_FORMAT_R32_UINT, 0);
	}

	void OutlineRenderPass::CreateObjectIndexConstantBuffer()
	{
		//ID�� �����͸� �޾ƿ´�.
		UINT* _cbData = &_toHighlightID;

		//Constant Buffer ��ü�� ����� �ڵ�.
		int sizeCB = (((sizeof(unsigned int) - 1) / 16) + 1) * 16;	// declspec ���� 16����Ʈ ������ �� �ִ�?
		assert(sizeCB % 16 == 0);

		D3D11_BUFFER_DESC tDesc;
		tDesc.ByteWidth = sizeCB; // ������۴� 16����Ʈ ����
		tDesc.Usage = D3D11_USAGE_DYNAMIC;
		tDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		tDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA tSubResource;
		tSubResource.pSysMem = _cbData;

		HR(_DXStorage->_device->CreateBuffer(&tDesc, &tSubResource, &(_constantBufferObjID)));
	}

	void OutlineRenderPass::CreateWidthHeightConstantBuffer()
	{
		//ID�� �����͸� �޾ƿ´�.
		DirectX::XMFLOAT2* _cbData = &_widthHeight;

		//Constant Buffer ��ü�� ����� �ڵ�.
		int sizeCB = (((sizeof(DirectX::XMFLOAT2) - 1) / 16) + 1) * 16;	// declspec ���� 16����Ʈ ������ �� �ִ�?
		assert(sizeCB % 16 == 0);

		D3D11_BUFFER_DESC tDesc;
		tDesc.ByteWidth = sizeCB; // ������۴� 16����Ʈ ����
		tDesc.Usage = D3D11_USAGE_DYNAMIC;
		tDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		tDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA tSubResource;
		tSubResource.pSysMem = _cbData;

		HR(_DXStorage->_device->CreateBuffer(&tDesc, &tSubResource, &(_widthHeightObjID)));
	}

	

}
