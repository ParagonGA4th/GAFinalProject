#include "FinalRenderer.h"

#include "LowDX11Storage.h"
#include "LayoutDefine.h"
#include "GraphicsResourceManager.h"



#include "../ParagonData/GameObject.h"
#include "../ParagonData/Transform.h"
#include "../ParagonData/CameraData.h"
#include "../ParagonData/LightType.h"

namespace Pg::Graphics
{
	FinalRenderer::FinalRenderer(D3DCarrier* d3dCarrier) : BaseSpecificRenderer(d3dCarrier)
	{

	}

	void FinalRenderer::Initialize()
	{
		_finalRenderPass = std::make_unique<FinalRenderPass>();
	}

	void FinalRenderer::SetupRenderPasses()
	{
		_finalRenderPass->Initialize();
	}

	void FinalRenderer::RenderContents(void* renderObjectList, Pg::Data::CameraData* camData)
	{
		////SRV Index 0 : Opaque Quad의 인덱스 전달.
		//_carrier->_srvArray[0] = _opaqueQuadRTV->GetSRV();
		////SRV Index 1 : ObjMat 레이어에서 나온 뎁스의 SRV 전달.
		//_carrier->_srvArray[1] = _objMatDepthSRV;

		//Final Render Pass.
		_finalRenderPass->ReceiveRequiredElements(*_carrier);
		_finalRenderPass->BindPass();
		_finalRenderPass->RenderPass(renderObjectList, camData);
		_finalRenderPass->UnbindPass();
		_finalRenderPass->ExecuteNextRenderRequirements();
		_finalRenderPass->PassNextRequirements(*_carrier);
	}

	void FinalRenderer::ConfirmCarrierData()
	{

	}

	unsigned int FinalRenderer::GetPickingID(unsigned int widthPixel, unsigned int widthHeight)
	{
		return 0;
	}

}