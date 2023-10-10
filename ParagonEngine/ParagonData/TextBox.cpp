#include "TextBox.h"

namespace Pg::Data
{

	TextBox::TextBox(GameObject* obj)
		: UIRenderer(obj),
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

}
