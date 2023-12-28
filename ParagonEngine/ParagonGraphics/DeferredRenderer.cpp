#include "DeferredRenderer.h"
#include "LowDX11Storage.h"
#include "LayoutDefine.h"
#include "GraphicsResourceManager.h"
#include "RenderMaterial.h"

#include "RenderObject3D.h"
#include "RenderObject3DList.h"

//RenderPasses
#include "IRenderPass.h"
#include "FirstRenderPass.h"
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
		//Pass를 순서대로 호출하는 방식.
		for (auto& it : _renderPassVector)
		{
			it->BindPass();
			it->RenderPass(renderObjectList, camData);
			it->UnbindPass();
			it->SetupNextRequirements();
		}
	}

	void DeferredRenderer::PushRenderPasses()
	{
		//Render Pass Vector 구성.

		//첫번째는 무조건 FirstRenderPass.
		_renderPassVector.push_back(new FirstRenderPass());

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

}


