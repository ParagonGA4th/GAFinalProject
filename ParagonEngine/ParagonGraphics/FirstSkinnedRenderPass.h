#pragma once
#pragma once
#include "IRenderSinglePass.h"
#include "../ParagonData/EditorMode.h"
#include "DX11Headers.h"
#include "ConstantBuffer.h"
#include "ConstantBufferDefine.h"

#include <vector>
#include <memory>

/// <summary>
/// First Render Pass. World Space -> Screen Space. (Skinned)
/// </summary>

namespace Pg::Graphics
{
	class GBufferRender;
	class GBufferDepthStencil;
	class LowDX11Storage;
	class SystemVertexShader;
	class SystemInterfacedVertexShader;
	class SystemPixelShader;
}

namespace Pg::Graphics
{
	class FirstSkinnedRenderPass : public IRenderSinglePass
	{
	public:
		FirstSkinnedRenderPass(const Pg::Data::Enums::eEditorMode* const editorMode);
		~FirstSkinnedRenderPass();

		virtual void Initialize() override;
		void SetDeltaTime(float dt);
		virtual void ReceiveRequiredElements(const D3DCarrier& carrier) override;
		virtual void BindPass() override;
		virtual void RenderPass(void* renderObjectList, Pg::Data::CameraData* camData) override;
		virtual void UnbindPass() override;
		virtual void ExecuteNextRenderRequirements() override;
		virtual void PassNextRequirements(D3DCarrier& gCarrier) override;

	private:
		void CreateShaders();

	private:
		std::unique_ptr<SystemVertexShader> _vs;
		std::unique_ptr<SystemPixelShader> _ps;
		std::unique_ptr<SystemPixelShader> _depthRecordOnlyPS;

	private:
		float _deltaTimeStorage{ 0.f };
		LowDX11Storage* _DXStorage{ nullptr };
		const D3DCarrier* _d3dCarrierTempStorage{ nullptr };
		std::unique_ptr<ConstantBuffer<ConstantBufferDefine::cbSwitchableViewProj>> _switchableViewProjCBuffer{ nullptr };

	private:
		const Pg::Data::Enums::eEditorMode* const _editorMode;
	};
}


