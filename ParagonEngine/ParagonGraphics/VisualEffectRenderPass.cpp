#include "VisualEffectRenderPass.h"
#include "LowDX11Storage.h"
#include "LayoutDefine.h"
#include "SystemVertexShader.h"
#include "SystemPixelShader.h"
#include "D3DCarrier.h"
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

		std::string tPath = _resourceListPath + "/15_VisualEffectList.csv";
		_visualEffectController->LoadAllEffects(Pg::Util::Helper::CSVHelper::ReturnDataFromVisualEffectCSV(tPath));
	}

	void VisualEffectRenderPass::ReceiveRequiredElements(const D3DCarrier& carrier)
	{
		//Carrier 포인터 받기.
		_carrier = &carrier;
	}

	void VisualEffectRenderPass::BindPass()
	{
			
	}

	void VisualEffectRenderPass::RenderPass(void* renderObjectList, Pg::Data::CameraData* camData)
	{
		//실제 이펙트 렌더 로직.
		_visualEffectController->Render(_carrier, camData);
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
