#include "DeferredRenderer.h"
#include "LowDX11Storage.h"
#include "LayoutDefine.h"
#include "GraphicsResourceManager.h"
#include "RenderMaterial.h"
#include "RenderCubemap.h"
#include "RenderTexture2D.h"

#include "RenderObject3D.h"
#include "RenderObject3DList.h"
#include "SceneInformationList.h"

//RenderPasses
#include "IRenderSinglePass.h"
#include "SceneInformationSender.h"
#include "FirstInstancedRenderPass.h"
#include "FirstStaticRenderPass.h"
#include "FirstSkinnedRenderPass.h"
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
	DeferredRenderer::DeferredRenderer(D3DCarrier* d3dCarrier, const Pg::Data::Enums::eEditorMode* const editorMode) : BaseSpecificRenderer(d3dCarrier), _editorMode(editorMode)
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
		InitPBRDirectX();
	}

	void DeferredRenderer::ConnectDefaultResources()
	{
		InitFetchIBLBuffers();
	}

	void DeferredRenderer::SetDeltaTime(float dt)
	{
		_deltaTimeStorage = dt;
	}

	void DeferredRenderer::ClearPlaceResources()
	{
		//패스 외적으로 들어가야 하는 리소스들 GPU에 배치. 이 경우, SamplerState만 위로 배치.
		PlaceRequiredResources();

		//렌더러들 사이로 돌 D3DCarrier의 리소스 기본 설정.
		UpdateCarrierResources();
	}

	void DeferredRenderer::RenderContents(void* renderObjectList, void* optionalRequirement, Pg::Data::CameraData* camData)
	{
		Render((RenderObject3DList*)renderObjectList, (SceneInformationList*)optionalRequirement, camData);
	}

	void DeferredRenderer::Render(RenderObject3DList* renderObjectList, SceneInformationList* sceneInfoList, Pg::Data::CameraData* camData)
	{
		//Pass를 순서대로 호출하는 방식.
		//나중에 Custom 동작이 필요하다고 한다면, 단순한 For문으로 안될 수도 있다.
		//여튼, 특정한 Pass에 값을 전달하는 코드가 있어야 할 것이다.

		//For문 대신, 명시적으로 값 호출. (나누기)
		//일단 Scene 정보 활용을 위해 호출 먼저.
		SendSceneInformation(sceneInfoList, camData);

		RenderFirstInstancedPass(renderObjectList, camData);
		RenderFirstStaticPass(renderObjectList, camData);
		RenderFirstSkinnedPass(renderObjectList, camData);

		SendPBRBufferSRVs();
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
		//0. FirstInstancedRenderPass
		_firstInstancedRenderPass = std::make_unique<FirstInstancedRenderPass>();

		//1. FirstStaticRenderPass.
		_firstStaticRenderPass = std::make_unique<FirstStaticRenderPass>();

		//2. FirstSkinnedRenderPass.
		_firstSkinnedRenderPass = std::make_unique<FirstSkinnedRenderPass>(_editorMode);

		//5. SceneInfromationSender.
		_sceneInformationSender = std::make_unique<SceneInformationSender>();

		//6. OpaqueShadowRenderPass.
		_opaqueShadowPass = std::make_unique<OpaqueShadowRenderPass>();
	}

	void DeferredRenderer::SetupOpaqueQuadRenderPasses()
	{
		//기존에 있는 거 있으면 다 지워주고 다시 시작)
		for (auto& it : _opaqueQuadPassesVector)
		{
			if (it != nullptr)
			{
				delete it;
			}
		}
		//클리어.
		_opaqueQuadPassesVector.clear();

		//5. OpaqueQuadRenderPass
		//모든 Material의 목록을 받은 뒤, 순서대로 OpaqueQuadRenderPass 호출. (일반적인 경우)
		//N개의 Material이 있으면, N개의 Pass가 만들어진다.
		using Pg::Graphics::Manager::GraphicsResourceManager;
		auto tMatVec = GraphicsResourceManager::Instance()->GetAllResourcesByDefine(Data::Enums::eAssetDefine::_RENDERMATERIAL);
		for (auto& it : tMatVec)
		{
			RenderMaterial* tRM = static_cast<RenderMaterial*>(it.get());
			assert(tRM != nullptr);

			if (!tRM->GetIsUseAlphaBlending())
			{
				_opaqueQuadPassesVector.push_back(new OpaqueQuadRenderPass(tRM));
			}
		}
	}

	void DeferredRenderer::InitializeRenderPasses()
	{
		_firstInstancedRenderPass->Initialize();
		_firstStaticRenderPass->Initialize();
		_firstSkinnedRenderPass->Initialize();
		_sceneInformationSender->Initialize();

		_opaqueShadowPass->Initialize();
	}

	void DeferredRenderer::InitializeResettablePasses()
	{
		//일괄적으로 Initialize() 호출.
		for (auto& it : _opaqueQuadPassesVector)
		{
			it->Initialize();
		}

		_firstInstancedRenderPass->Initialize();
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
		//_carrier->_quadObjMatRT_AoR = _quadObjMatRTV.get();

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

	void DeferredRenderer::RenderFirstInstancedPass(RenderObject3DList* renderObjectList, Pg::Data::CameraData* camData)
	{
		//0번째 RenderPass : Instanced.
		_firstInstancedRenderPass->ReceiveRequiredElements(*_carrier);
		_firstInstancedRenderPass->BindPass();
		_firstInstancedRenderPass->RenderPass(renderObjectList, camData);
		_firstInstancedRenderPass->UnbindPass();
		_firstInstancedRenderPass->ExecuteNextRenderRequirements();
		_firstInstancedRenderPass->PassNextRequirements(*_carrier);
	}

	void DeferredRenderer::RenderFirstStaticPass(RenderObject3DList* renderObjectList, Pg::Data::CameraData* camData)
	{
		//1. 초반 Static Mesh 그대로 전달한다.
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
		//2번째 RenderPass : 초반 Skinned Mesh 그대로 전달한다.
		//DeltaTime은 이미 전달된 상황.
		_firstSkinnedRenderPass->ReceiveRequiredElements(*_carrier);
		_firstSkinnedRenderPass->SetDeltaTime(_deltaTimeStorage); 	//Skinning 활용 패스에 DeltaTime 내부적으로 전달.
		_firstSkinnedRenderPass->BindPass();
		_firstSkinnedRenderPass->RenderPass(renderObjectList, camData);
		_firstSkinnedRenderPass->UnbindPass();
		_firstSkinnedRenderPass->ExecuteNextRenderRequirements();
		_firstSkinnedRenderPass->PassNextRequirements(*_carrier);
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
			if (_opaqueQuadPassesVector[i]->GetIsOpaque())
			{
				//Render Target, Shader Resource View는 이대로 전달할 것.
				_opaqueQuadPassesVector[i]->ReceiveRequiredElements(*_carrier);
				_opaqueQuadPassesVector[i]->BindPass();
				_opaqueQuadPassesVector[i]->RenderPass(renderObjectList, camData);
				_opaqueQuadPassesVector[i]->UnbindPass();
				_opaqueQuadPassesVector[i]->ExecuteNextRenderRequirements();
				_opaqueQuadPassesVector[i]->PassNextRequirements(*_carrier);
			}
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

		//PS Constant Buffer -> SceneInfo 값 리셋.
		ID3D11Buffer* tNullBuffer = nullptr;
		_DXStorage->_deviceContext->PSSetConstantBuffers(4, 1, &tNullBuffer);

		//PS Constant Buffer -> LightInfo 값 리셋.
		_DXStorage->_deviceContext->PSSetConstantBuffers(5, 1, &tNullBuffer);

		//GBufferTextures-> GBuffer / Depth Buffer Unbind.
		_DXStorage->_deviceContext->PSSetShaderResources(12, 1, &tNullSRV);
		_DXStorage->_deviceContext->PSSetShaderResources(13, 1, &tNullSRV);
		_DXStorage->_deviceContext->PSSetShaderResources(14, 1, &tNullSRV);
		_DXStorage->_deviceContext->PSSetShaderResources(15, 1, &tNullSRV);
		_DXStorage->_deviceContext->PSSetShaderResources(16, 1, &tNullSRV);
		_DXStorage->_deviceContext->PSSetShaderResources(17, 1, &tNullSRV);
		_DXStorage->_deviceContext->PSSetShaderResources(18, 1, &tNullSRV);
		_DXStorage->_deviceContext->PSSetShaderResources(19, 1, &tNullSRV);

		//t21-22 : IBL TextureCubes + LUT Textures Unbind.
		_DXStorage->_deviceContext->PSSetShaderResources(20, 1, &tNullSRV);
		_DXStorage->_deviceContext->PSSetShaderResources(21, 1, &tNullSRV);
		_DXStorage->_deviceContext->PSSetShaderResources(22, 1, &tNullSRV);
	}

	void DeferredRenderer::InitOpaqueQuadDirectX()
	{
		//요구되는 렌더 리소스 만들기 (GBufferRender & Depth Stencil)
		_quadMainRTV = std::make_unique<GBufferRender>(DXGI_FORMAT_R32G32B32A32_TYPELESS, DXGI_FORMAT_R32G32B32A32_FLOAT);
		//ObjMat RenderTarget -> 이제 PBR 버퍼와 swizzling되어 쓰인다!
		//_quadObjMatRTV = std::make_unique<GBufferRender>(DXGI_FORMAT_R32G32B32A32_TYPELESS, DXGI_FORMAT_R32G32B32A32_FLOAT);

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
		//_carrier->_quadObjMatRT_AoR = _quadObjMatRTV.get();

		//자체적인 OpaqueQuad DSV.
		_opaqueQuadDSV = std::make_unique<GBufferDepthStencil>();
	}

	void DeferredRenderer::InitFirstQuadDirectX()
	{
		//RT0
		_carrier->_gBufRequiredInfoRT.emplace_back(std::make_unique<GBufferRender>(DXGI_FORMAT_R32G32B32A32_TYPELESS, DXGI_FORMAT_R32G32B32A32_FLOAT));
		//RT1
		_carrier->_gBufRequiredInfoRT.emplace_back(std::make_unique<GBufferRender>(DXGI_FORMAT_R32G32B32A32_TYPELESS, DXGI_FORMAT_R32G32B32A32_FLOAT));
		//RT2
		_carrier->_gBufRequiredInfoRT.emplace_back(std::make_unique<GBufferRender>(DXGI_FORMAT_R32G32B32A32_TYPELESS, DXGI_FORMAT_R32G32B32A32_FLOAT));
		//RT3
		_carrier->_gBufRequiredInfoRT.emplace_back(std::make_unique<GBufferRender>(DXGI_FORMAT_R32G32B32A32_TYPELESS, DXGI_FORMAT_R32G32B32A32_FLOAT));
		//RT4
		_carrier->_gBufRequiredInfoRT.emplace_back(std::make_unique<GBufferRender>(DXGI_FORMAT_R32G32B32A32_TYPELESS, DXGI_FORMAT_R32G32B32A32_FLOAT));
		//RT5 -> ObjMatAoR
		_carrier->_gBufRequiredInfoRT.emplace_back(std::make_unique<GBufferRender>(DXGI_FORMAT_R32G32B32A32_TYPELESS, DXGI_FORMAT_R32G32B32A32_FLOAT));
		//RT6 -> AlbedoMetallic
		_carrier->_gBufRequiredInfoRT.emplace_back(std::make_unique<GBufferRender>(DXGI_FORMAT_R32G32B32A32_TYPELESS, DXGI_FORMAT_R32G32B32A32_FLOAT));
		//RT7 -> Normal Alpha
		_carrier->_gBufRequiredInfoRT.emplace_back(std::make_unique<GBufferRender>(DXGI_FORMAT_R32G32B32A32_TYPELESS, DXGI_FORMAT_R32G32B32A32_FLOAT));
		//(Depth)
		_carrier->_gBufRequiredInfoDSV = std::make_unique<GBufferDepthStencil>();

		//FirstStage_PS에서 Binding될 Render Target들.
		//Depth는 자동 연동 (DepthStencil 바인딩 공간 별도 존재)
		for (auto& e : _carrier->_gBufRequiredInfoRT)
		{
			_carrier->_gBufRequiredRTVArray.emplace_back(e->GetRTV());
		}

		for (auto& e : _carrier->_gBufRequiredInfoRT)
		{
			_carrier->_gBufRequiredSRVArray.emplace_back(e->GetSRV());
		}

		//지금까지 바인딩된 값만큼 RTV Null Array를 만들어준다.
		//DepthStencil을 더이상 RTV로 기록되지 않음.
		for (int i = 0; i < _carrier->_gBufRequiredInfoRT.size(); ++i)
		{
			_carrier->NullRTV.emplace_back(nullptr);
		}

		//지금까지 바인딩된 값만큼 SRV Null Array를 만들어준다.
		for (int i = 0; i < _carrier->_gBufRequiredSRVArray.size(); ++i)
		{
			_carrier->NullSRV.emplace_back(nullptr);
		}
	}

	void DeferredRenderer::InitPBRDirectX()
	{
		//DepthStencil은 MainQuadDepthStencil이다. (Skinned도 마찬가지)
		//OpaqueQuad 시리즈가 가능한 이유는,
		//Rendering은 Main Render Target에 함에도 DepthStencil을 자체적으로 생성해서 쓰기 때문 (기존의 값이 영향을 주지 않음)

		//이제 ObjMat과 PBR 요소 일부는 함께 기록됨.
		//_carrier->_albedoMetallic_GBuffer = std::make_unique<GBufferRender>(DXGI_FORMAT_R32G32B32A32_TYPELESS, DXGI_FORMAT_R32G32B32A32_FLOAT);
		//_carrier->_normalAlpha_GBuffer = std::make_unique<GBufferRender>(DXGI_FORMAT_R32G32B32A32_TYPELESS, DXGI_FORMAT_R32G32B32A32_FLOAT);

		//일단 값을 OMSetRenderTargets를 위해 설정.
		//ObjMat은 전에 _quadObjMatRT와 공유.
		_carrier->_pbrBindArray[0] = _carrier->_gBufRequiredRTVArray.at(5); //ObjMatAoR
		_carrier->_pbrBindArray[1] = _carrier->_gBufRequiredRTVArray.at(6); //AlbedoMetallic
		_carrier->_pbrBindArray[2] = _carrier->_gBufRequiredRTVArray.at(7); //NormalAlpha

		//NullRTV Array를 위해, nullptr 채우기!
		std::fill(_carrier->_pbrNullBindArray.begin(), _carrier->_pbrNullBindArray.end(), nullptr);
	}

	void DeferredRenderer::SendPBRBufferSRVs()
	{
		//더 이상 t3에 ObjMat 버퍼가 새로 들어가지 않는다. t12에서 같이 들어가서 쓰인다.

		//t12-19 - GBuffer + ObjMatInternalPBRTextures Bind
		_DXStorage->_deviceContext->PSSetShaderResources(12, 5, _carrier->_gBufRequiredSRVArray.data());
		_DXStorage->_deviceContext->PSSetShaderResources(17, 1, &_carrier->_gBufRequiredSRVArray.at(5));
		_DXStorage->_deviceContext->PSSetShaderResources(18, 1, &_carrier->_gBufRequiredSRVArray.at(6));
		_DXStorage->_deviceContext->PSSetShaderResources(19, 1, &_carrier->_gBufRequiredSRVArray.at(7));
		
		//독립적인 IBL Texture들, 여기서 바인딩.
		//t21-23 - internal IBL TextureCubes Bind
		_DXStorage->_deviceContext->PSSetShaderResources(20, 1, &(_iblDiffuseIrradianceMap->GetSRV()));
		_DXStorage->_deviceContext->PSSetShaderResources(21, 1, &(_iblSpecularIrradianceMap->GetSRV()));
		_DXStorage->_deviceContext->PSSetShaderResources(22, 1, &(_iblSpecularLutTextureMap->GetSRV()));
	}

	void DeferredRenderer::InitFetchIBLBuffers()
	{
		auto tDiff = Pg::Graphics::Manager::GraphicsResourceManager::Instance()->GetResource(
			Pg::Defines::ASSET_DEFAULT_IBL_DIFFUSE_IRRADIANCE_CUBEMAP_PATH, Pg::Data::Enums::eAssetDefine::_CUBEMAP);
		_iblDiffuseIrradianceMap = static_cast<RenderCubemap*>(tDiff.get());

		auto tSpec = Pg::Graphics::Manager::GraphicsResourceManager::Instance()->GetResource(
			Pg::Defines::ASSET_DEFAULT_IBL_SPECULAR_IRRADIANCE_CUBEMAP_PATH, Pg::Data::Enums::eAssetDefine::_CUBEMAP);
		_iblSpecularIrradianceMap = static_cast<RenderCubemap*>(tSpec.get());

		auto tSpecLUT = Pg::Graphics::Manager::GraphicsResourceManager::Instance()->GetResource(
			Pg::Defines::ASSET_DEFAULT_IBL_SPECULAR_BRDF_LUT_TEXTURE_PATH, Pg::Data::Enums::eAssetDefine::_TEXTURE2D);
		_iblSpecularLutTextureMap = static_cast<RenderTexture2D*>(tSpecLUT.get());
	}
}


