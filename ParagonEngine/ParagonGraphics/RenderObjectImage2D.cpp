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
		//ImageRenderer ���� �����͸� �ޱ�.
		Pg::Data::ImageRenderer* tImageRenderer = static_cast<Pg::Data::ImageRenderer*>(GetBaseRenderer());


		assert(!tImageRenderer->GetImagePath().empty() && "�ε��� �� �̹��� Path ��� ������ �ȵ�. ");

		//HOTFIX. ���� Image Path Data�� �켱 '^'�� ��� �ִ������� Ȯ��.
		if (tImageRenderer->GetImagePath().find('^') == std::string::npos)
		{
			//ĳ�� �� ã�� - 1���� ����ִ� ��.
			// 
			//Image �����͸� �ޱ�.
			auto tTexture2dData = GraphicsResourceManager::Instance()->GetResource(tImageRenderer->GetImagePath(), eAssetDefine::_TEXTURE2D);
			_texture2DVector.push_back(static_cast<RenderTexture2D*>(tTexture2dData.get()));
		}
		else
		{
			std::string token;
			std::stringstream ss(tImageRenderer->GetImagePath());
			std::vector<std::string> outStringVector;
			//Ex. "^../asd.png^../ase.png^../asf.png" �̷� ������ path�� �����ؾ� �Ѵ�.
			//���� �� ũ�Ⱑ ���ƾ� ����.
			//ĳ���� �������� �̹��� �޾Ƶ��̴� �� ������.
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
				//Image �����͸� �ޱ�.
				auto tTexture2dData = GraphicsResourceManager::Instance()->GetResource(it, eAssetDefine::_TEXTURE2D);
				_texture2DVector.push_back(static_cast<RenderTexture2D*>(tTexture2dData.get()));
			}
		}

		//Image Function Binding.
		RenderObjectImage2D* tSelf = this;
		tImageRenderer->_setImageIndexFunc = std::bind(&RenderObjectImage2D::SetImageIndex, tSelf, std::placeholders::_1);
		tImageRenderer->_getImageIndexFunc = std::bind(&RenderObjectImage2D::GetImageIndex, tSelf);

		//Image Width / Height ������ ����.
		///RenderTexture2D�� Width / Height�� �����ϰ�, Override�Ѵ�!
		tImageRenderer->_width = _texture2DVector.at(0)->GetFileWidth();
		tImageRenderer->_height = _texture2DVector.at(0)->GetFileHeight();

		_imageWidth = &(tImageRenderer->_width);
		_imageHeight = &(tImageRenderer->_height);

		_fillRatio = &(tImageRenderer->_fillRatio);
		_sortingLayer = &(tImageRenderer->_sortingLayer);

		_fillRatioDirection = &(tImageRenderer->_fillRatioDirection);
	}

	void RenderObjectImage2D::Render(DirectX::SpriteBatch* spriteBatch, Pg::Data::CameraData* camData)
	{
		//�⺻���� Position ������ ����!
		//DirectX::XMFLOAT2 tPositionXM = { _baseRenderer->_object->_transform._position.x, _baseRenderer->_object->_transform._position.y };
		//
		//spriteBatch->Draw(_texture2D->GetSRV(), tPositionXM);

		DirectX::XMFLOAT2 ttTrans = { _baseRenderer->_object->_transform._position.x, _baseRenderer->_object->_transform._position.y };
		float ttRotation = 0.f;
		DirectX::XMFLOAT2 ttScale = { _baseRenderer->_object->_transform._scale.x, _baseRenderer->_object->_transform._scale.y };
		float ttScaleAverage = (ttScale.x + ttScale.y) / 2.0f;

		//DirectX::XMFLOAT2 ttOrigin = DirectX::XMFLOAT2(0.0f,0.0f);

		//������ �׷��ִ� ����.
		DirectX::XMFLOAT4 ttFF = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		//DirectX::XMFLOAT4 ttFF = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, _alpha);
		DirectX::XMVECTOR ttTintColor = DirectX::XMLoadFloat4(&ttFF);

		//���⿡ �ִ� ������ Fill ���� �뺯
		RECT tImageRect{};
		DirectX::XMFLOAT2 ttOrigin{}; 
		ReturnAppropriateFillRectOriginTrans(*_fillRatioDirection, tImageRect, ttOrigin, ttTrans);

		//SortingLayer ����.
		float tTrueSortingLayer = static_cast<float>(std::clamp(*_sortingLayer, (UINT)0, (UINT)100)) / 100.f;

		//�̹��� �򸮴� ���� ��ħ!
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
			PG_WARN("�߸��� �� ����. Clamp�Ǿ� ���Ե� ��.");
		}

		_imageIndex = std::clamp<unsigned int>(val, 0u, _texture2DVector.size() - 1);
	}

	

	void RenderObjectImage2D::ReturnAppropriateFillRectOriginTrans(Pg::Data::eFillRatioDirection dir, RECT& outRect, DirectX::XMFLOAT2& outOrigin, DirectX::XMFLOAT2& outTrans)
	{
		//SourceRect�� ������Ʈ �״�� �����Ѵ�����, ����� �� ���� ������?
		float tCorrectedRatio = std::clamp(*_fillRatio, std::numeric_limits<float>::epsilon(), 100.0f); //���� DivideByZero������ ����.

		if (dir == Pg::Data::eFillRatioDirection::LEFT_TO_RIGHT)
		{
			//���⿡ �ִ� ������ Fill ���� �뺯
			outRect = { 0,0, static_cast<LONG>((*_imageWidth) * (tCorrectedRatio / 100.0f)), static_cast<LONG>(*_imageHeight) };
			outOrigin = DirectX::XMFLOAT2(*_imageWidth / 2.0f, *_imageHeight / 2.0f);
		}
		else if (dir == Pg::Data::eFillRatioDirection::DOWN_TO_UP)
		{
			//float tRealPercentage = tCorrectedRatio / 100.f;
			//LONG tImageHeight = static_cast<LONG>(*_imageHeight);
			//int tVisibleHeight = static_cast<int>(tImageHeight * tRealPercentage);
			//
			//outRect = {
			//	0, tImageHeight - tVisibleHeight,
			//	static_cast<LONG>(*_imageWidth), tImageHeight };
			//outOrigin = DirectX::XMFLOAT2(*_imageWidth / 2.0f, *_imageHeight / 2.0f); 
			//outTrans.y -= tVisibleHeight;
			////����μ��� ������ DOWN_TO_UP �������� �ȵǾ� �ִ� ����.
			////�ļ����� �ؾ�.
			assert(false && "���� ������ ����");
		}
		else if (dir == Pg::Data::eFillRatioDirection::UP_TO_DOWN)
		{
			outRect = { 0,0, static_cast<LONG>(*_imageWidth),
				static_cast<LONG>((*_imageHeight) * (tCorrectedRatio / 100.0f)) };
			outOrigin = DirectX::XMFLOAT2(*_imageWidth / 2.0f, *_imageHeight / 2.0f);
		}
		else
		{
			//����� ������.
			assert(false);
		}
	}

}
