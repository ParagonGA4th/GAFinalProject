#include "FogProcessingPass.h"
#include "LowDX11Storage.h"
#include "LayoutDefine.h"
#include "SystemVertexShader.h"
#include "SystemPixelShader.h"
#include "GeometryGenerator.h"

namespace Pg::Graphics
{

	FogProcessingPass::FogProcessingPass() : _DXStorage(LowDX11Storage::GetInstance())
	{

	}

	FogProcessingPass::~FogProcessingPass()
	{

	}

	void FogProcessingPass::Initialize()
	{

	}

	void FogProcessingPass::ReceiveRequiredElements(const D3DCarrier& carrier)
	{

	}

	void FogProcessingPass::BindPass()
	{

	}

	void FogProcessingPass::RenderPass(void* renderObjectList, Pg::Data::CameraData* camData)
	{

	}

	void FogProcessingPass::UnbindPass()
	{

	}

	void FogProcessingPass::ExecuteNextRenderRequirements()
	{

	}

	void FogProcessingPass::PassNextRequirements(D3DCarrier& gCarrier)
	{

	}

	//
	void FogProcessingPass::CreateShaders()
	{

	}

	void FogProcessingPass::BindVertexIndexBuffer()
	{

	}

}