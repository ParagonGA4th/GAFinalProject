#include "Handle.h"
#include "GameObject.h"
#include "ImageRenderer.h"
#include "../ParagonUtil/Log.h"
namespace Pg::Data
{
	Handle::Handle(GameObject* owner) :
		Component(owner)
	{
		FACTORY_INIT;
	
		if (owner->GetComponent<ImageRenderer>())
		{
			assert(false);
		}



	}

	void Handle::Internal_EngineAwake()
	{
		if (!_object->GetComponent<ImageRenderer>())
		{
			assert(false && "Handle의 ImageRenderer Component가 없습니다");
		}

		_imageRenderer = _object->GetComponent<ImageRenderer>();
		_imageRenderer->_sortingLayer = 0;
		_imageWidth = &(_imageRenderer->_width);
		_imageHeight = &(_imageRenderer->_height);
	}

	void Handle::SetImagePath(const std::string& path)
	{
		_imageRenderer->SetImagePath(path);
	}

	void Handle::SetImageSize(float width, float height)
	{
		SetImageWidth(width);
		SetImageHeight(height);
	}

	void Handle::SetImageWidth(float width)
	{
		_imageRenderer->SetWidth(width);
	}

	float Handle::GetImageWidth()
	{
		return *_imageWidth;
	}

	void Handle::SetImageHeight(float height)
	{
		_imageRenderer->SetHeight(height);
	}

	float Handle::GetImageHeight()
	{
		return *_imageHeight;
	}
}