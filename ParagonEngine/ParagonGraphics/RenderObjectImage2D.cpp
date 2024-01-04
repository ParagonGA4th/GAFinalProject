#include "RenderObjectImage2D.h"
#include "LowDX11Storage.h"
#include "RenderTexture2D.h"
#include "GraphicsResourceManager.h"
#include "MathHelper.h"
#include "../ParagonData/ImageRenderer.h"

#include <DirectXMath.h>

namespace Pg::Graphics
{
	using Pg::Graphics::Helper::MathHelper;
	using Pg::Graphics::Manager::GraphicsResourceManager;
	using Pg::Data::Enums::eAssetDefine;

	RenderObjectImage2D::RenderObjectImage2D(Pg::Data::BaseRenderer* baseRenderer) : RenderObject2D(baseRenderer)
	{
		//ImageRenderer 따로 포인터를 받기.
		Pg::Data::ImageRenderer* tImageRenderer = static_cast<Pg::Data::ImageRenderer*>(GetBaseRenderer());

		//Image 데이터를 받기.
		auto tTexture2dData = GraphicsResourceManager::Instance()->GetResource(tImageRenderer->GetImagePath(), eAssetDefine::_2DTEXTURE);
		_texture2D = static_cast<RenderTexture2D*>(tTexture2dData.get());
	}

	void RenderObjectImage2D::Render(DirectX::SpriteBatch* spriteBatch, Pg::Data::CameraData* camData)
	{
		//기본적인 Position 연동만 설정!
		DirectX::XMFLOAT2 tPositionXM = { _baseRenderer->_object->_transform._position.x, _baseRenderer->_object->_transform._position.y };
		
		spriteBatch->Draw(_texture2D->GetSRV(), tPositionXM);
	}
}
