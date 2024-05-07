#include "PreparationSkinnedRenderPass.h"
#include "GBufferRender.h"
#include "GBufferDepthStencil.h"
#include "LowDX11Storage.h"
#include "LayoutDefine.h"
#include "SystemVertexShader.h"
#include "RenderTexture2D.h"
#include "RenderCubemap.h"
#include "SystemPixelShader.h"
#include "GraphicsResourceManager.h"
#include "../ParagonData/ParagonDefines.h"

#include <algorithm> 

namespace Pg::Graphics
{

	PreparationSkinnedRenderPass::PreparationSkinnedRenderPass(const Pg::Data::Enums::eEditorMode* const editorMode) : _editorMode(editorMode)
	{
		_DXStorage = LowDX11Storage::GetInstance();
	}

	PreparationSkinnedRenderPass::~PreparationSkinnedRenderPass()
	{

	}

	void PreparationSkinnedRenderPass::Initialize()
	{
		CreateShaders();
		FetchIBLBuffers();
	}

	void PreparationSkinnedRenderPass::SetDeltaTime(float dt)
	{
		_deltaTimeStorage = dt;
	}

	void PreparationSkinnedRenderPass::ReceiveRequiredElements(const D3DCarrier& carrier)
	{
		_d3dCarrierStorage = &carrier;
	}

	void PreparationSkinnedRenderPass::BindPass()
	{
		//이미 Static에서 OMSetRenderTarget들은 원한대로 설정되었다.
		//_DXStorage->_deviceContext->OMSetRenderTargets(_d3dCarrierStorage->_pbrBindArray.size(), _d3dCarrierStorage->_pbrBindArray.data(), _d3dCarrierStorage->_quadMainGDS->GetDSV());
		//_DXStorage->_deviceContext->OMSetRenderTargets(1, &(_gBufferRender->GetRTV()), _DXStorage->_depthStencilView);

		_vs->Bind();
		_ps->Bind();
	}

	void PreparationSkinnedRenderPass::RenderPass(void* renderObjectList, Pg::Data::CameraData* camData)
	{
		RenderObject3DList* tRenderObjectList = reinterpret_cast<RenderObject3DList*>(renderObjectList);

		//모든 오브젝트 렌더링.
		for (auto& it : tRenderObjectList->_skinnedList)
		{
			//Vector
			for (int i = 0; i < it.second->size(); i++)
			{
				if (it.second->at(i).second->GetBaseRenderer()->GetActive())
				{
					it.second->at(i).second->ObjMat_UpdateConstantBuffers(camData);
					it.second->at(i).second->ObjMat_BindBuffers();
					it.second->at(i).second->ObjMat_Render(&_deltaTimeStorage);
					it.second->at(i).second->ObjMat_UnbindBuffers();
				}
			}
		}
	}

	void PreparationSkinnedRenderPass::UnbindPass()
	{
		// Unbind RenderTarget
		//더 이상 값을 설정하지 않을 때 이런 식으로 할당 해제해주면 된다.
		//여기에서는 해주어야 함.
		_DXStorage->_deviceContext->OMSetRenderTargets(_d3dCarrierStorage->_pbrNullBindArray.size(), _d3dCarrierStorage->_pbrNullBindArray.data(), nullptr);

		// Unbind Shaders
		_vs->Unbind();
		_ps->Unbind();
	}

	void PreparationSkinnedRenderPass::ExecuteNextRenderRequirements()
	{
		//만약 Skinned가 들어온다면, 이 코드는 ObjMatSkinnedRenderPass로 가야 한다.
		//당연히 GBuffer-DepthStencil 역시 옮겨받아야 하고.

		//더 이상 t3에 ObjMat 버퍼가 새로 들어가지 않는다. t12에서 같이 들어가서 쓰인다.

		//t12-14 - ObjMat GBuffer + InternalPBRTextures Bind
		_DXStorage->_deviceContext->PSSetShaderResources(12, 1, &(_d3dCarrierStorage->_quadObjMatRT_AoR->GetSRV()));
		_DXStorage->_deviceContext->PSSetShaderResources(13, 1, &(_d3dCarrierStorage->_albedoMetallic_GBuffer->GetSRV()));
		_DXStorage->_deviceContext->PSSetShaderResources(14, 1, &(_d3dCarrierStorage->_normalAlpha_GBuffer->GetSRV()));

		//독립적인 IBL Texture들, 여기서 바인딩.
		//t21-23 - internal IBL TextureCubes Bind
		_DXStorage->_deviceContext->PSSetShaderResources(21, 1, &(_iblDiffuseIrradianceMap->GetSRV()));
		_DXStorage->_deviceContext->PSSetShaderResources(22, 1, &(_iblSpecularIrradianceMap->GetSRV()));
		_DXStorage->_deviceContext->PSSetShaderResources(23, 1, &(_iblSpecularLutTextureMap->GetSRV()));
	}

	void PreparationSkinnedRenderPass::PassNextRequirements(D3DCarrier& gCarrier)
	{

	}

	void PreparationSkinnedRenderPass::FetchIBLBuffers()
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

	void PreparationSkinnedRenderPass::CreateShaders()
	{
		//ObjMatSkinned 용도 셰이더 갖고 오기.
		_vs = std::make_unique<SystemVertexShader>(L"../Builds/x64/Debug/Individual_PerObjMatSkinnedVS.cso", LayoutDefine::GetPerObjMatSkinnedLayout(),
			LowDX11Storage::GetInstance()->_solidState, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		_ps = std::make_unique<SystemPixelShader>(L"../Builds/x64/Debug/Individual_PerObjMatPS.cso");
	}

}