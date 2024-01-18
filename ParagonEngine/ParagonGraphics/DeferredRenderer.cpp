#include "DeferredRenderer.h"
#include "LowDX11Storage.h"
#include "LayoutDefine.h"
#include "GraphicsResourceManager.h"
#include "RenderMaterial.h"

#include "RenderObject3D.h"
#include "RenderObject3DList.h"

//RenderPasses
#include "IRenderPass.h"
#include "FirstStaticRenderPass.h"
#include "ObjMatStaticRenderPass.h"
#include "OpaqueQuadRenderPass.h"
#include "FinalRenderPass.h"

#include "../ParagonData/GameObject.h"
#include "../ParagonData/Transform.h"
#include "../ParagonData/CameraData.h"
#include "../ParagonData/LightType.h"

#include <cassert>

namespace Pg::Graphics
{
	DeferredRenderer::DeferredRenderer(D3DCarrier* d3dCarrier) : BaseSpecificRenderer(d3dCarrier)
	{
		_DXStorage = LowDX11Storage::GetInstance();
	}

	DeferredRenderer::~DeferredRenderer()
	{

	}

	void DeferredRenderer::Initialize()
	{
		//요구되는 렌더 리소스 만들기 (GBufferRender & Depth Stencil)
		_quadMainRTV = std::make_unique<GBufferRender>(DXGI_FORMAT_R32G32B32A32_TYPELESS, DXGI_FORMAT_R32G32B32A32_FLOAT);

		//Depth Writing이 가능한 Description 투입. (현재는 Default랑 같음)
		D3D11_DEPTH_STENCIL_DESC tDepthStencilDesc;
		tDepthStencilDesc.DepthEnable = TRUE;
		tDepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		tDepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
		tDepthStencilDesc.StencilEnable = FALSE;
		tDepthStencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		tDepthStencilDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
		tDepthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		tDepthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		tDepthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		tDepthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		tDepthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		tDepthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		tDepthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		tDepthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;

		_quadMainDSV = std::make_unique<GBufferDepthStencil>(&tDepthStencilDesc);

		//자체적인 OpaqueQuad DSV.
		_opaqueQuadDSV = std::make_unique<GBufferDepthStencil>();
	}

	void DeferredRenderer::RenderContents(void* renderObjectList, Pg::Data::CameraData* camData)
	{
		Render((RenderObject3DList*)renderObjectList, camData);
	}

	void DeferredRenderer::Render(RenderObject3DList* renderObjectList, Pg::Data::CameraData* camData)
	{
		//패스 외적으로 들어가야 하는 리소스들 GPU에 배치. 이 경우, SamplerState만 위로 배치.
		PlaceRequiredResources();

		//렌더러들 사이로 돌 D3DCarrier의 리소스 기본 설정.
		UpdateCarrierResources();

		//Pass를 순서대로 호출하는 방식.
		//나중에 Custom 동작이 필요하다고 한다면, 단순한 For문으로 안될 수도 있다.
		//여튼, 특정한 Pass에 값을 전달하는 코드가 있어야 할 것이다.

		//For문 대신, 명시적으로 값 호출. (나누기)
		RenderFirstStaticPass(renderObjectList, camData);
		RenderObjMatStaticPass(renderObjectList, camData);
		RenderOpaqueQuadPasses(renderObjectList, camData);


	}

	void DeferredRenderer::ConfirmCarrierData()
	{
		//		_carrier->_quadMainGDS = 
	}

	void DeferredRenderer::PushRenderPasses()
	{
		//Render Pass Vector 구성.
		
		//첫번째는 무조건 FirstRenderPass.
		_firstStaticRenderPass = new FirstStaticRenderPass();

		//두번째는 일단 ObjMatStaticRenderPass.
		_objMatStaticRenderPass = new ObjMatStaticRenderPass();

		//Skinned가 들어오면 FirstStatic->FirstSkinned->ObjMatStatic->ObjMatSkinned일것.

		//모든 Material의 목록을 받은 뒤, 순서대로 OpaqueQuadRenderPass 호출. (일반적인 경우)
		//N개의 Material이 있으면, N개의 Pass가 만들어진다.
		using Pg::Graphics::Manager::GraphicsResourceManager;
		auto tMatVec = GraphicsResourceManager::Instance()->GetAllResourcesByDefine(Data::Enums::eAssetDefine::_RENDERMATERIAL);
		for (auto& it : tMatVec)
		{
			RenderMaterial* tRM = static_cast<RenderMaterial*>(it.get());
			assert(tRM != nullptr);
			_opaqueQuadPassesVector.push_back(new OpaqueQuadRenderPass(tRM));
		}
	}

	void DeferredRenderer::InitializeRenderPasses()
	{
		_firstStaticRenderPass->Initialize();
		_objMatStaticRenderPass->Initialize();

		//일괄적으로 Initialize() 호출.
		for (auto& it : _opaqueQuadPassesVector)
		{
			it->Initialize();
		}
	}

