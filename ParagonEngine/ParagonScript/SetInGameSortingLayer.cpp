#include "SetInGameSortingLayer.h"
#include "../ParagonData/ImageRenderer.h"

namespace Pg::DataScript
{
	SetInGameSortingLayer::SetInGameSortingLayer(Pg::Data::GameObject* obj) :
		ScriptInterface(obj)
	{

	}

	void SetInGameSortingLayer::Start()
	{
		_imageRenderer = _object->GetComponent<Pg::Data::ImageRenderer>();

		_imageRenderer->_sortingLayer = 5;
	}
}