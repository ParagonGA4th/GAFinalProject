#pragma once
#include "DX11Headers.h"
#include "GBufferRender.h"
#include "GBufferDepthStencil.h"
#include "GraphicsCarrier.h"
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
}

namespace Pg::Graphics
{
	class DeferredRenderer
	{
	public:
		DeferredRenderer();
		~DeferredRenderer();

	public:
		void Initialize();
		void SetupRenderPasses();
		void Render(RenderObject3DList* renderObjectList, Pg::Data::CameraData* camData);

	private:
		void PushRenderPasses();
		void InitializeRenderPasses();
		void PlaceRequiredResources();

	private:
		void RenderFirstStaticPass(RenderObject3DList* renderObjectList, Pg::Data::CameraData* camData);
		void RenderObjMatStaticPass(RenderObject3DList* renderObjectList, Pg::Data::CameraData* camData);
		void RenderOpaqueQuadPasses(RenderObject3DList* renderObjectList, Pg::Data::CameraData* camData);
		void RenderFinalRenderPass(RenderObject3DList* renderObjectList, Pg::Data::CameraData* camData);
	
	private:
		//저장용 X, 객체를 임시로 보관할 수 있게 매개변수용 자료 세팅.
		std::unique_ptr<GraphicsCarrier> _gCarrier;

	private:
		LowDX11Storage* _DXStorage;
		std::vector<IRenderPass*> _renderPassVector;

		//메인 렌더 타겟으로 넘어갈 G-Buffer Render & Depth Stencil.
		std::unique_ptr<GBufferRender> _opaqueQuadRTV;
		std::unique_ptr<GBufferDepthStencil> _opaqueQuadDSV;

		//Depth Writing에 쓰일 SRV, Final Render Pass에서 활용됨.
		ID3D11ShaderResourceView* _objMatDepthSRV = nullptr;
		

	};
}