#include "TitleScene.h"
#include "../ParagonData/ImageRenderer.h"

namespace Pg::DataScript
{
	TitleScene::TitleScene(Pg::Data::GameObject* obj) :
		ScriptInterface(obj)
	{

	}

	void TitleScene::Awake()
	{
		_imageRenderer = _object->GetComponent<Pg::Data::ImageRenderer>();

		_imageRenderer->_sortingLayer = 5;
	}
}


