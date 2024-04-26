#pragma once
#include "IRenderSinglePass.h"
#include "DX11Headers.h"
#include "../ParagonData/EditorMode.h"
#include <vector>
#include <memory>

namespace Pg::Graphics
{
	class GBufferRender;
	class GBufferDepthStencil;
	class LowDX11Storage;
	class SystemVertexShader;
	class SystemPixelShader;
}

/// <summary>
/// BlendingRenderPass
/// </summary>

namespace Pg::Graphics
{
	class AlphaBlendedRenderPass : public IRenderSinglePass
	{
	public:
		AlphaBlendedRenderPass(const Pg::Data::Enums::eEditorMode* const editorMode);
		~AlphaBlendedRenderPass();

		virtual void Initialize() override;
		void SetDeltaTime(float dt);
		virtual void ReceiveRequiredElements(const D3DCarrier& carrier) override;
		virtual void BindPass() override;
		virtual void RenderPass(void* renderObjectList, Pg::Data::CameraData* camData) override;
		virtual void UnbindPass() override;
		virtual void ExecuteNextRenderRequirements() override;
		virtual void PassNextRequirements(D3DCarrier& gCarrier) override;



	private:
		float _deltaTimeStorage;
		const Pg::Data::Enums::eEditorMode* const _editorMode;
	};
}


