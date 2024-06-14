#include "BillboardRenderPass.h"
#include "LowDX11Storage.h"
#include "LayoutDefine.h"
#include "SystemVertexShader.h"
#include "SystemPixelShader.h"
#include "GeometryGenerator.h"

#include "../ParagonHelper/CSVHelper.h"

namespace Pg::Graphics
{
	BillboardRenderPass::BillboardRenderPass(const std::string& resourceListPath) : 
		_DXStorage(LowDX11Storage::GetInstance()), _resourceListPath(resourceListPath)
	{

	}

	BillboardRenderPass::~BillboardRenderPass()
	{

	}

	void BillboardRenderPass::Initialize()
	{	
		_visualEffectController = std::make_unique<VisualEffectRenderer>();

		//std::string tPath = _resourceListPath + "/15_VisualEffectList.csv";
		//_visualEffectController->LoadAllEffects(Pg::Util::Helper::CSVHelper::ReturnDataFromVisualEffectCSV(tPath));
	
	}

	void BillboardRenderPass::ReceiveRequiredElements(const D3DCarrier& carrier)
	{

	}

	void BillboardRenderPass::BindPass()
	{
			
	}

	void BillboardRenderPass::RenderPass(void* renderObjectList, Pg::Data::CameraData* camData)
	{

	}

	void BillboardRenderPass::UnbindPass()
	{

	}

	void BillboardRenderPass::ExecuteNextRenderRequirements()
	{

	}

	void BillboardRenderPass::PassNextRequirements(D3DCarrier& gCarrier)
	{

	}

	void BillboardRenderPass::RegisterAllEffects()
	{

	}

}
