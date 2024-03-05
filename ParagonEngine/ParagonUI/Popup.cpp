#include "Popup.h"
#include "imgui.h"

Pg::UI::Widget::Popup::Popup(std::string popupName, std::vector<IWidget*> widgets)
	:_popupName(popupName), _widgets(widgets), _isPopupOpen(false)
{

}

void Pg::UI::Widget::Popup::Update()
{
	if (_isPopupOpen)
	{
		ImGui::OpenPopup(_popupName.c_str());
	}

	if(ImGui::BeginPopup(_popupName.c_str()))
	{
		ImGui::Separator();

		for (auto i : _widgets)
		{
			i->Update();
		}

		ImGui::EndPopup();
	}
}

bool* Pg::UI::Widget::Popup::isPopupOpen()
{
	return &_isPopupOpen;
}
