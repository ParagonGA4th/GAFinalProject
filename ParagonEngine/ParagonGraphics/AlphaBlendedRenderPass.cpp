#include "AlphaBlendedRenderPass.h"
#include "GBufferRender.h"
#include "GBufferDepthStencil.h"
#include "LowDX11Storage.h"
#include "LayoutDefine.h"
#include "SystemVertexShader.h"
#include "RenderTexture2D.h"
#include "RenderCubemap.h"
#include "SystemVertexShader.h"
#include "SystemPixelShader.h"
#include "GraphicsResourceManager.h"
#include "../ParagonData/ParagonDefines.h"

#include <algorithm>  

namespace Pg::Graphics
{
	AlphaBlendedRenderPass::AlphaBlendedRenderPass(const Pg::Data::Enums::eEditorMode* const editorMode) :
		IRenderSinglePass(), _deltaTimeStorage(0.f), _editorMode(editorMode)
	{

	}

	AlphaBlendedRenderPass::~AlphaBlendedRenderPass()
	{

	}

	void AlphaBlendedRenderPass::Initialize()
	{

	}

	void AlphaBlendedRenderPass::ReceiveRequiredElements(const D3DCarrier& carrier)
	{

	}

	void AlphaBlendedRenderPass::BindPass()
	{

	}

	void AlphaBlendedRenderPass::RenderPass(void* renderObjectList, Pg::Data::CameraData* camData)
	{

	}

	void AlphaBlendedRenderPass::UnbindPass()
	{

	}

	void AlphaBlendedRenderPass::ExecuteNextRenderRequirements()
	{

	}

	void AlphaBlendedRenderPass::PassNextRequirements(D3DCarrier& gCarrier)
	{

	}

	void AlphaBlendedRenderPass::SetDeltaTime(float dt)
	{
		_deltaTimeStorage = dt;
	}

}