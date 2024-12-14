#pragma once
#include "IWidget.h"

namespace Pg::UI::Widget
{
	class Image : public Pg::UI::IWidget
	{
	public:
		Image(void* image, float width, float height);
		virtual void Update() override;
		virtual std::string GetWidgetLabel() override;

		float* GetWidth();
		float* GetHeight();
	private:
		void* _image;

		float _orgWidth;
		float _orgHeight;

		float _width;
		float _height;
	};
}
