#pragma once
#include "ScriptInterface.h"

namespace Pg::Data
{
	class ImageRenderer;
}

namespace Pg::DataScript
{
	class TitleScene : public ScriptInterface<TitleScene>
	{
		DEFINE_PARAGON_SCRIPT(TitleScene);

	public:
		TitleScene(Pg::Data::GameObject* obj);

	public:
		virtual void Awake() override;

	private:
		Pg::Data::ImageRenderer* _imageRenderer;
	};
}

