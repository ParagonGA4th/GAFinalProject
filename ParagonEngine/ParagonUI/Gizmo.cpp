#include "Gizmo.h"

#include "imgui.h"
#include "ImGuizmo.h"

#include "../ParagonData/Camera.h"
#include "../ParagonData/Transform.h"

Pg::UI::Helper::Gizmo::Gizmo()
{
}

void Pg::UI::Helper::Gizmo::CreateFrame()
{
	ImGuizmo::SetOrthographic(false);  
	ImGuizmo::BeginFrame();
}

void Pg::UI::Helper::Gizmo::SetCamera(Pg::Data::Camera* camera)
{
	//proj
	_cameraProj[0][0] = camera->GetProjMatrix()._11; _cameraProj[0][1] = camera->GetProjMatrix()._12; 
	_cameraProj[0][2] = camera->GetProjMatrix()._13; _cameraProj[0][3] = camera->GetProjMatrix()._14;
	_cameraProj[1][0] = camera->GetProjMatrix()._21; _cameraProj[1][1] = camera->GetProjMatrix()._22; 
	_cameraProj[1][2] = camera->GetProjMatrix()._23; _cameraProj[1][3] = camera->GetProjMatrix()._24;
	_cameraProj[2][0] = camera->GetProjMatrix()._31; _cameraProj[2][1] = camera->GetProjMatrix()._32; 
	_cameraProj[2][2] = -camera->GetProjMatrix()._33; _cameraProj[2][3] = -camera->GetProjMatrix()._34;
	_cameraProj[3][0] = camera->GetProjMatrix()._41; _cameraProj[3][1] = camera->GetProjMatrix()._42; 
	_cameraProj[3][2] = camera->GetProjMatrix()._43; _cameraProj[3][3] = camera->GetProjMatrix()._44;

	// view
	//DirectX::XMMATRIX wtm = *reinterpret_cast<DirectX::XMMATRIX*>(const_cast<Pg::Math::PGFLOAT4X4*>(&camera->GetViewMatrix()));
	//DirectX::XMVECTOR d;
	//auto im = *reinterpret_cast<Pg::Math::PGFLOAT4X4*>(&DirectX::XMMatrixInverse(&d, wtm));

}

void Pg::UI::Helper::Gizmo::SetTransform(Pg::Data::Transform* trans)
{
	//auto ftm = *reinterpret_cast<DirectX::XMcamera->GetProjMatrix()RIX*>(const_cast<yunuGI::camera->GetProjMatrix()rix4x4*>(&camera->GetProjMatrix()));
	//DirectX::XMVECTOR scale;
	//DirectX::XMVECTOR rotation;
	//DirectX::XMVECTOR position;

	//DirectX::XMcamera->GetProjMatrix()rixDecompose(&scale, &rotation, &position, ftm);

	//yunuGI::Vector4 fs = *reinterpret_cast<yunuGI::Vector4*>(&scale);
	//yunuGI::Vector4 fr = *reinterpret_cast<yunuGI::Vector4*>(&rotation);
	//yunuGI::Vector4 fp = *reinterpret_cast<yunuGI::Vector4*>(&position);

	//fr.x *= -1;
	//fr.y *= -1;
	//auto finalcamera->GetProjMatrix() = glm::translate(glm::camera->GetProjMatrix()4(1.0f), glm::vec3(fp.x, fp.y, -fp.z)) * glm::camera->GetProjMatrix()4_cast(*reinterpret_cast<glm::quat*>(&fr)) * glm::scale(glm::camera->GetProjMatrix()4(1.0f), glm::vec3(fs.x, fs.y, fs.z));

}

void Pg::UI::Helper::Gizmo::SetWindowSize(float width, float height)
{
	_displayWidth = width;
	_displayHeight = height;
}

void Pg::UI::Helper::Gizmo::ConvertPTM(Pg::Math::PGFLOAT4X4& mt)
{

}

void Pg::UI::Helper::Gizmo::ConvertVTM(Pg::Math::PGFLOAT4X4& mt)
{

}

void Pg::UI::Helper::Gizmo::ConvertWTM(Pg::Math::PGFLOAT4X4& mt)
{

}

void Pg::UI::Helper::Gizmo::DrawGizmo()
{
	ImGuizmo::SetRect(0, 0, _displayWidth, _displayHeight);

	//ImGuizmo::Manipulate(
	//	&(_cameraView[0][0]),
	//	&(_cameraProj[0][0]),
	//	ImGuizmo::TRANSLATE,
	//	ImGuizmo::LOCAL,
	//	&(_transcamera->GetProjMatrix()rix[0][0])
	//);
}
