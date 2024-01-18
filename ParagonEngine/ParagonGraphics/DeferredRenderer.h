#pragma once
#include "DX11Headers.h"
#include "GBufferRender.h"
#include "GBufferDepthStencil.h"
#include "BaseSpecificRenderer.h"
#include <vector>
#include <memory>

namespace Pg::Data
{
	class GameObject;
	class CameraData;
}

namespace Pg::Graphics
{
	class RenderObject3DList;
	class LowDX11Storage;
	class IRenderPass;

	class FirstStaticRenderPass;
	class ObjMatStaticRenderPass;
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

		virtual void RenderContents(void* renderObjectList, Pg::Data::CameraData* camData) override;
		virtual void ConfirmCarrierData() override;

	private:
		void PushRenderPasses();
		void InitializeRenderPasses();
		void PlaceRequiredResources();
		void UpdateCarrierResources();

	private:
		void Render(RenderObject3DList* renderObjectList, Pg::Data::CameraData* camData);
		void RenderFirstStaticPass(RenderObject3DList* renderObjectList, Pg::Data::CameraData* camData);
		void RenderObjMatStaticPass(RenderObject3DList* renderObjectList, Pg::Data::CameraData* camData);
		void RenderOpaqueQuadPasses(RenderObject3DList* renderObjectList, Pg::Data::CameraData* camData);

	private:
		LowDX11Storage* _DXStorage;

		FirstStaticRenderPass* _firstStaticRenderPass;
		ObjMatStaticRenderPass* _objMatStaticRenderPass;

		std::vector<IRenderPass*> _opaqueQuadPassesVector;

		//메인 렌더 타겟으로 넘어갈 G-Buffer Render & Depth Stencil.
		//모든 Renderer를 거치면서 값이 활용될 것이다.
		std::unique_ptr<GBufferRender> _quadMainRTV;
		std::unique_ptr<GBufferDepthStencil> _quadMainDSV;

		//별도로 OpaqueQuad가 사용하는 DSV. (ObjMat 딴에서 기록된 Depth 값을 훼손하지 않기 위해서)
		std::unique_ptr<GBufferDepthStencil> _opaqueQuadDSV;
	};
}