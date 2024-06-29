#include "VisualEffectRenderPass.h"
#include "LowDX11Storage.h"
#include "LayoutDefine.h"
#include "SystemVertexShader.h"
#include "SystemPixelShader.h"
#include "GeometryGenerator.h"

#include "../ParagonHelper/CSVHelper.h"

namespace Pg::Graphics
{
	VisualEffectRenderPass::VisualEffectRenderPass(const std::string& resourceListPath) : 
		_DXStorage(LowDX11Storage::GetInstance()), _resourceListPath(resourceListPath)
	{

	}

	VisualEffectRenderPass::~VisualEffectRenderPass()
	{

	}

	void VisualEffectRenderPass::Initialize()
	{	
		_visualEffectController = std::make_unique<VisualEffectRenderer>();

		//std::string tPath = _resourceListPath + "/15_VisualEffectList.csv";
		//_visualEffectController->LoadAllEffects(Pg::Util::Helper::CSVHelper::ReturnDataFromVisualEffectCSV(tPath));
	
	}

	void VisualEffectRenderPass::ReceiveRequiredElements(const D3DCarrier& carrier)
	{

	}

	void VisualEffectRenderPass::BindPass()
	{
			
	}

	void VisualEffectRenderPass::RenderPass(void* renderObjectList, Pg::Data::CameraData* camData)
	{

	}

	void VisualEffectRenderPass::UnbindPass()
	{

	}

	void VisualEffectRenderPass::ExecuteNextRenderRequirements()
	{

	}

	void VisualEffectRenderPass::PassNextRequirements(D3DCarrier& gCarrier)
	{

	}

	void VisualEffectRenderPass::RegisterAllEffects()
	{

	}

}
