#include "Popup.h"
#include "imgui.h"

Pg::UI::Widget::Popup::Popup(std::string popupName, std::vector<IWidget*> widgets)
	:_popupName(popupName), _widgets(widgets)
{

}

void Pg::UI::Widget::Popup::Update()
{
	if (ImGui::BeginPopup(_popupName.c_str()))
	{
		for (auto& item : _widgets) item->Update();
		ImGui::EndPopup();
	}
}

std::string Pg::UI::Widget::Popup::GetWidgetLabel()
{
	return _popupName;
}

