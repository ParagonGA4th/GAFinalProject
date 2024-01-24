#pragma once
#include "SystemVertexShader.h"
#include "SystemPixelShader.h"
#include "BaseSpecificRenderer.h"

//RenderPass
#include "FinalRenderPass.h"

namespace Pg::Data
{
	class GameObject;
	class CameraData;
}

namespace Pg::Graphics
{
	class LowDX11Storage;
}

/// <summary>
/// 마지막으로 렌더될 요소들이 여기로 전달.
/// 바로 MainRenderTarget으로 정보가 전달된다.
/// </summary>

namespace Pg::Graphics
{
	class FinalRenderer : public BaseSpecificRenderer
	{
	public:
		FinalRenderer(D3DCarrier* d3dCarrier);
		
		virtual void Initialize() override;
		virtual void SetupRenderPasses() override;

		virtual void RenderContents(void* renderObjectList, Pg::Data::CameraData* camData) override;
		virtual void ConfirmCarrierData() override;


	private:
		std::unique_ptr<FinalRenderPass> _finalRenderPass;
	};
}

