#include "RenderObjectText2D.h"
#include "LowDX11Storage.h"
#include "RenderFont.h"
#include "MathHelper.h"
#include "GraphicsResourceManager.h"
#include "../ParagonData/TextRenderer.h"

#include <algorithm>
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

		_textWidth = &(tTextRenderer->_width);
		_textHeight = &(tTextRenderer->_height);
		_textColor = &(tTextRenderer->_fontColor);
		_sortingLayer = &(tTextRenderer->_sortingLayer);
	}

	void RenderObjectText2D::Render(DirectX::SpriteBatch* spriteBatch, Pg::Data::CameraData* camData)
	{
		//TextRenderer 따로 포인터를 받기.
		Pg::Data::TextRenderer* tTextRenderer = static_cast<Pg::Data::TextRenderer*>(GetBaseRenderer());
		//
		////기본적인 Position 연동만 설정!
		//DirectX::XMFLOAT2 tPositionXM = { _baseRenderer->_object->_transform._position.x, _baseRenderer->_object->_transform._position.y };
		//
		//_renderFont->_font->DrawString(spriteBatch, tTextRenderer->GetString().c_str(), tPositionXM, DirectX::Colors::White);

		DirectX::XMFLOAT2 ttTrans = { _baseRenderer->_object->_transform._position.x, _baseRenderer->_object->_transform._position.y };
		DirectX::XMFLOAT2 ttScale = { _baseRenderer->_object->_transform._scale.x, _baseRenderer->_object->_transform._scale.y };
		float ttScaleAverage = (ttScale.x + ttScale.y) / 2.0f;

		DirectX::XMFLOAT2 ttOrigin = DirectX::XMFLOAT2((*_textWidth) / 2.0f, (*_textHeight) / 2.0f);
		DirectX::XMVECTOR ttColor = PG2XM_FLOAT4_VECTOR(*_textColor);

		//SortingLayer 정리.
		float tTrueSortingLayer = static_cast<float>(std::clamp(*_sortingLayer, (UINT)0, (UINT)100)) / 100.f;

		//tTextRenderer->GetString().c_str()
		_renderFont->_font->DrawString(spriteBatch, tTextRenderer->_string.c_str(), ttTrans, ttColor, 0.f, ttOrigin,
			ttScaleAverage, DirectX::SpriteEffects_None, tTrueSortingLayer);
	}
}