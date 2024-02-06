#include "Gizmo.h"

#include "imgui.h"
#include "ImGuizmo.h"

#include "../ParagonData/Camera.h"
#include "../ParagonData/Transform.h"

Pg::UI::Helper::Gizmo::Gizmo()
	: _imGizmoMatrix
	(
		1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, -1.f, 0.f,
		0.f, 0.f, 0.f, 1.f
	)
{
}

void Pg::UI::Helper::Gizmo::CreateFrame()
{
	ImGuizmo::SetOrthographic(false);  
	ImGuizmo::BeginFrame();
}

void Pg::UI::Helper::Gizmo::SetCamera(Pg::Data::Camera* camera)
{
	ParagonMatrixToImGizmoMatrix(camera->GetViewMatrix(), 0);
	ParagonMatrixToImGizmoMatrix(camera->GetProjMatrix(), 1);
}

void Pg::UI::Helper::Gizmo::SetTransform(Pg::Data::Transform* trans)
{
	ParagonMatrixToImGizmoMatrix(trans->GetWorldTM(), 2);
}

void Pg::UI::Helper::Gizmo::SetWindowSize(float width, float height)
{
	_displayWidth = width;
	_displayHeight = height;
}

void Pg::UI::Helper::Gizmo::DrawGizmo()
{
	ImGuizmo::SetRect(0, 0, _displayWidth, _displayHeight);

	ImGuizmo::Manipulate(
		&(_cameraView[0][0]),
		&(_cameraProj[0][0]),
		ImGuizmo::TRANSLATE,
		ImGuizmo::LOCAL,
		&(_transMatrix[0][0])
	);
}

void Pg::UI::Helper::Gizmo::ParagonMatrixToImGizmoMatrix(Pg::Math::PGFLOAT4X4 matrix, int matType)
{
	matrix *= _imGizmoMatrix;

	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
	
			if (matType == 0) _cameraView[i][j] = matrix.m[i][j];
			else if (matType == 1) _cameraProj[i][j] = matrix.m[i][j];
			else if (matType == 2) _transMatrix[i][j] = matrix.m[i][j];	
		}
	}
}

