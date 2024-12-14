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
/// ���������� ������ ��ҵ��� ����� ����.
/// �ٷ� MainRenderTarget���� ������ ���޵ȴ�.
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

		//[DEPRECATED] Unsigned Int�� Picking�� ID�� �޴´�.
		[[deprecated("Not Using Picking Anymore")]]
		unsigned int GetPickingObjectID(unsigned int widthPixel, unsigned int heightPixel);

		void SetOutlineRenderingMode(bool val);
		void SetObjectIDSelected(unsigned int val);

		//�̸� �׸��� ������ Picking Logic���� ��������.
		void RenderOutlineStencil(Pg::Data::CameraData* camData);
		
		void ConnectDefaultResources();

	public:
		//API��
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
		//PostProcessing�� ���� ���� ���ؽ� ���̴� Ȱ��, �̸� �и��ߴ�.
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
