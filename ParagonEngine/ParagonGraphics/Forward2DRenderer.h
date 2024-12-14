#pragma once
#include "BaseSpecificRenderer.h"

#include <dxtk/SpriteBatch.h>
#include <dxtk/CommonStates.h>
#include <memory>
#include <string>
/// <summary>
/// 이미지 / 텍스트 등, 2D 리소스 출력을 맡는 렌더러이다.
/// </summary>

namespace Pg::Data
{
	struct CameraData;
}

namespace Pg::Graphics
{
	struct RenderObject2DList;
	
	class LowDX11Storage;
}

namespace Pg::Graphics
{
	class Forward2DRenderer : public BaseSpecificRenderer
	{
	public:
		Forward2DRenderer(D3DCarrier* d3dCarrier);

		virtual void Initialize() override;
		virtual void SetupRenderPasses() override;

		virtual void RenderContents(void* renderObjectList, void* optionalRequirement, Pg::Data::CameraData* camData) override;
		virtual void ConfirmCarrierData() override;
	

	private:
		void Render(RenderObject2DList* render2DList, Pg::Data::CameraData* camData);

		//Sprite Rendering을 위해.
		std::unique_ptr<DirectX::SpriteBatch> _spriteBatch;
		std::unique_ptr<DirectX::CommonStates> _dxtkCommonStates; 
		Pg::Graphics::LowDX11Storage* _DXStorage;
	};
}


