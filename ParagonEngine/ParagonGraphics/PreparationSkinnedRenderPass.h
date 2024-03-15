#pragma once

#pragma once
#include "IRenderSinglePass.h"
#include "DX11Headers.h"
#include "ConstantBuffer.h"
#include "ConstantBufferDefine.h"

#include <vector>
#include <memory>
#include <array>

/// <summary>
/// Object && Material이 Screen Space에서 어디 있는지 표시해주기 위해
/// (== ClipUnfit을 위해) 존재하는 RenderPass. (Skinned)
/// </summary>

namespace Pg::Graphics
{
	class GBufferRender;
	class GBufferDepthStencil;
	class LowDX11Storage;
	class SystemVertexShader;
	class SystemPixelShader;
	class RenderCubemap;
	class RenderTexture2D;
}

namespace Pg::Graphics
{
	class PreparationSkinnedRenderPass : public IRenderSinglePass
	{
	public:
		PreparationSkinnedRenderPass();
		~PreparationSkinnedRenderPass();

		virtual void Initialize() override;
		void SetDeltaTime(float dt);
		virtual void ReceiveRequiredElements(const D3DCarrier& carrier) override;
		virtual void BindPass() override;
		virtual void RenderPass(void* renderObjectList, Pg::Data::CameraData* camData) override;
		virtual void UnbindPass() override;
		virtual void ExecuteNextRenderRequirements() override;
		virtual void PassNextRequirements(D3DCarrier& gCarrier) override;


	private:
		float _deltaTimeStorage = 0.16f;

	private:
		LowDX11Storage* _DXStorage;
	};
}


