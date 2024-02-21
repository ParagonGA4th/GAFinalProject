#include "OpaqueShadowRenderPass.h"
#include "LowDX11Storage.h"
#include "LayoutDefine.h"
#include "SystemVertexShader.h"
#include "SystemPixelShader.h"
#include "GeometryGenerator.h"

namespace Pg::Graphics
{
	OpaqueShadowRenderPass::OpaqueShadowRenderPass() : _DXStorage(LowDX11Storage::GetInstance())
	{
		//{0.5, -1, 0.5} Normalize한 것.
		_mainLightDir = { 0.408248f, -0.816497f, 0.408248f };
	}

	OpaqueShadowRenderPass::~OpaqueShadowRenderPass()
	{

	}

	void OpaqueShadowRenderPass::Initialize()
	{

	}

	void OpaqueShadowRenderPass::ReceiveRequiredElements(const D3DCarrier& carrier)
	{

	}

	void OpaqueShadowRenderPass::BindPass()
	{
		//MainLight의 시점에서 Render한 DepthMap이 있을 것이다.

		
	}

	void OpaqueShadowRenderPass::RenderPass(void* renderObjectList, Pg::Data::CameraData* camData)
	{

	}

	void OpaqueShadowRenderPass::UnbindPass()
	{

	}

	void OpaqueShadowRenderPass::ExecuteNextRenderRequirements()
	{

	}

	void OpaqueShadowRenderPass::PassNextRequirements(D3DCarrier& gCarrier)
	{

	}

	//Internal
	void OpaqueShadowRenderPass::CreateShaders()
	{

	}

	void OpaqueShadowRenderPass::BindVertexIndexBuffer()
	{

	}

}