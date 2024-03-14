#pragma once
#include "SystemVertexShader.h"
#include "SystemPixelShader.h"
#include "BaseSpecificRenderer.h"
#include "DX11Headers.h"

//RenderPass
#include "OutlineRenderPass.h"
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

		virtual void RenderContents(void* renderObjectList, void* optionalRequirement, Pg::Data::CameraData* camData) override;
		virtual void ConfirmCarrierData() override;

		//Unsigned Int로 Picking된 ID를 받는다.
		unsigned int GetPickingObjectID(unsigned int widthPixel, unsigned int heightPixel);
		void SetOutlineRenderingMode(bool val);
		void SetObjectIDSelected(unsigned int val);

		//미리 그릴지 말지가 Picking Logic으로 정해진다.
		void RenderOutlineStencil(Pg::Data::CameraData* camData);

	private:
		void CreateStagingPickingBuffer();



	private:
		std::unique_ptr<OutlineRenderPass> _outlineRenderPass;
		std::unique_ptr<FinalRenderPass> _finalRenderPass;

		ID3D11Texture2D* _pickingStagingBuffer = nullptr;
		LowDX11Storage* _DXStorage = nullptr;

		bool _outlineRenderingMode = false;
		unsigned int _pickedObjID = 0;
	};
}
//finalRenderer->RenderOutlineStencil();
