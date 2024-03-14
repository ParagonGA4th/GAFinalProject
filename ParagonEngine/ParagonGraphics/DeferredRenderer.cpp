#include "DeferredRenderer.h"
#include "LowDX11Storage.h"
#include "LayoutDefine.h"
#include "GraphicsResourceManager.h"
#include "RenderMaterial.h"

#include "RenderObject3D.h"
#include "RenderObject3DList.h"
#include "SceneInformationList.h"

//RenderPasses
#include "IRenderSinglePass.h"
#include "FirstStaticRenderPass.h"
#include "FirstSkinnedRenderPass.h"
#include "PreparationStaticRenderPass.h"
#include "PreparationSkinnedRenderPass.h"
#include "SceneInformationSender.h"
#include "OpaqueQuadRenderPass.h"
#include "OpaqueShadowRenderPass.h"
#include "FinalRenderPass.h"

#include "../ParagonData/GameObject.h"
#include "../ParagonData/Transform.h"
#include "../ParagonData/CameraData.h"
#include "../ParagonData/LightType.h"

#include <cassert>
#include <algorithm>

namespace Pg::Graphics
{
	DeferredRenderer::DeferredRenderer(D3DCarrier* d3dCarrier) : BaseSpecificRenderer(d3dCarrier)
	{
		_DXStorage = LowDX11Storage::GetInstance();

		//NullSRV Array nullptr로 채우기.
		std::fill(_nullSRVArray.begin(), _nullSRVArray.end(), nullptr);
	}

	DeferredRenderer::~DeferredRenderer()
	{

	}

	void DeferredRenderer::Initialize()
	{
		InitOpaqueQuadDirectX();
		InitFirstQuadDirectX();
	}

	void DeferredRenderer::SetDeltaTime(float dt)
	{
		_deltaTimeStorage = dt;
	}

	void DeferredRenderer::RenderContents(void* renderObjectList, void* optionalRequirement, Pg::Data::CameraData* camData)
	{
		Render((RenderObject3DList*)renderObjectList, (SceneInformationList*)optionalRequirement, camData);
	}

	void DeferredRenderer::Render(RenderObject3DList* renderObjectList, SceneInformationList* sceneInfoList, Pg::Data::CameraData* camData)
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
		RenderFirstSkinnedPass(renderObjectList, camData);
		RenderObjMatStaticPass(renderObjectList, camData);
		RenderObjMatSkinnedPass(renderObjectList, camData);
		SendSceneInformation(sceneInfoList, camData);
		RenderOpaqueQuadPasses(renderObjectList, camData);
		RenderOpaqueShadowPass(renderObjectList, camData);

