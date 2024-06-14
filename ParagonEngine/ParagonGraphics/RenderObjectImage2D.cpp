#include "RenderObjectImage2D.h"
#include "LowDX11Storage.h"
#include "RenderTexture2D.h"
#include "GraphicsResourceManager.h"
#include "MathHelper.h"
#include "../ParagonData/ImageRenderer.h"

#include <algorithm>
#include <DirectXMath.h>
#include <functional>

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

		//HOTFIX. 이제 Image Path Data는 우선 '^'이 들어 있는지부터 확인.
		if (tImageRenderer->GetImagePath().find('^') == std::string::npos)
		{
			//캐럿 못 찾음 - 1개만 들어있는 것.
			// 
			//Image 데이터를 받기.
			auto tTexture2dData = GraphicsResourceManager::Instance()->GetResource(tImageRenderer->GetImagePath(), eAssetDefine::_TEXTURE2D);
			_texture2DVector.push_back(static_cast<RenderTexture2D*>(tTexture2dData.get()));
		}
		else
		{
			std::string token;
			std::stringstream ss(tImageRenderer->GetImagePath());
			std::vector<std::string> outStringVector;
			//Ex. "^../asd.png^../ase.png^../asf.png" 이런 식으로 path가 존재해야 한다.
			//전부 다 크기가 같아야 동작.
			//캐럿을 기준으로 이미지 받아들이는 거 나누기.
			while (std::getline(ss, token, '^'))
			{
				outStringVector.push_back(token);
			}

			for (auto& it : outStringVector)
			{
				if (it.empty())
				{
					continue;
				}
				//Image 데이터를 받기.
				auto tTexture2dData = GraphicsResourceManager::Instance()->GetResource(it, eAssetDefine::_TEXTURE2D);
				_texture2DVector.push_back(static_cast<RenderTexture2D*>(tTexture2dData.get()));
			}
		}

		//Image Function Binding.
		RenderObjectImage2D* tSelf = this;
		tImageRenderer->_setImageIndexFunc = std::bind(&RenderObjectImage2D::SetImageIndex, tSelf, std::placeholders::_1);
		tImageRenderer->_getImageIndexFunc = std::bind(&RenderObjectImage2D::GetImageIndex, tSelf);

		//Image Width / Height 포인터 보관.
		///RenderTexture2D의 Width / Height을 보관하고, Override한다!
		tImageRenderer->_width = _texture2DVector.at(0)->GetFileWidth();
		tImageRenderer->_height = _texture2DVector.at(0)->GetFileHeight();

		_imageWidth = &(tImageRenderer->_width);
		_imageHeight = &(tImageRenderer->_height);

		_fillRatio = &(tImageRenderer->_fillRatio);
		_sortingLayer = &(tImageRenderer->_sortingLayer);
	}

	void RenderObjectImage2D::Render(DirectX::SpriteBatch* spriteBatch, Pg::Data::CameraData* camData)
	{
		//기본적인 Position 연동만 설정!
		//DirectX::XMFLOAT2 tPositionXM = { _baseRenderer->_object->_transform._position.x, _baseRenderer->_object->_transform._position.y };
		//
		//spriteBatch->Draw(_texture2D->GetSRV(), tPositionXM);

		DirectX::XMFLOAT2 ttTrans = { _baseRenderer->_object->_transform._position.x, _baseRenderer->_object->_transform._position.y };
		float ttRotation = 0.f;
		DirectX::XMFLOAT2 ttScale = { _baseRenderer->_object->_transform._scale.x, _baseRenderer->_object->_transform._scale.y };
		float ttScaleAverage = (ttScale.x + ttScale.y) / 2.0f;

		DirectX::XMFLOAT2 ttOrigin = DirectX::XMFLOAT2(*_imageWidth / 2.0f, *_imageHeight / 2.0f);
		//DirectX::XMFLOAT2 ttOrigin = DirectX::XMFLOAT2(0.0f,0.0f);

		//실제로 그려주는 과정.
		DirectX::XMFLOAT4 ttFF = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		//DirectX::XMFLOAT4 ttFF = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, _alpha);
		DirectX::XMVECTOR ttTintColor = DirectX::XMLoadFloat4(&ttFF);

		//SourceRect를 오브젝트 그대로 조정한다음에, 출력할 수 있지 않을까?
		float tCorrectedRatio = std::clamp(*_fillRatio, std::numeric_limits<float>::epsilon(), 100.0f); //이제 DivideByZero생기지 않음.

		//여기에 있는 비율은 Fill 값을 대변
		RECT tImageRect = { 0,0, static_cast<LONG>((*_imageWidth) * (tCorrectedRatio / 100.0f)), static_cast<LONG>(*_imageHeight) };

		//SortingLayer 정리.
		float tTrueSortingLayer = static_cast<float>(std::clamp(*_sortingLayer, (UINT)0, (UINT)100)) / 100.f;

		//이미지 쏠리는 문제 고침!
		spriteBatch->Draw(_texture2DVector.at(_imageIndex)->GetSRV(), ttTrans, &tImageRect, ttTintColor,
			ttRotation, ttOrigin, ttScaleAverage, DirectX::SpriteEffects::SpriteEffects_None, tTrueSortingLayer);
		//_spriteBatch->Draw(_textureData->GetSRV(), ttTrans, &tImageRect, NULL,
		//	ttRotation, ttOrigin, ttScaleAverage, DirectX::SpriteEffects::SpriteEffects_None, _sortingLayer);
	}

	unsigned int RenderObjectImage2D::GetImageIndex()
	{
		return _imageIndex;
	}

	void RenderObjectImage2D::SetImageIndex(unsigned int val)
	{
		if (_texture2DVector.size() - 1 < val)
		{
			PG_WARN("잘못된 값 투입. Clamp되어 투입될 것.");
		}

		_imageIndex = std::clamp<unsigned int>(val, 0u, _texture2DVector.size() - 1);
	}
}
