#pragma once
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

	class FirstStaticRenderPass;
	class FirstSkinnedRenderPass;
	class PreparationStaticRenderPass;
	class PreparationSkinnedRenderPass;
	class SceneInformationSender;
	class OpaqueShadowRenderPass;
}

namespace Pg::Graphics
{
	class DeferredRenderer : public BaseSpecificRenderer
	{
	public:
		DeferredRenderer(D3DCarrier* d3dCarrier);
		~DeferredRenderer();

		virtual void Initialize() override;
		virtual void SetupRenderPasses() override;

		//매 프레임마다 Skinned 동작을 위해 사용.
		void SetDeltaTime(float dt);
		virtual void RenderContents(void* renderObjectList, void* optionalRequirement, Pg::Data::CameraData* camData) override;
		virtual void ConfirmCarrierData() override;

	private:
		void PushRenderPasses();
		void InitializeRenderPasses();
		void PlaceRequiredResources();
		void UpdateCarrierResources();
	private:
		void InitOpaqueQuadDirectX();
		void InitFirstQuadDirectX();
		void InitPBRDirectX();


	private:
		void Render(RenderObject3DList* renderObjectList, SceneInformationList* sceneInfoList, Pg::Data::CameraData* camData);
		void RenderFirstStaticPass(RenderObject3DList* renderObjectList, Pg::Data::CameraData* camData);
		void RenderFirstSkinnedPass(RenderObject3DList* renderObjectList, Pg::Data::CameraData* camData);
		void RenderObjMatStaticPass(RenderObject3DList* renderObjectList, Pg::Data::CameraData* camData);
		void RenderObjMatSkinnedPass(RenderObject3DList* renderObjectList, Pg::Data::CameraData* camData);
		void SendSceneInformation(SceneInformationList* infoList, Pg::Data::CameraData* camData);
		void RenderOpaqueQuadPasses(RenderObject3DList* renderObjectList, Pg::Data::CameraData* camData);
		void RenderOpaqueShadowPass(RenderObject3DList* renderObjectList, Pg::Data::CameraData* camData);
		void UnbindExpiredResources();

	private:
		LowDX11Storage* _DXStorage;

		std::unique_ptr<FirstStaticRenderPass> _firstStaticRenderPass;
		std::unique_ptr<FirstSkinnedRenderPass> _firstSkinnedRenderPass;
		std::unique_ptr<PreparationStaticRenderPass> _objMatStaticRenderPass;
		std::unique_ptr<PreparationSkinnedRenderPass> _objMatSkinnedRenderPass;
		std::unique_ptr<SceneInformationSender> _sceneInformationSender;
		std::vector<IRenderSinglePass*> _opaqueQuadPassesVector;
		std::unique_ptr<OpaqueShadowRenderPass> _opaqueShadowPass;


		//메인 렌더 타겟으로 넘어갈 G-Buffer Render & Depth Stencil.
		//모든 Renderer를 거치면서 값이 활용될 것이다.
		std::unique_ptr<GBufferRender> _quadMainRTV;
		std::unique_ptr<GBufferDepthStencil> _quadMainDSV;
		std::unique_ptr<GBufferRender> _quadObjMatRTV;


		//별도로 OpaqueQuad가 사용하는 DSV. (ObjMat 딴에서 기록된 Depth 값을 훼손하지 않기 위해서)
		std::unique_ptr<GBufferDepthStencil> _opaqueQuadDSV;

	private:
		//NullSRV, 그냥 만들어놓고 사용.
		std::array<ID3D11ShaderResourceView*, 10> _nullSRVArray;

	private:
		float _deltaTimeStorage;
	};
}