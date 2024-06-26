#pragma once
#include "ScriptInterface.h"


namespace Pg::Data
{
	class ImageRenderer;
}

namespace Pg::DataScript
{
	class SetInGameSortingLayer : public ScriptInterface<SetInGameSortingLayer>
	{
		DEFINE_PARAGON_SCRIPT(SetInGameSortingLayer);

	public:
		SetInGameSortingLayer(Pg::Data::GameObject* obj);

	public:
		virtual void Start() override;

	private:
		Pg::Data::ImageRenderer* _imageRenderer{ nullptr };
	};
}


