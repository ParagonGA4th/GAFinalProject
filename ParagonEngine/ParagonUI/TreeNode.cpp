#include "TreeNode.h"
#include "imgui.h"

Pg::UI::Widget::TreeNode::TreeNode(std::string nodeName, std::vector<IWidget*> widgets)
	:_nodeName(nodeName), _widgets(widgets)
{
}

void Pg::UI::Widget::TreeNode::Update()
{
	if (ImGui::TreeNode(_nodeName.c_str()))
	{
		for (auto i : _widgets)
		{
			i->Update();
		}

		ImGui::TreePop();
	}
}

std::string Pg::UI::Widget::TreeNode::GetWidgetLabel()
{
	return _nodeName;
}
