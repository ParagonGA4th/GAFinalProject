#include "TextRenderer.h"

#include <atlconv.h>
#include <windows.h>
#include <stringapiset.h>

namespace Pg::Data
{

	TextRenderer::TextRenderer(GameObject* obj)
		: RendererBase2D(obj),
		_string(""),
		_fontSize(1.f)
	{
		SetRendererTypeName(typeid(this).name());
	}

	void TextRenderer::SetString(const std::string& str)
	{
		_string = str;

		////String을 셋할 때는 무조건 SetString을 써야 한다.
		//for (int i = 0; i < MAXIMUM_TEXTBUFFER_COUNT; i++)
		//{
		//	_wBuffer[i] = '\0';
		//}
		//
		//int _tStringSize = min(str.size(), MAXIMUM_TEXTBUFFER_COUNT);
		//MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length() + 1, _wBuffer, _tStringSize);
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

	//const wchar_t* TextRenderer::GetWCharPointer() const
	//{
	//	return _wBuffer;
	//}
	

}
