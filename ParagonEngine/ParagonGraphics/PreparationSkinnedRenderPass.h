#pragma once

#pragma once
#include "IRenderSinglePass.h"
#include "DX11Headers.h"
#include "ConstantBuffer.h"
#include "ConstantBufferDefine.h"
#include "../ParagonData/EditorMode.h"

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
	class D3DCarrier;
}

namespace Pg::Graphics
{
	class PreparationSkinnedRenderPass : public IRenderSinglePass
	{
	public:
		PreparationSkinnedRenderPass(const Pg::Data::Enums::eEditorMode* const editorMode);
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
		void FetchIBLBuffers();
		void CreateShaders();

	private:
		std::unique_ptr<SystemVertexShader> _vs;
		std::unique_ptr<SystemPixelShader> _ps;

	private:
		float _deltaTimeStorage = 0.16f;
		const D3DCarrier* _d3dCarrierStorage = nullptr;

	private:
		LowDX11Storage* _DXStorage;

		//IBL Textures. (Textures 21-22)
		RenderCubemap* _iblDiffuseIrradianceMap = nullptr;
		RenderCubemap* _iblSpecularIrradianceMap = nullptr;
		RenderTexture2D* _iblSpecularLutTextureMap = nullptr;

	private:
		const Pg::Data::Enums::eEditorMode* const _editorMode;
	};
}


