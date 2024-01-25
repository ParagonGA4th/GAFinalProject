#pragma once
#include "IWidget.h"

namespace Pg::UI::Widget
{
	class ImageButton : public Pg::UI::IWidget
	{
	public:
		ImageButton(void* texture, float width, float height);
		virtual void Update() override;

		static bool _isButtonClick;

	protected:
		void* _texture;
		float _width;
		float _height;
	};
}

