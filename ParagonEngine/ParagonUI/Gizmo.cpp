#include "Gizmo.h"

#include "imgui.h"
#include "ImGuizmo.h"

#include "../ParagonData/Camera.h"
#include "../ParagonData/Transform.h"

Pg::UI::Helper::Gizmo::Gizmo()
	: _camera(nullptr), _trans(nullptr)
{
}

void Pg::UI::Helper::Gizmo::CreateFrame()
{
	ImGuizmo::SetOrthographic(false);
	ImGuizmo::BeginFrame();
}

void Pg::UI::Helper::Gizmo::SetCamera(Pg::Data::Camera* camera)
{
	_camera = camera;
}

void Pg::UI::Helper::Gizmo::SetTransform(Pg::Data::Transform* trans)
{
	_trans = trans;
}

void Pg::UI::Helper::Gizmo::SetWindowSize(float width, float height)
{
	_displayWidth = width;
	_displayHeight = height;
}

void Pg::UI::Helper::Gizmo::DrawGizmo()
{

}


