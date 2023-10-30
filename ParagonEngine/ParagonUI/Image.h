#pragma once
#include "IWidget.h"

namespace Pg::UI::Widget
{
	class Image : public Pg::UI::IWidget
	{
	public:
		Image(void* image, float width, float height);
		virtual void Update() abstract;

	private:
		void* _image;
		float _width;
		float _height;
	};
}

