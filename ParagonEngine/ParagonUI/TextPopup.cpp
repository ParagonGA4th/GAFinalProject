#include "TextPopup.h"
#include "imgui.h"

Pg::UI::Widget::TextPopup::TextPopup(std::string popupName, bool& popupShow, std::string text)
	:_popupName(popupName), _isPopupShow(popupShow), _text(text)
{

}

void Pg::UI::Widget::TextPopup::Update()
{
	if (_isPopupShow)
	{
		ImGui::OpenPopup(_popupName.c_str());
	}

	if (ImGui::BeginPopup(_popupName.c_str()))
	{
		ImGui::Text(_text.c_str());
		ImGui::EndPopup();
	}
}
