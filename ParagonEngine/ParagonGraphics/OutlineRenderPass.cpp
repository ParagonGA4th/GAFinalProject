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
	//Stencil 기법을 쓰는데, 
	//1. 아까 선택된 ObjID만 Quad에 읽어오기.
	//2. Stencil Masking, Texture에 기록.
	//3. Screen Space Blurring, Gaussian일 이유는 없다.
	//4. 그렇다면, 가운데가 텅 빈 Blur만 존재할 것. (StencilMask로 가운데를 잘랐을 것)
	//5. 이를 Depth 없이 렌더하면 Outline이 가능하다.

	OutlineRenderPass::OutlineRenderPass()
	{
		_DXStorage = LowDX11Storage::GetInstance();

		//Outline에 필요한 값을 받기.
		_outlineBufferRender = std::make_unique<GBufferRender>(DXGI_FORMAT_R32G32B32A32_TYPELESS, DXGI_FORMAT_R32G32B32A32_FLOAT);

		//Masking Mode는 GBufferDepthStencil 내부에 들어있다. (DSS)
		D3D11_DEPTH_STENCIL_DESC tMaskingDesc = CD3D11_DEPTH_STENCIL_DESC{ D3D11_DEFAULT };
		tMaskingDesc.StencilEnable = TRUE;
		tMaskingDesc.StencilWriteMask = 0xFF;
		tMaskingDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		tMaskingDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
		tMaskingDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		tMaskingDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;

		_outlineMaskingGDS = std::make_unique<GBufferDepthStencil>(&tMaskingDesc);

		//Write Mode: Depth를 끄고 출력한다.
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
		// Selected Outline Passes.
		_vs = std::make_unique<SystemVertexShader>(L"../Builds/x64/Debug/SelectedOutline_VS.cso", LayoutDefine::GetDeferredQuadLayout(),
			LowDX11Storage::GetInstance()->_solidState, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		_singleColorPs = std::make_unique<SystemPixelShader>(L"../Builds/x64/Debug/SelectedOutline_SingleColor_PS.cso");
		_blurPs = std::make_unique<SystemPixelShader>(L"../Builds/x64/Debug/SelectedOutline_Blur_PS.cso");

		//Width Height을 할당.
		_widthHeight = { static_cast<float>(_DXStorage->_screenWidth), 
			 static_cast<float>(_DXStorage->_screenHeight) };

		CreateObjectIndexConstantBuffer();
		CreateWidthHeightConstantBuffer();
	}

	void OutlineRenderPass::ReceiveRequiredElements(const D3DCarrier& carrier)
	{
		_objMatSaveSRV = carrier._quadObjMatRT->GetSRV();
		_quadMainSaveRTV = carrier._quadMainRT->GetRTV();
	}

	void OutlineRenderPass::BindPass()
	{
		FLOAT tTransparentColor[4] = {0.f, 0.f, 0.f, 0.f};
		_DXStorage->_deviceContext->ClearRenderTargetView(_outlineBufferRender->GetRTV(), tTransparentColor);
		_DXStorage->_deviceContext->ClearDepthStencilView(_outlineMaskingGDS->GetDSV(),
			D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0.0f);

		_DXStorage->_deviceContext->OMSetRenderTargets(1, &(_outlineBufferRender->GetRTV()), _outlineMaskingGDS->GetDSV());
		//_DXStorage->_deviceContext->OMSetDepthStencilState(_outlineMaskingGDS->GetDSState(), 0xFF);

		_vs->Bind();
		_singleColorPs->Bind();

		//Vertex-Index Buffer 바인딩.
		BindVertexIndexBuffer();

		//B3 Slot 바인딩. (Chosen Input ID)
		BindObjectIndexConstantBuffer();

		//3번 Slot에 ObjMat SRV를 넣는다. (다시금 ClipUnfits를 쓰기 위해서)
		_DXStorage->_deviceContext->PSSetShaderResources(3, 1, &_objMatSaveSRV);
	}

	void OutlineRenderPass::RenderPass(void* renderObjectList, Pg::Data::CameraData* camData)
	{
		//Masking Pass 이후, Screen Space Writing Blur를 해야 한다!
		
		//일단 SingleColor로 렌더.
		_DXStorage->_deviceContext->DrawIndexed(GeometryGenerator::QUAD_INDICE_COUNT, 0, 0);

		//PS만 다시 바인딩.
		_singleColorPs->Unbind();
		_blurPs->Bind();

		// Unbind RenderTarget -> Outline Buffer Render를 활용하기 위해!
		_DXStorage->_deviceContext->OMSetRenderTargets(0, nullptr, nullptr);

		//7번 레지스터 (SRV)
		_DXStorage->_deviceContext->PSSetShaderResources(7, 1, &(_outlineBufferRender->GetSRV()));

		//7번 레지스터 (Constant Buffer)
		BindWidthHeightConstantBuffer();
		
		//다시금 OMSetRenderTargets. -> Main Quad.
		_DXStorage->_deviceContext->OMSetRenderTargets(1, &(_quadMainSaveRTV), nullptr);

		//그리기.
		_DXStorage->_deviceContext->DrawIndexed(GeometryGenerator::QUAD_INDICE_COUNT, 0, 0);
	}

	void OutlineRenderPass::UnbindPass()
	{
		// Unbind RenderTarget
		//더 이상 값을 설정하지 않을 때 이런 식으로 할당 해제해주면 된다.
		_DXStorage->_deviceContext->OMSetRenderTargets(0, nullptr, nullptr);

		_vs->Unbind();
		_blurPs->Unbind();

		//3번 버퍼 리셋. (InputID 리셋)
		ID3D11Buffer* tNullCB = nullptr;
		_DXStorage->_deviceContext->PSSetConstantBuffers(3, 1, &tNullCB);

		//3번 Slot에 SRV Nullptr를 넣는다. (ObjMat 리셋)
		ID3D11ShaderResourceView* tNullSRV = nullptr;
		_DXStorage->_deviceContext->PSSetShaderResources(3, 1, &tNullSRV);

		//7번 Slot에 SRV Nullptr를 넣는다. (Blur GBuffer SRV 리셋)
		_DXStorage->_deviceContext->PSSetShaderResources(7, 1, &tNullSRV);
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
		//Mapped Subresource 메모리 클린.
		D3D11_MAPPED_SUBRESOURCE res;
		ZeroMemory(&res, sizeof(D3D11_MAPPED_SUBRESOURCE));

		HR(_DXStorage->_deviceContext->Map(_constantBufferObjID, 0, D3D11_MAP_WRITE_DISCARD, 0, &res));

		//자신의 Data 시작 포인터 받아오기.
		UINT* data = reinterpret_cast<UINT*>(res.pData);

		//Material 부여.
		*(data) = _toHighlightID;

		_DXStorage->_deviceContext->Unmap(_constantBufferObjID, 0);

		//이제 Binding. (Pixel Shader 3번 레지스터로 매핑)
		_DXStorage->_deviceContext->PSSetConstantBuffers(3, 1, &_constantBufferObjID);
	}

	void OutlineRenderPass::BindWidthHeightConstantBuffer()
	{
		//Mapped Subresource 메모리 클린.
		D3D11_MAPPED_SUBRESOURCE res;
		ZeroMemory(&res, sizeof(D3D11_MAPPED_SUBRESOURCE));

		HR(_DXStorage->_deviceContext->Map(_widthHeightObjID, 0, D3D11_MAP_WRITE_DISCARD, 0, &res));

		//자신의 Data 시작 포인터 받아오기.
		DirectX::XMFLOAT2* data = reinterpret_cast<DirectX::XMFLOAT2*>(res.pData);

		//Material 부여.
		*(data) = _widthHeight;

		_DXStorage->_deviceContext->Unmap(_widthHeightObjID, 0);

		//이제 Binding. (Pixel Shader 3번 레지스터로 매핑)
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
		//ID의 포인터를 받아온다.
		UINT* _cbData = &_toHighlightID;

		//Constant Buffer 자체를 만드는 코드.
		int sizeCB = (((sizeof(unsigned int) - 1) / 16) + 1) * 16;	// declspec 으로 16바이트 정렬할 수 있다?
		assert(sizeCB % 16 == 0);

		D3D11_BUFFER_DESC tDesc;
		tDesc.ByteWidth = sizeCB; // 상수버퍼는 16바이트 정렬
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
		//ID의 포인터를 받아온다.
		DirectX::XMFLOAT2* _cbData = &_widthHeight;

		//Constant Buffer 자체를 만드는 코드.
		int sizeCB = (((sizeof(DirectX::XMFLOAT2) - 1) / 16) + 1) * 16;	// declspec 으로 16바이트 정렬할 수 있다?
		assert(sizeCB % 16 == 0);

		D3D11_BUFFER_DESC tDesc;
		tDesc.ByteWidth = sizeCB; // 상수버퍼는 16바이트 정렬
		tDesc.Usage = D3D11_USAGE_DYNAMIC;
		tDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		tDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA tSubResource;
		tSubResource.pSysMem = _cbData;

		HR(_DXStorage->_device->CreateBuffer(&tDesc, &tSubResource, &(_widthHeightObjID)));
	}

	

}
