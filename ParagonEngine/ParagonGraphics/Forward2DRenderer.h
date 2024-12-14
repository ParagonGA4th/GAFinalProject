#pragma once
#include "BaseSpecificRenderer.h"

#include <dxtk/SpriteBatch.h>
#include <dxtk/CommonStates.h>
#include <memory>
#include <string>
/// <summary>
/// �̹��� / �ؽ�Ʈ ��, 2D ���ҽ� ����� �ô� �������̴�.
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

		//Sprite Rendering�� ����.
		std::unique_ptr<DirectX::SpriteBatch> _spriteBatch;
		std::unique_ptr<DirectX::CommonStates> _dxtkCommonStates; 
		Pg::Graphics::LowDX11Storage* _DXStorage;
	};
}


