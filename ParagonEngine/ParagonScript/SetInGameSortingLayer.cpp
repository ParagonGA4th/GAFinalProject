#include "SetInGameSortingLayer.h"
#include "../ParagonData/ImageRenderer.h"

namespace Pg::DataScript
{
	SetInGameSortingLayer::SetInGameSortingLayer(Pg::Data::GameObject* obj) :
		ScriptInterface(obj)
	{

	}

	void SetInGameSortingLayer::OnDeserialize(SerializeVector& sv)
	{
		Pg::Data::SerializerHelper::OnDeserializerHelper(this, sv);
	}

	void SetInGameSortingLayer::OnSerialize(SerializeVector& sv)
	{
		Pg::Data::SerializerHelper::OnSerializerHelper(this, sv);
	}

	void SetInGameSortingLayer::Start()
	{
		_imageRenderer = _object->GetComponent<Pg::Data::ImageRenderer>();

		_imageRenderer->_sortingLayer = _sortingLayer;
	}
}