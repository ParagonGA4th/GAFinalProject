#pragma once
#include "../ParagonData/EditorMode.h"

#include "DX11Headers.h"
#include "GBufferRender.h"
#include "GBufferDepthStencil.h"
#include "BaseSpecificRenderer.h"
#include <vector>
#include <array>
#include <memory>

namespace Pg::Data
{
	class GameObject;
	class CameraData;
}

namespace Pg::Graphics
{
	class RenderObject3DList;
	class SceneInformationList;
	class LowDX11Storage;
	class IRenderSinglePass;

	class AlphaBlendedRenderPass;
}

/// <summary>
/// 미래를 위해 미리 Forward로 렌더링될 요소들 포함.
/// </summary>

namespace Pg::Graphics
{
	class Forward3DRenderer : public BaseSpecificRenderer
	{
	public:
		Forward3DRenderer(D3DCarrier* d3dCarrier, const Pg::Data::Enums::eEditorMode* const editorMode);

		virtual void Initialize() override;
		virtual void SetupRenderPasses() override;

		virtual void RenderContents(void* renderObjectList, void* optionalRequirement, Pg::Data::CameraData* camData) override;
		virtual void ConfirmCarrierData() override;

		//매 프레임마다 Skinned 동작을 위해 사용.
		void SetDeltaTime(float dt);

	private:
		void RenderAlphaBlendedTotalPass(RenderObject3DList* renderObjectList, Pg::Data::CameraData* camData);


	private:
		std::unique_ptr<AlphaBlendedRenderPass> _alphaBlendedRenderPass;
	private:
		LowDX11Storage* _DXStorage;
		float _deltaTimeStorage;

	private:
		const Pg::Data::Enums::eEditorMode* const _editorMode;

	};
}


