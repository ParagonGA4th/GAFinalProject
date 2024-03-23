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

	PreparationSkinnedRenderPass::PreparationSkinnedRenderPass()
	{
		_DXStorage = LowDX11Storage::GetInstance();
	}

	PreparationSkinnedRenderPass::~PreparationSkinnedRenderPass()
	{

	}

	void PreparationSkinnedRenderPass::Initialize()
	{
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

	}

	void PreparationSkinnedRenderPass::RenderPass(void* renderObjectList, Pg::Data::CameraData* camData)
	{

	}

	void PreparationSkinnedRenderPass::UnbindPass()
	{

	}

	void PreparationSkinnedRenderPass::ExecuteNextRenderRequirements()
	{
		//만약 Skinned가 들어온다면, 이 코드는 ObjMatSkinnedRenderPass로 가야 한다.
		//당연히 GBuffer-DepthStencil 역시 옮겨받아야 하고.

		//t3에, ObjMat GBuffer가 들어간다. 대응. (Depth 제외)
		_DXStorage->_deviceContext->PSSetShaderResources(3, 1, &(_d3dCarrierStorage->_quadObjMatRT->GetSRV()));

		//t12-14 - internalPBRTextures Bind
		_DXStorage->_deviceContext->PSSetShaderResources(12, 1, &(_d3dCarrierStorage->_albedoAmbiBuffer->GetSRV()));
		_DXStorage->_deviceContext->PSSetShaderResources(13, 1, &(_d3dCarrierStorage->_normalRoughBuffer->GetSRV()));
		_DXStorage->_deviceContext->PSSetShaderResources(14, 1, &(_d3dCarrierStorage->_specularMetalBuffer->GetSRV()));

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

}