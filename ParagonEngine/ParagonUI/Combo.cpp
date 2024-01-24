#include "Combo.h"

int Pg::UI::Widget::Combo::_itemType = 0;

Pg::UI::Widget::Combo::Combo(std::string label, std::vector<std::string> itemList)
	:_label(label), _itemList(itemList), _isStart(true)
{

}

void Pg::UI::Widget::Combo::Update()
{
	//if (_isStart)
	//{
	//const char* _itemNames[]

	//}
	//ImGui::Combo(_label.c_str(), &_itemType, )
}
