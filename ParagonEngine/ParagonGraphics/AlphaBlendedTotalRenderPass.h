#pragma once
#include "IRenderSinglePass.h"
#include "DX11Headers.h"
#include "ConstantBuffer.h"
#include "ConstantBufferDefine.h"
#include "../ParagonData/EditorMode.h"
#include <dxtk/CommonStates.h>
#include <vector>
#include <memory>

namespace Pg::Graphics
{
	class GBufferRender;
	class GBufferDepthStencil;
	class LowDX11Storage;
	class SystemVertexShader;
	class SystemPixelShader;
	class D3DCarrier;
}

/// <summary>
/// BlendingRenderPass - Skinned & Static ��� �ٷ��. VS / PS ���ε� �޶��� ���ۿ� ���� ��.
/// ���ο��� ������ ���̴�.
/// </summary>

namespace Pg::Graphics
{
	class AlphaBlendedTotalRenderPass : public IRenderSinglePass
	{
	public:
		AlphaBlendedTotalRenderPass(const Pg::Data::Enums::eEditorMode* const editorMode);
		~AlphaBlendedTotalRenderPass();

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
		const D3DCarrier* _carrier{ nullptr };
		std::unique_ptr<ConstantBuffer<ConstantBufferDefine::cbAlphaDefault>> _cbAlpha;

	private:
		LowDX11Storage* _DXStorage;
		std::unique_ptr<DirectX::CommonStates> _commonStates{ nullptr };
	};
}


