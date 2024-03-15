#include "FirstSkinnedRenderPass.h"
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

	FirstSkinnedRenderPass::FirstSkinnedRenderPass()
	{
		_DXStorage = LowDX11Storage::GetInstance();
	}

	FirstSkinnedRenderPass::~FirstSkinnedRenderPass()
	{

	}

	void FirstSkinnedRenderPass::Initialize()
	{

	}

	void FirstSkinnedRenderPass::SetDeltaTime(float dt)
	{
		_deltaTimeStorage = dt;
	}

	void FirstSkinnedRenderPass::ReceiveRequiredElements(const D3DCarrier& carrier)
	{

	}

	void FirstSkinnedRenderPass::BindPass()
	{

	}

	void FirstSkinnedRenderPass::RenderPass(void* renderObjectList, Pg::Data::CameraData* camData)
	{

	}

	void FirstSkinnedRenderPass::UnbindPass()
	{

	}

	void FirstSkinnedRenderPass::ExecuteNextRenderRequirements()
	{

	}

	void FirstSkinnedRenderPass::PassNextRequirements(D3DCarrier& gCarrier)
	{

	}

	

}
