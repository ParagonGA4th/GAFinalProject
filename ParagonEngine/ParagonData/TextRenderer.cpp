#include "TextRenderer.h"

namespace Pg::Data
{

	TextRenderer::TextRenderer(GameObject* obj)
		: RendererBase2D(obj),
		_string(""),
		_fontSize(1.f)
	{
		SetRendererTypeName(typeid(this).name());
	}

	void TextRenderer::SetString(std::string str)
	{
		_string = str;
	}

	std::string TextRenderer::GetString()
	{
		return _string;
	}

	void TextRenderer::SetSize(float size)
	{
		_fontSize = size;
	}

	float TextRenderer::GetSize()
	{
		return _fontSize;
	}

	void TextRenderer::SetFont(std::string fontName)
	{
		_font = fontName;
	}

	std::string TextRenderer::GetFontName()
	{
		return _font;
	}

	void TextRenderer::SetFontColor(PGFLOAT4 color)
	{
		_fontColor = color;
	}

	Pg::Math::PGFLOAT4 TextRenderer::GetFontColor()
	{
		return _fontColor;
	}

}
