#include "RenderObjectText2D.h"
#include "LowDX11Storage.h"
#include "RenderFont.h"
#include "MathHelper.h"
#include "GraphicsResourceManager.h"
#include "../ParagonData/TextRenderer.h"

#include <DirectXMath.h>

namespace Pg::Graphics
{
	using Pg::Graphics::Helper::MathHelper;
	using Pg::Graphics::Manager::GraphicsResourceManager;
	using Pg::Data::Enums::eAssetDefine;

	RenderObjectText2D::RenderObjectText2D(Pg::Data::BaseRenderer* baseRenderer) : RenderObject2D(baseRenderer)
	{
		//ImageRenderer 따로 포인터를 받기.
		Pg::Data::TextRenderer* tTextRenderer = static_cast<Pg::Data::TextRenderer*>(GetBaseRenderer());

		//Image 데이터를 받기.
		auto tFontData = GraphicsResourceManager::Instance()->GetResource(tTextRenderer->GetFontName(), eAssetDefine::_FONT);
		_renderFont = static_cast<RenderFont*>(tFontData.get());
	}

	void RenderObjectText2D::Render(DirectX::SpriteBatch* spriteBatch, Pg::Data::CameraData* camData)
	{
		//TextRenderer 따로 포인터를 받기.
		Pg::Data::TextRenderer* tTextRenderer = static_cast<Pg::Data::TextRenderer*>(GetBaseRenderer());

		//기본적인 Position 연동만 설정!
		DirectX::XMFLOAT2 tPositionXM = { _baseRenderer->_object->_transform.GetPosition().x, _baseRenderer->_object->_transform.GetPosition().y };

		_renderFont->_font->DrawString(spriteBatch, tTextRenderer->GetString().c_str(), tPositionXM, DirectX::Colors::White);
	}

}