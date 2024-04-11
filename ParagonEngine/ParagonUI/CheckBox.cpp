#include "CheckBox.h"
#include "imgui.h"

Pg::UI::Widget::CheckBox::CheckBox(std::string label, bool* check)
	:_isCheck(check), _label("##")
{
	_label.append(label);
}

void Pg::UI::Widget::CheckBox::Update()
{
	ImGui::Checkbox(_label.c_str(), _isCheck);
}

std::string Pg::UI::Widget::CheckBox::GetWidgetLabel()
{
	return _label;
}