		UnbindExpiredResources();
	}

	void DeferredRenderer::ConfirmCarrierData()
	{
		//		_carrier->_quadMainGDS = 
	}

	void DeferredRenderer::PushRenderPasses()
	{
		//Render Pass Vector 구성.
		
		//0. FirstStaticRenderPass.
		_firstStaticRenderPass = std::make_unique<FirstStaticRenderPass>();

		//1. FirstSkinnedRenderPass.
		_firstSkinnedRenderPass = std::make_unique<FirstSkinnedRenderPass>();

		//2. ObjMatStaticRenderPass.
		_objMatStaticRenderPass = std::make_unique<PreparationStaticRenderPass>();

		//3. ObjMatSkinnedRenderPass.
		_objMatSkinnedRenderPass = std::make_unique<PreparationSkinnedRenderPass>();

		//4. SceneInfromationSender.
		_sceneInformationSender = std::make_unique<SceneInformationSender>();

		//5. OpaqueQuadRenderPass
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

		//6. OpaqueShadowRenderPass.
		_opaqueShadowPass = std::make_unique<OpaqueShadowRenderPass>();
	}

	void DeferredRenderer::InitializeRenderPasses()
	{
		_firstStaticRenderPass->Initialize();
		_firstSkinnedRenderPass->Initialize();
		_objMatStaticRenderPass->Initialize();
		_objMatSkinnedRenderPass->Initialize();
		_sceneInformationSender->Initialize();

		//일괄적으로 Initialize() 호출.
		for (auto& it : _opaqueQuadPassesVector)
		{
			it->Initialize();
		}

		_opaqueShadowPass->Initialize();
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
		
		//SamplerState blurSS : register(s3);
		_DXStorage->_deviceContext->PSSetSamplers(3, 1, &(_DXStorage->_blurSamplerState));
	}

	void DeferredRenderer::UpdateCarrierResources()
	{
		//Carrier에 값을 전달한다. (MainRenderTarget 전까지 모든 렌더링의 기본이 될 것)
		_carrier->_quadMainRT = _quadMainRTV.get();
		_carrier->_quadMainGDS = _quadMainDSV.get();

		//Main ObjMat RT를 Carrier에 전달한다.
		_carrier->_quadObjMatRT = _quadObjMatRTV.get();

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

		//이미 Depth가 올라간 상황.
	}

	void DeferredRenderer::RenderFirstSkinnedPass(RenderObject3DList* renderObjectList, Pg::Data::CameraData* camData)
	{
		//1번째 RenderPass : 초반 Skinned Mesh 그대로 전달한다.
		//DeltaTime은 이미 전달된 상황.
		_firstSkinnedRenderPass->ReceiveRequiredElements(*_carrier);
		_firstSkinnedRenderPass->SetDeltaTime(_deltaTimeStorage); 	//Skinning 활용 패스에 DeltaTime 내부적으로 전달.
		_firstSkinnedRenderPass->BindPass();
		_firstSkinnedRenderPass->RenderPass(renderObjectList, camData);
		_firstSkinnedRenderPass->UnbindPass();
		_firstSkinnedRenderPass->ExecuteNextRenderRequirements();
		_firstSkinnedRenderPass->PassNextRequirements(*_carrier);
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

	void DeferredRenderer::RenderObjMatSkinnedPass(RenderObject3DList* renderObjectList, Pg::Data::CameraData* camData)
	{
		_objMatSkinnedRenderPass->ReceiveRequiredElements(*_carrier);
		_objMatSkinnedRenderPass->SetDeltaTime(_deltaTimeStorage); 	//Skinning 활용 패스에 DeltaTime 내부적으로 전달.
		_objMatSkinnedRenderPass->BindPass();
		_objMatSkinnedRenderPass->RenderPass(renderObjectList, camData);
		_objMatSkinnedRenderPass->UnbindPass();
		_objMatSkinnedRenderPass->ExecuteNextRenderRequirements();
		_objMatSkinnedRenderPass->PassNextRequirements(*_carrier);
	}

	void DeferredRenderer::SendSceneInformation(SceneInformationList* infoList, Pg::Data::CameraData* camData)
	{
		//이 Send/Receive는 CPU <-> GPU 기준.
		//Send = CPU->GPU. // Receive = GPU -> CPU
		_sceneInformationSender->SendData(*infoList, camData);
		_sceneInformationSender->ProcessData();
		_sceneInformationSender->ReceiveData(*infoList);
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

		//더 이상 값을 설정하지 않을 때 이런 식으로 할당 해제해주면 된다.
		_DXStorage->_deviceContext->OMSetRenderTargets(0, nullptr, nullptr);

		//Quad 렌더링하는데 쓰였던 Resources들 Clear.
		//더 이상 안쓰이는 Resource Slot들 -> nullptr로 설정.
		ID3D11ShaderResourceView* tNullSRV = nullptr;
		for (int i = 0; i < 7; i++)
		{
			_DXStorage->_deviceContext->PSSetShaderResources(i, 1, &tNullSRV);
		}
	}

	void DeferredRenderer::RenderOpaqueShadowPass(RenderObject3DList* renderObjectList, Pg::Data::CameraData* camData)
	{
		//Quad에 담겨 있는 상태에서, Light의 위치를 받아 실행.
		//MainLight만을 가지고, Single-DirectionalLight PCF Shadow Mapping을 할 것.

		_opaqueShadowPass->ReceiveRequiredElements(*_carrier);
		_opaqueShadowPass->BindPass();
		_opaqueShadowPass->RenderPass(renderObjectList, camData);
		_opaqueShadowPass->UnbindPass();
		_opaqueShadowPass->ExecuteNextRenderRequirements();
		_opaqueShadowPass->PassNextRequirements(*_carrier);
	}

	void DeferredRenderer::UnbindExpiredResources()
	{
		//Unbing
		ID3D11ShaderResourceView* tNullSRV = nullptr;
		//t12-14 - internalPBRTextures Unbind
		_DXStorage->_deviceContext->PSSetShaderResources(12, 1, &tNullSRV);
		_DXStorage->_deviceContext->PSSetShaderResources(13, 1, &tNullSRV);
		_DXStorage->_deviceContext->PSSetShaderResources(14, 1, &tNullSRV);

		//VS Constant Buffer -> SceneInfo 값 리셋.
		ID3D11Buffer* tNullBuffer = nullptr;
		_DXStorage->_deviceContext->PSSetConstantBuffers(4, 1, &tNullBuffer);

		//GBufferTextures-> GBuffer / Depth Buffer Unbind.
		_DXStorage->_deviceContext->PSSetShaderResources(15, 5, _nullSRVArray.data());
		_DXStorage->_deviceContext->PSSetShaderResources(20, 1, _nullSRVArray.data());

		//t21-22 : IBL TextureCubes + LUT Textures Unbind.
		_DXStorage->_deviceContext->PSSetShaderResources(21, 1, &tNullSRV);
		_DXStorage->_deviceContext->PSSetShaderResources(22, 1, &tNullSRV);
		_DXStorage->_deviceContext->PSSetShaderResources(23, 1, &tNullSRV);
	}

	void DeferredRenderer::InitOpaqueQuadDirectX()
	{
		//요구되는 렌더 리소스 만들기 (GBufferRender & Depth Stencil)
		_quadMainRTV = std::make_unique<GBufferRender>(DXGI_FORMAT_R32G32B32A32_TYPELESS, DXGI_FORMAT_R32G32B32A32_FLOAT);
		//ObjMat RenderTarget
		_quadObjMatRTV = std::make_unique<GBufferRender>(DXGI_FORMAT_R32G32_TYPELESS, DXGI_FORMAT_R32G32_FLOAT);

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

		//Carrier에 값을 전달한다. (MainRenderTarget 전까지 모든 렌더링의 기본이 될 것)
		_carrier->_quadMainRT = _quadMainRTV.get();
		_carrier->_quadMainGDS = _quadMainDSV.get();
		_carrier->_quadObjMatRT = _quadObjMatRTV.get();

		//자체적인 OpaqueQuad DSV.
		_opaqueQuadDSV = std::make_unique<GBufferDepthStencil>();
	}

	void DeferredRenderer::InitFirstQuadDirectX()
	{
		





	}



}


