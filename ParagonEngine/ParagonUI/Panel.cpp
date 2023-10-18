#include "Panel.h"

Pg::UI::Panel::Panel()
	:_isShow(true)
{

}

Pg::UI::Panel::~Panel()
{

}

void Pg::UI::Panel::SetPanelShow(bool show)
{
	_isShow = show;
}

bool Pg::UI::Panel::GetPanelShow()
{
	return _isShow;
}


