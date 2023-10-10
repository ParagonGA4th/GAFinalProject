#include "TextBox.h"

namespace Pg::Data
{

	TextBox::TextBox(GameObject* obj)
		: RendererBase2D(obj),
		_string(""),
		_fontSize(1.f)
	{

	}

	void TextBox::SetString(std::string str)
	{
		_string = str;
	}

	std::string TextBox::GetString()
	{
		return _string;
	}

	void TextBox::SetSize(float size)
	{
		_fontSize = size;
	}

	float TextBox::GetSize()
	{
		return _fontSize;
	}

	void TextBox::SetFont(std::string fontName)
	{
		_font = fontName;
	}

	std::string TextBox::GetFontName()
	{
		return _font;
	}

	void TextBox::SetFontColor(PGFLOAT4 color)
	{
		_fontColor = color;
	}

	Pg::Math::PGFLOAT4 TextBox::GetFontColor()
	{
		return _fontColor;
	}

}
