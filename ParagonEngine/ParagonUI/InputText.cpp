#include "InputText.h"
#include "imgui.h"

Pg::UI::Widget::InputText::InputText(std::string label, std::string* text)
	: _label("##"), _text(text), _inputText()
{
	_label.append(label);
	strcpy(_inputText, (*text).c_str());
}

void Pg::UI::Widget::InputText::Update()
{
	strcpy(_inputText, (*_text).c_str());

	if (ImGui::InputText(_label.c_str(), _inputText, IM_ARRAYSIZE(_inputText)))
	{
		if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter)))
		{
			*_text = _inputText;
		}
	}
}

std::string Pg::UI::Widget::InputText::GetWidgetLabel()
{
	return _label;
}
