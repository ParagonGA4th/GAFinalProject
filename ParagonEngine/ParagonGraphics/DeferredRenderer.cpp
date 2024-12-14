#include "DeferredRenderer.h"
#include "LowDX11Storage.h"
#include "LayoutDefine.h"
#include "GraphicsResourceManager.h"
#include "GraphicsResourceHelper.h"
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
#include "DefaultQuadRenderPass.h"
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

		//NullSRV Array nullptr�� ä���.
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
		//�н� �������� ���� �ϴ� ���ҽ��� GPU�� ��ġ. �� ���, SamplerState�� ���� ��ġ.
		PlaceRequiredResources();

		//�������� ���̷� �� D3DCarrier�� ���ҽ� �⺻ ����.
		UpdateCarrierResources();
	}

	void DeferredRenderer::RenderContents(void* renderObjectList, void* optionalRequirement, Pg::Data::CameraData* camData)
	{
		Render((RenderObject3DList*)renderObjectList, (SceneInformationList*)optionalRequirement, camData);
	}

	void DeferredRenderer::Render(RenderObject3DList* renderObjectList, SceneInformationList* sceneInfoList, Pg::Data::CameraData* camData)
	{
		//Pass�� ������� ȣ���ϴ� ���.
		//���߿� Custom ������ �ʿ��ϴٰ� �Ѵٸ�, �ܼ��� For������ �ȵ� ���� �ִ�.
		//��ư, Ư���� Pass�� ���� �����ϴ� �ڵ尡 �־�� �� ���̴�.

		//For�� ���, ��������� �� ȣ��. (������)
		//�ϴ� Scene ���� Ȱ���� ���� ȣ�� ����.

		UnbindPreviousBoundResources();
		UnbindExpiredResources();
		UpdateCullingInformation(renderObjectList, camData);
		SendSceneInformation(sceneInfoList, camData);

		RenderFirstInstancedPass(renderObjectList, camData);
		RenderFirstStaticPass(renderObjectList, camData);
		RenderFirstSkinnedPass(renderObjectList, camData);
		UpdateShadowDSV();
		//RenderOpaqueShadowPass(renderObjectList, camData);
		
		//���⼭ �ϴ� ����, ���� ������ �ٽ� Viewport�� ��������.
		_DXStorage->_deviceContext->RSSetViewports(1, &(_DXStorage->_defaultViewport));

		SendPBRBufferSRVs();
		RenderDefaultQuadPass(renderObjectList, camData);
		RenderOpaqueQuadPasses(renderObjectList, camData);
	}

	void DeferredRenderer::ConfirmCarrierData()
	{
		//		_carrier->_quadMainGDS = 
	}

	void DeferredRenderer::PushRenderPasses()
	{
		//Render Pass Vector ����.
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

		//Default Pass.
		_defaultQuadRenderPass = std::make_unique<DefaultQuadRenderPass>();
	}

	void DeferredRenderer::SetupOpaqueQuadRenderPasses()
	{
		//������ �ִ� �� ������ �� �����ְ� �ٽ� ����)
		for (auto& it : _opaqueQuadPassesVector)
		{
			if (it != nullptr)
			{
				delete it;
			}
		}
		//Ŭ����.
		_opaqueQuadPassesVector.clear();

		//5. OpaqueQuadRenderPass
		//��� Material�� ����� ���� ��, ������� OpaqueQuadRenderPass ȣ��. (�Ϲ����� ���)
		//N���� Material�� ������, N���� Pass�� ���������.
		using Pg::Graphics::Manager::GraphicsResourceManager;
		auto tMatVec = GraphicsResourceManager::Instance()->GetAllResourcesByDefine(Data::Enums::eAssetDefine::_RENDERMATERIAL);

		for (auto& it : tMatVec)
		{
			RenderMaterial* tRM = static_cast<RenderMaterial*>(it.get());
			assert(tRM != nullptr);

			bool tIsDefaultMaterial = Pg::Graphics::Helper::GraphicsResourceHelper::IsMaterialDefaultMaterial(tRM);

			//Default Material�� �ƴ� ���� �̷��� ó���Ѵٴ� ���̴�.
			if ((!tRM->GetIsUseAlphaBlending()) && (!tIsDefaultMaterial))
			{
				assert((tRM->GetMaterialID() != 0) && (tRM->GetMaterialID() != 1));
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
		_defaultQuadRenderPass->Initialize();
	}

	void DeferredRenderer::InitializeResettablePasses()
	{
		//�ϰ������� Initialize() ȣ��.
		for (auto& it : _opaqueQuadPassesVector)
		{
			it->Initialize();
		}

		_firstInstancedRenderPass->Initialize();
	}

	void DeferredRenderer::PlaceRequiredResources()
	{
		//���÷� �Լ�. (Appends_SamplerStates.hlsli)

		//SamplerState fullScreenQuadSS : register(s0)
		_DXStorage->_deviceContext->PSSetSamplers(0, 1, &(_DXStorage->_fullScreenQuadSamplerState));

		//SamplerState lightmapSS : register(s1);
		_DXStorage->_deviceContext->PSSetSamplers(1, 1, &(_DXStorage->_lightmapSamplerState));

		//SamplerState defaultTextureSS : register(s2);
		_DXStorage->_deviceContext->PSSetSamplers(2, 1, &(_DXStorage->_defaultSamplerState));
		
		//SamplerState lutSS : register(s3);
		_DXStorage->_deviceContext->PSSetSamplers(3, 1, &(_DXStorage->_lutSamplerState));
	}

	void DeferredRenderer::UpdateCarrierResources()
	{
		//Carrier�� ���� �����Ѵ�. (MainRenderTarget ������ ��� �������� �⺻�� �� ��)
		_carrier->_quadMainRT = _quadMainRTV.get();
		_carrier->_quadMainGDS = _quadMainDSV.get();

		//Main ObjMat RT�� Carrier�� �����Ѵ�.
		//_carrier->_quadObjMatRT_AoR = _quadObjMatRTV.get();

		//��� RGBA���� 0�� �ǵ��� �ʱ�ȭ.
		float zeroColArray[4] = {0.f, 0.f, 0.f, 0.f};

		//��ü���� DSV Clear, Depth Stencil State ����, OMSetRenderTargets.
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
		//0��° RenderPass : Instanced.
		_firstInstancedRenderPass->ReceiveRequiredElements(*_carrier);
		_firstInstancedRenderPass->BindPass();
		_firstInstancedRenderPass->RenderPass(renderObjectList, camData);
		_firstInstancedRenderPass->UnbindPass();
		_firstInstancedRenderPass->ExecuteNextRenderRequirements();
		_firstInstancedRenderPass->PassNextRequirements(*_carrier);
	}

	void DeferredRenderer::RenderFirstStaticPass(RenderObject3DList* renderObjectList, Pg::Data::CameraData* camData)
	{
		//1. �ʹ� Static Mesh �״�� �����Ѵ�.
		_firstStaticRenderPass->ReceiveRequiredElements(*_carrier);
		_firstStaticRenderPass->BindPass();
		_firstStaticRenderPass->RenderPass(renderObjectList, camData);
		_firstStaticRenderPass->UnbindPass();
		_firstStaticRenderPass->ExecuteNextRenderRequirements();
		_firstStaticRenderPass->PassNextRequirements(*_carrier);

		//�̹� Depth�� �ö� ��Ȳ.
	}

	void DeferredRenderer::RenderFirstSkinnedPass(RenderObject3DList* renderObjectList, Pg::Data::CameraData* camData)
	{
		//2��° RenderPass : �ʹ� Skinned Mesh �״�� �����Ѵ�.
		//DeltaTime�� �̹� ���޵� ��Ȳ.
		_firstSkinnedRenderPass->ReceiveRequiredElements(*_carrier);
		_firstSkinnedRenderPass->SetDeltaTime(_deltaTimeStorage); 	//Skinning Ȱ�� �н��� DeltaTime ���������� ����.
		_firstSkinnedRenderPass->BindPass();
		_firstSkinnedRenderPass->RenderPass(renderObjectList, camData);
		_firstSkinnedRenderPass->UnbindPass();
		_firstSkinnedRenderPass->ExecuteNextRenderRequirements();
		_firstSkinnedRenderPass->PassNextRequirements(*_carrier);
	}

	void DeferredRenderer::SendSceneInformation(SceneInformationList* infoList, Pg::Data::CameraData* camData)
	{
		//�� Send/Receive�� CPU <-> GPU ����.
		//Send = CPU->GPU. // Receive = GPU -> CPU
		_sceneInformationSender->SendData(_carrier, *infoList, camData);
		_sceneInformationSender->ProcessData();
		_sceneInformationSender->ReceiveData(*infoList);
	}

	void DeferredRenderer::RenderDefaultQuadPass(RenderObject3DList* renderObjectList, Pg::Data::CameraData* camData)
	{
		//Opaque Quad ���� RTV / DSV Ŭ����.
		_DXStorage->_deviceContext->ClearDepthStencilView(_opaqueQuadDSV->GetDSV(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0.0f);
		_DXStorage->_deviceContext->OMSetRenderTargets(1, &(_quadMainRTV->GetRTV()), _opaqueQuadDSV->GetDSV());

		//Opaque Quad���� ���Ǵ� ������ ĳ��� ����.
		_carrier->_dsv = _opaqueQuadDSV->GetDSV();

		_defaultQuadRenderPass->ReceiveRequiredElements(*_carrier);
		_defaultQuadRenderPass->BindPass();
		_defaultQuadRenderPass->RenderPass(renderObjectList, camData);
		_defaultQuadRenderPass->UnbindPass();
		_defaultQuadRenderPass->ExecuteNextRenderRequirements();
		_defaultQuadRenderPass->PassNextRequirements(*_carrier);
	}

	void DeferredRenderer::RenderOpaqueQuadPasses(RenderObject3DList* renderObjectList, Pg::Data::CameraData* camData)
	{

		//Opaque Quad Render Pass 
		for (int i = 0; i < _opaqueQuadPassesVector.size(); i++)
		{
			if (_opaqueQuadPassesVector[i]->GetIsOpaque())
			{
				//Render Target, Shader Resource View�� �̴�� ������ ��.
				_opaqueQuadPassesVector[i]->ReceiveRequiredElements(*_carrier);
				_opaqueQuadPassesVector[i]->BindPass();
				_opaqueQuadPassesVector[i]->RenderPass(renderObjectList, camData);
				_opaqueQuadPassesVector[i]->UnbindPass();
				_opaqueQuadPassesVector[i]->ExecuteNextRenderRequirements();
				_opaqueQuadPassesVector[i]->PassNextRequirements(*_carrier);
			}
		}

		//�� �̻� ���� �������� ���� �� �̷� ������ �Ҵ� �������ָ� �ȴ�.
		_DXStorage->_deviceContext->OMSetRenderTargets(0, nullptr, nullptr);

		//Quad �������ϴµ� ������ Resources�� Clear.
		//�� �̻� �Ⱦ��̴� Resource Slot�� -> nullptr�� ����.
		ID3D11ShaderResourceView* tNullSRV = nullptr;
		for (int i = 0; i < 7; i++)
		{
			_DXStorage->_deviceContext->PSSetShaderResources(i, 1, &tNullSRV);
		}
	}

	void DeferredRenderer::RenderOpaqueShadowPass(RenderObject3DList* renderObjectList, Pg::Data::CameraData* camData)
	{
		//Quad�� ��� �ִ� ���¿���, Light�� ��ġ�� �޾� ����.
		//MainLight���� ������, Single-DirectionalLight PCF Shadow Mapping�� �� ��.
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

		//Shadow ���� Depth ��� ����.
		_DXStorage->_deviceContext->PSSetShaderResources(23, 1, &tNullSRV);
		_DXStorage->_deviceContext->PSSetShaderResources(24, 1, &tNullSRV);
	}

	void DeferredRenderer::InitOpaqueQuadDirectX()
	{
		//�䱸�Ǵ� ���� ���ҽ� ����� (GBufferRender & Depth Stencil)
		_quadMainRTV = std::make_unique<GBufferRender>(DXGI_FORMAT_R32G32B32A32_TYPELESS, DXGI_FORMAT_R32G32B32A32_FLOAT);
		//ObjMat RenderTarget -> ���� PBR ���ۿ� swizzling�Ǿ� ���δ�!
		//_quadObjMatRTV = std::make_unique<GBufferRender>(DXGI_FORMAT_R32G32B32A32_TYPELESS, DXGI_FORMAT_R32G32B32A32_FLOAT);

		//Depth Writing�� ������ Description ����. (����� Default�� ����)
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

		//Carrier�� ���� �����Ѵ�. (MainRenderTarget ������ ��� �������� �⺻�� �� ��)
		_carrier->_quadMainRT = _quadMainRTV.get();
		_carrier->_quadMainGDS = _quadMainDSV.get();
		//_carrier->_quadObjMatRT_AoR = _quadObjMatRTV.get();

		//��ü���� OpaqueQuad DSV.
		_opaqueQuadDSV = std::make_unique<GBufferDepthStencil>();
	}

	void DeferredRenderer::InitFirstQuadDirectX()
	{
		//RT0
		_carrier->_gBufRequiredInfoRT.push_back(std::make_unique<GBufferRender>(DXGI_FORMAT_R32G32B32A32_TYPELESS, DXGI_FORMAT_R32G32B32A32_FLOAT));
		//RT1
		_carrier->_gBufRequiredInfoRT.push_back(std::make_unique<GBufferRender>(DXGI_FORMAT_R32G32B32A32_TYPELESS, DXGI_FORMAT_R32G32B32A32_FLOAT));
		//RT2
		_carrier->_gBufRequiredInfoRT.push_back(std::make_unique<GBufferRender>(DXGI_FORMAT_R32G32B32A32_TYPELESS, DXGI_FORMAT_R32G32B32A32_FLOAT));
		//RT3
		_carrier->_gBufRequiredInfoRT.push_back(std::make_unique<GBufferRender>(DXGI_FORMAT_R32G32B32A32_TYPELESS, DXGI_FORMAT_R32G32B32A32_FLOAT));
		//RT4
		_carrier->_gBufRequiredInfoRT.push_back(std::make_unique<GBufferRender>(DXGI_FORMAT_R32G32B32A32_TYPELESS, DXGI_FORMAT_R32G32B32A32_FLOAT));
		//RT5 -> ObjMatAoR
		_carrier->_gBufRequiredInfoRT.push_back(std::make_unique<GBufferRender>(DXGI_FORMAT_R32G32B32A32_TYPELESS, DXGI_FORMAT_R32G32B32A32_FLOAT));
		//RT6 -> AlbedoMetallic
		_carrier->_gBufRequiredInfoRT.push_back(std::make_unique<GBufferRender>(DXGI_FORMAT_R32G32B32A32_TYPELESS, DXGI_FORMAT_R32G32B32A32_FLOAT));
		//RT7 -> Normal Alpha
		_carrier->_gBufRequiredInfoRT.push_back(std::make_unique<GBufferRender>(DXGI_FORMAT_R32G32B32A32_TYPELESS, DXGI_FORMAT_R32G32B32A32_FLOAT));
		//(Depth)
		_carrier->_gBufRequiredInfoDSV = std::make_unique<GBufferDepthStencil>();

		//FirstStage_PS���� Binding�� Render Target��.
		//Depth�� �ڵ� ���� (DepthStencil ���ε� ���� ���� ����)
		for (auto& e : _carrier->_gBufRequiredInfoRT)
		{
			_carrier->_gBufRequiredRTVArray.push_back(e->GetRTV());
		}

		for (auto& e : _carrier->_gBufRequiredInfoRT)
		{
			_carrier->_gBufRequiredSRVArray.push_back(e->GetSRV());
		}

		//���ݱ��� ���ε��� ����ŭ RTV Null Array�� ������ش�.
		//DepthStencil�� ���̻� RTV�� ��ϵ��� ����.
		assert(_carrier->NullRTV.empty());
		_carrier->NullRTV.resize(_carrier->_gBufRequiredInfoRT.size());
		for (int i = 0; i < _carrier->_gBufRequiredInfoRT.size(); ++i)
		{
			ID3D11RenderTargetView* tNull = nullptr;
			_carrier->NullRTV.at(i) = tNull;
		}

		//���ݱ��� ���ε��� ����ŭ SRV Null Array�� ������ش�.
		assert(_carrier->NullSRV.empty());
		_carrier->NullSRV.resize(_carrier->_gBufRequiredSRVArray.size());
		for (int i = 0; i < _carrier->_gBufRequiredSRVArray.size(); ++i)
		{
			ID3D11ShaderResourceView* tNull = nullptr;
			_carrier->NullSRV.at(i) = tNull;
		}

		//Main Light DSV �Ҵ�.
		_carrier->_mainLightGBufRT = std::make_unique<GBufferRender>(DXGI_FORMAT_R32G32B32A32_TYPELESS, DXGI_FORMAT_R32G32B32A32_FLOAT, 
			DirectX::XMFLOAT2(Pg::Data::GameConstantData::SHADOW_MAP_LENGTH, Pg::Data::GameConstantData::SHADOW_MAP_LENGTH));
		_carrier->_mainLightGBufDSV = std::make_unique<GBufferDepthStencil>(nullptr, 
			DirectX::XMFLOAT2(Pg::Data::GameConstantData::SHADOW_MAP_LENGTH, Pg::Data::GameConstantData::SHADOW_MAP_LENGTH));
	}

	void DeferredRenderer::InitPBRDirectX()
	{
		//DepthStencil�� MainQuadDepthStencil�̴�. (Skinned�� ��������)
		//OpaqueQuad �ø�� ������ ������,
		//Rendering�� Main Render Target�� �Կ��� DepthStencil�� ��ü������ �����ؼ� ���� ���� (������ ���� ������ ���� ����)

		//���� ObjMat�� PBR ��� �Ϻδ� �Բ� ��ϵ�.
		//_carrier->_albedoMetallic_GBuffer = std::make_unique<GBufferRender>(DXGI_FORMAT_R32G32B32A32_TYPELESS, DXGI_FORMAT_R32G32B32A32_FLOAT);
		//_carrier->_normalAlpha_GBuffer = std::make_unique<GBufferRender>(DXGI_FORMAT_R32G32B32A32_TYPELESS, DXGI_FORMAT_R32G32B32A32_FLOAT);

		//�ϴ� ���� OMSetRenderTargets�� ���� ����.
		//ObjMat�� ���� _quadObjMatRT�� ����.
		_carrier->_pbrBindArray[0] = _carrier->_gBufRequiredRTVArray.at(5); //ObjMatAoR
		_carrier->_pbrBindArray[1] = _carrier->_gBufRequiredRTVArray.at(6); //AlbedoMetallic
		_carrier->_pbrBindArray[2] = _carrier->_gBufRequiredRTVArray.at(7); //NormalAlpha

		//NullRTV Array�� ����, nullptr ä���!
		std::fill(_carrier->_pbrNullBindArray.begin(), _carrier->_pbrNullBindArray.end(), nullptr);
	}

	void DeferredRenderer::SendPBRBufferSRVs()
	{
		//�� �̻� t3�� ObjMat ���۰� ���� ���� �ʴ´�. t12���� ���� ���� ���δ�.

		//t12-19 - GBuffer + ObjMatInternalPBRTextures Bind
		_DXStorage->_deviceContext->PSSetShaderResources(12, 5, _carrier->_gBufRequiredSRVArray.data());
		_DXStorage->_deviceContext->PSSetShaderResources(17, 1, &_carrier->_gBufRequiredSRVArray.at(5));
		_DXStorage->_deviceContext->PSSetShaderResources(18, 1, &_carrier->_gBufRequiredSRVArray.at(6));
		_DXStorage->_deviceContext->PSSetShaderResources(19, 1, &_carrier->_gBufRequiredSRVArray.at(7));
		
		//�������� IBL Texture��, ���⼭ ���ε�.
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

	void DeferredRenderer::UnbindPreviousBoundResources()
	{
		//VS / PS Constant Buffer -> SceneInfo �� ����.
		ID3D11Buffer* tNullBuffer = nullptr;
		_DXStorage->_deviceContext->VSSetConstantBuffers(4, 1, &tNullBuffer);
		_DXStorage->_deviceContext->PSSetConstantBuffers(4, 1, &tNullBuffer);

		//VS / PS Constant Buffer -> LightInfo �� ����.
		_DXStorage->_deviceContext->VSSetConstantBuffers(5, 1, &tNullBuffer);
		_DXStorage->_deviceContext->PSSetConstantBuffers(5, 1, &tNullBuffer);
	}

	void DeferredRenderer::UpdateCullingInformation(RenderObject3DList* renderObjectList, Pg::Data::CameraData* camData)
	{
		renderObjectList->UpdateObjectCullingState(camData);
	}

	void DeferredRenderer::UpdateShadowDSV()
	{
		_DXStorage->_deviceContext->PSSetShaderResources(23, 1, &(_carrier->_mainLightGBufDSV->GetSRV()));
		_DXStorage->_deviceContext->PSSetShaderResources(24, 1, &(_carrier->_gBufRequiredInfoDSV->GetSRV()));
	}
}


