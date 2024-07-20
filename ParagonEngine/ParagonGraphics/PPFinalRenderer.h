#pragma once
#include "SystemVertexShader.h"
#include "SystemPixelShader.h"
#include "DebugOverlayQuad.h"
#include "BaseSpecificRenderer.h"
#include "DX11Headers.h"

//RenderPass
#include "OutlineRenderPass.h"
#include "FinalRenderPass.h"
#include "HeightFogRenderPass.h"
#include "VignetteRenderPass.h"
#include "BloomRenderPass.h"
#include "LUTRenderPass.h"

#include "FadeInOutPass.h"

//DebugOverlayQuads
#include "DebugOverlayQuad.h"

namespace Pg::Data
{
	class GameObject;
	class CameraData;
}

namespace Pg::Graphics
{
	class LowDX11Storage;
	class SystemVertexShader;
	class DebugOverlayQuad;
}

namespace Pg::Util
{
	namespace Time { class TimeSystem; }
}

/// <summary>
/// 마지막으로 렌더될 요소들이 여기로 전달.
/// 바로 MainRenderTarget으로 정보가 전달된다.
/// </summary>

namespace Pg::Graphics
{
	class PPFinalRenderer : public BaseSpecificRenderer
	{
	public:
		PPFinalRenderer(D3DCarrier* d3dCarrier);
		
		virtual void Initialize() override;
		virtual void SetupRenderPasses() override;

		void RenderPostProcessingStages(void* renderObjectList, Pg::Data::CameraData* camData);
		void RenderFadeInOut();
		void RenderDebugQuadsOverlay(); // TO ADD.

		virtual void RenderContents(void* renderObjectList, void* optionalRequirement, Pg::Data::CameraData* camData) override;
		virtual void ConfirmCarrierData() override;

		//[DEPRECATED] Unsigned Int로 Picking된 ID를 받는다.
		[[deprecated("Not Using Picking Anymore")]]
		unsigned int GetPickingObjectID(unsigned int widthPixel, unsigned int heightPixel);

		void SetOutlineRenderingMode(bool val);
		void SetObjectIDSelected(unsigned int val);

		//미리 그릴지 말지가 Picking Logic으로 정해진다.
		void RenderOutlineStencil(Pg::Data::CameraData* camData);
		
		void ConnectDefaultResources();

	public:
		//API용
		void Effect_FadeIn();
		void Effect_FadeOut();

		bool GetIsFadingIn() { return _isFadingIn; }
		bool GetIsFadingOut() { return _isFadingOut; }

	private:
		float _fadeEffectSourceRatio{ 1.f };
		bool _isFadingIn{ false };
		bool _isFadingOut{ false };

	private:
		void InitPostProcessingQuads();
		void CreateStagingPickingBuffer();
		void CreateDebugOverlayQuads(); // TO ADD.

	private:
		void WhenActivatedUpdateFadeLogic();
	private:
		//PostProcessing은 전부 같은 버텍스 셰이더 활용, 이를 분리했다.
		std::unique_ptr<SystemVertexShader> _ppSystemVertexShader;
	private:
		std::unique_ptr<OutlineRenderPass> _outlineRenderPass;
		std::unique_ptr<FinalRenderPass> _finalRenderPass;

		//PostProcessing Passes.
		std::vector<std::unique_ptr<IRenderSinglePass>> _postprocessingRenderPassList;

		std::unique_ptr<FadeInOutPass> _fadeInOutPass;

		//DebugOverlayQuads
		std::vector<std::unique_ptr<DebugOverlayQuad>> _debugOverlayQuadList;
	

		ID3D11Texture2D* _pickingStagingBuffer = nullptr;
		LowDX11Storage* _DXStorage = nullptr;

		bool _outlineRenderingMode = false;
		unsigned int _pickedObjID = 0;

		std::unique_ptr<GBufferRender> _postProcessingBuffer1;
		std::unique_ptr<GBufferRender> _postProcessingBuffer2;
	
		Pg::Util::Time::TimeSystem* _timeSystem{ nullptr };

	private:
		//ShadowMap Quad.
		std::unique_ptr<DebugOverlayQuad> _debugOverlayShadowQuad;
	};
}
//finalRenderer->RenderOutlineStencil();
