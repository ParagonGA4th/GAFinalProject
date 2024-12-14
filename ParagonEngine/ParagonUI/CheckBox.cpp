#include "CheckBox.h"
#include "imgui.h"

Pg::UI::Widget::CheckBox::CheckBox(std::string label, bool* check)
	:_isCheck(check), _label("##")
{
	_label.append(label);
}

Pg::UI::Widget::CheckBox::CheckBox(std::string label, bool check)
	:_isCheck(nullptr), _label("##")
{
	_isCheck = new bool(check);
	_label.append(label);
}

void Pg::UI::Widget::CheckBox::Update()
{
	if(ImGui::Checkbox(_label.c_str(), _isCheck))
	{
		int a = 3;
	}
}

std::string Pg::UI::Widget::CheckBox::GetWidgetLabel()
{
	return _label;
}

bool Pg::UI::Widget::CheckBox::GetCheck()
{
	return *_isCheck;
}