	void DeferredRenderer::PlaceRequiredResources()
	{
		//샘플러 함수. (Appends_SamplerStates.hlsli)

		//SamplerState fullScreenQuadSS : register(s0)
		_DXStorage->_deviceContext->PSSetSamplers(0, 1, &(_DXStorage->_fullScreenQuadSamplerState));

		//SamplerState lightmapSS : register(s1);
		_DXStorage->_deviceContext->PSSetSamplers(1, 1, &(_DXStorage->_lightmapSamplerState));

		//SamplerState defaultTextureSS : register(s2);
		_DXStorage->_deviceContext->PSSetSamplers(2, 1, &(_DXStorage->_defaultSamplerState));

	}

	void DeferredRenderer::UpdateCarrierResources()
	{
		//Carrier에 값을 전달한다. (MainRenderTarget 전까지 모든 렌더링의 기본이 될 것)
		_carrier->_quadMainRT = _quadMainRTV.get();
		_carrier->_quadMainGDS = _quadMainDSV.get();

		//모든 RGBA값이 0이 되도록 초기화.
		float zeroColArray[4] = {0.f, 0.f, 0.f, 0.f};

		//자체적인 DSV Clear, Depth Stencil State 리셋, OMSetRenderTargets.
		_DXStorage->_deviceContext->ClearRenderTargetView(_carrier->_quadMainRT->GetRTV(), zeroColArray);

		_DXStorage->_deviceContext->ClearDepthStencilView(_carrier->_quadMainGDS->GetDSV(),
			D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0.0f);
		_DXStorage->_deviceContext->OMSetDepthStencilState(_carrier->_quadMainGDS->GetDSState(), 0);
	}

	void DeferredRenderer::SetupRenderPasses()
	{
		PushRenderPasses();
		InitializeRenderPasses();
	}

	void DeferredRenderer::RenderFirstStaticPass(RenderObject3DList* renderObjectList, Pg::Data::CameraData* camData)
	{
		
		//0번째 RenderPass : 초반 Static Mesh 그대로 전달한다.
		_firstStaticRenderPass->ReceiveRequiredElements(*_carrier);
		_firstStaticRenderPass->BindPass();
		_firstStaticRenderPass->RenderPass(renderObjectList, camData);
		_firstStaticRenderPass->UnbindPass();
		_firstStaticRenderPass->ExecuteNextRenderRequirements();
		_firstStaticRenderPass->PassNextRequirements(*_carrier);

		//[구상했던 것, 취소됨]
		//Skinned가 들어오면 달라져야 하지만, DepthStencil을 전달할 수 있어야 한다.
		//Forward에게 넘길 Deferred Object의 Depth Stencil을 전달한다.

	}

	void DeferredRenderer::RenderObjMatStaticPass(RenderObject3DList* renderObjectList, Pg::Data::CameraData* camData)
	{
		//1번째 RenderPass : ObjMatStaticRenderPass.
		_objMatStaticRenderPass->ReceiveRequiredElements(*_carrier);
		_objMatStaticRenderPass->BindPass();
		_objMatStaticRenderPass->RenderPass(renderObjectList, camData);
		_objMatStaticRenderPass->UnbindPass();
		_objMatStaticRenderPass->ExecuteNextRenderRequirements();
		_objMatStaticRenderPass->PassNextRequirements(*_carrier);
	
	}

	void DeferredRenderer::RenderOpaqueQuadPasses(RenderObject3DList* renderObjectList, Pg::Data::CameraData* camData)
	{
		//Opaque Quad 전용 RTV / DSV 클리어.
		_DXStorage->_deviceContext->ClearDepthStencilView(_opaqueQuadDSV->GetDSV(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0.0f);
		_DXStorage->_deviceContext->OMSetRenderTargets(1, &(_quadMainRTV->GetRTV()), _opaqueQuadDSV->GetDSV());

		//Opaque Quad에서 사용되는 정보들 캐리어에 투입.
		_carrier->_dsv = _opaqueQuadDSV->GetDSV();

		//Opaque Quad Render Pass 
		for (int i = 0; i < _opaqueQuadPassesVector.size(); i++)
		{
			//Render Target, Shader Resource View는 이대로 전달할 것.
			_opaqueQuadPassesVector[i]->ReceiveRequiredElements(*_carrier);
			_opaqueQuadPassesVector[i]->BindPass();
			_opaqueQuadPassesVector[i]->RenderPass(renderObjectList, camData);
			_opaqueQuadPassesVector[i]->UnbindPass();
			_opaqueQuadPassesVector[i]->ExecuteNextRenderRequirements();
			_opaqueQuadPassesVector[i]->PassNextRequirements(*_carrier);
		}

		//Quad 렌더링하는데 쓰였던 Resources들 Clear.
		//더 이상 안쓰이는 Resource Slot들 -> nullptr로 설정.
		ID3D11ShaderResourceView* pSRV = nullptr;
		for (int i = 0; i < 7; i++)
		{
			_DXStorage->_deviceContext->PSSetShaderResources(i, 1, &pSRV);
		}
	}

}


