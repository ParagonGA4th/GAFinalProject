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

#include "../ParagonData/GameObject.h"
#include "../ParagonData/Transform.h"
#include "../ParagonData/CameraData.h"
#include "../ParagonData/LightType.h"

namespace Pg::Graphics
{
	DeferredRenderer::DeferredRenderer()
	{
		_DXStorage = LowDX11Storage::GetInstance();
	}

	DeferredRenderer::~DeferredRenderer()
	{

	}

	void DeferredRenderer::Initialize()
	{
		PushRenderPasses();
		InitializeRenderPasses();
	}

	void DeferredRenderer::Render(RenderObject3DList* renderObjectList, Pg::Data::CameraData* camData)
	{
		//보관할 수 있는 포인터.
		std::vector<ID3D11RenderTargetView*>* tRTVArray = nullptr;
		UINT tRTVCount = 0;
		std::vector<ID3D11ShaderResourceView*>* tSRVArray = nullptr;
		UINT tSRVCount = 0;
		ID3D11DepthStencilView* tDSV = nullptr;

		//패스 외적으로 들어가야 하는 리소스들 GPU에 배치. 이 경우, SamplerState만 위로 배치.
		PlaceRequiredResources();

		//Pass를 순서대로 호출하는 방식.
		//나중에 Custom 동작이 필요하다고 한다면, 단순한 For문으로 안될 수도 있다.
		//여튼, 특정한 Pass에 값을 전달하는 코드가 있어야 할 것이다.
		for (auto& it : _renderPassVector)
		{
			it->ReceiveRequiredElements(tRTVArray, tRTVCount, tSRVArray, tSRVCount, tDSV);
			it->BindPass();
			it->RenderPass(renderObjectList, camData);
			it->UnbindPass();
			it->ExecuteNextRenderRequirements();
			it->PassNextRequirements(tRTVArray, tRTVCount, tSRVArray, tSRVCount, tDSV);
		}
	}

	void DeferredRenderer::PushRenderPasses()
	{
		//Render Pass Vector 구성.

		//첫번째는 무조건 FirstRenderPass.
		_renderPassVector.push_back(new FirstStaticRenderPass());

		//두번째는 일단 ObjMatStaticRenderPass.
		_renderPassVector.push_back(new ObjMatStaticRenderPass());
		//Skinned가 들어오면 FirstStatic->FirstSkinned->ObjMatStatic->ObjMatSkinned일것.

		//모든 Material의 목록을 받은 뒤, 순서대로 OpaqueQuadRenderPass 호출. (일반적인 경우)
		//N개의 Material이 있으면, N개의 Pass가 만들어진다.
		using Pg::Graphics::Manager::GraphicsResourceManager;
		auto tMatVec = GraphicsResourceManager::Instance()->GetAllResourcesByDefine(Data::Enums::eAssetDefine::_RENDERMATERIAL);
		for (auto& it : tMatVec)
		{
			RenderMaterial* tRM = static_cast<RenderMaterial*>(it.get());
			assert(tRM != nullptr);
			_renderPassVector.push_back(new OpaqueQuadRenderPass(tRM));
		}


	}

	void DeferredRenderer::InitializeRenderPasses()
	{
		//일괄적으로 Initialize() 호출.
		for (auto& it : _renderPassVector)
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
	

}


