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

	}

	void PreparationSkinnedRenderPass::SetDeltaTime(float dt)
	{
		_deltaTimeStorage = dt;
	}

	void PreparationSkinnedRenderPass::ReceiveRequiredElements(const D3DCarrier& carrier)
	{

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

	}

	void PreparationSkinnedRenderPass::PassNextRequirements(D3DCarrier& gCarrier)
	{

	}

}