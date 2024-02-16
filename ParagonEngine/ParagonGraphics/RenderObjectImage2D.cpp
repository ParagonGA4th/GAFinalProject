#include "RenderObjectImage2D.h"
#include "LowDX11Storage.h"
#include "RenderTexture2D.h"
#include "GraphicsResourceManager.h"
#include "MathHelper.h"
#include "../ParagonData/ImageRenderer.h"

#include <algorithm>
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


		assert(!tImageRenderer->GetImagePath().empty() && "로드할 때 이미지 Path 비어 있으면 안됨. ");
		//Image 데이터를 받기.
		auto tTexture2dData = GraphicsResourceManager::Instance()->GetResource(tImageRenderer->GetImagePath(), eAssetDefine::_TEXTURE2D);
		_texture2D = static_cast<RenderTexture2D*>(tTexture2dData.get());

		////Image Width / Height 포인터 보관.
		//_imageWidth = &(tImageRenderer->_width);
		//_imageHeight = &(tImageRenderer->_height);
		//
		//_fillRatio = &(tImageRenderer->_fillRatio);
		//_sortingLayer = &(tImageRenderer->_f)
	}

	void RenderObjectImage2D::Render(DirectX::SpriteBatch* spriteBatch, Pg::Data::CameraData* camData)
	{
		//기본적인 Position 연동만 설정!
		DirectX::XMFLOAT2 tPositionXM = { _baseRenderer->_object->_transform._position.x, _baseRenderer->_object->_transform._position.y };
		
		spriteBatch->Draw(_texture2D->GetSRV(), tPositionXM);


		//DirectX::XMFLOAT2 ttTrans = { _baseRenderer->_object->_transform._position.x, _baseRenderer->_object->_transform._position.y };
		//float ttRotation = 0.f;
		//DirectX::XMFLOAT2 ttScale = { _baseRenderer->_object->_transform._scale.x, _baseRenderer->_object->_transform._scale.y };
		//float ttScaleAverage = (ttScale.x + ttScale.y) / 2.0f;
		//
		//DirectX::XMFLOAT2 ttOrigin = DirectX::XMFLOAT2(*_imageWidth / 2.0f, *_imageHeight / 2.0f);
		////DirectX::XMFLOAT2 ttOrigin = DirectX::XMFLOAT2(0.0f,0.0f);
		//
		////실제로 그려주는 과정.
		//DirectX::XMFLOAT4 ttFF = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, _alpha);
		////DirectX::XMFLOAT4 ttFF = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, _alpha);
		//DirectX::XMVECTOR ttTintColor = DirectX::XMLoadFloat4(&ttFF);
		//
		////SourceRect를 오브젝트 그대로 조정한다음에, 출력할 수 있지 않을까?
		//float tCorrectedRatio = std::clamp(_fillRatio, std::numeric_limits<float>::epsilon(), 100.0f); //이제 DivideByZero생기지 않음.
		//
		////여기에 있는 비율은 Fill 값을 대변
		//RECT tImageRect = { 0,0, static_cast<LONG>((*_imageWidth) * (tCorrectedRatio / 100.0f)), static_cast<LONG>(*_imageHeight) };
		//
		////이미지 쏠리는 문제 고침!
		//spriteBatch->Draw(_texture2D->GetSRV(), ttTrans, &tImageRect, ttTintColor,
		//	ttRotation, ttOrigin, ttScaleAverage, DirectX::SpriteEffects::SpriteEffects_None, _sortingLayer);
		////_spriteBatch->Draw(_textureData->GetSRV(), ttTrans, &tImageRect, NULL,
		////	ttRotation, ttOrigin, ttScaleAverage, DirectX::SpriteEffects::SpriteEffects_None, _sortingLayer);



	}
}
