#include "DLayout.h"

Pg::UI::Layout::DLayout::DLayout(std::string layoutName)
	:_layoutName(layoutName)
{

}

Pg::UI::Layout::DLayout::~DLayout()
{

}

void Pg::UI::Layout::DLayout::CreateWidget()
{

}

void Pg::UI::Layout::DLayout::Update()
{

}

std::string Pg::UI::Layout::DLayout::GetPanelName()
{
	return _layoutName;
}

 