#pragma once
#include "DX11Headers.h"

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
		void Render(RenderObject3DList* renderObjectList, Pg::Data::CameraData* camData);
	
	private:
		void PushRenderPasses();
		void InitializeRenderPasses();
		void PlaceRequiredResources();
		
	private:
		LowDX11Storage* _DXStorage;
		std::vector<IRenderPass*> _renderPassVector;

		//메인 렌더 타겟으로 넘어갈 G-Buffer Render & Depth Stencil.

	};
}