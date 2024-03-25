#include "Gizmo.h"

#include "imgui.h"
#include "ImGuizmo.h"

#include "../ParagonData/GameObject.h"

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
	_camera = camera;

	//proj
	_ptm = glm::perspective(camera->_fovY, ImGui::GetWindowSize().x / ImGui::GetWindowSize().y, camera->_nearZ, camera->_farZ);

	// view
	_vtm = ConvertVTM(camera->GetViewMatrix());
	//Pg::Math::PGFLOAT3 tpos = camera->_object->_transform._position;
	//_vtm = glm::lookAt(glm::vec3{ tpos.x, tpos.y, -tpos.z}, { 0.f, -0.1f, -1.f }, { 0,0,1 });
}

void Pg::UI::Helper::Gizmo::SetTransform(Pg::Data::Transform* trans)
{
	_trans = trans;

	if(_trans->GetParent() != nullptr) 	_wtm = ConvertWTM(trans->GetLocalTM());
	else _wtm = ConvertWTM(trans->GetWorldTM());
}

glm::mat4 Pg::UI::Helper::Gizmo::ConvertVTM(Pg::Math::PGFLOAT4X4 mt)
{
	return glm::inverse(ConvertWTM(mt.Inverse()));
}

glm::mat4 Pg::UI::Helper::Gizmo::ConvertWTM(Pg::Math::PGFLOAT4X4 mt)
{
	auto fmt = Pg::Math::PG2XM_MATRIX4X4(mt);

	DirectX::XMVECTOR ds;
	DirectX::XMVECTOR dr;
	DirectX::XMVECTOR dp;

	DirectX::XMMatrixDecompose(&ds, &dr, &dp, fmt);

	Pg::Math::PGFLOAT4 fs = Pg::Math::XM2PG_FLOAT4_VECTOR(ds);
	Pg::Math::PGFLOAT4 fr = Pg::Math::XM2PG_FLOAT4_VECTOR(dr);
	Pg::Math::PGFLOAT4 fp = Pg::Math::XM2PG_FLOAT4_VECTOR(dp);
	
	fr.x *= -1;
	fr.y *= -1;

	auto finalMt = 
		glm::translate(glm::mat4(1.0f), glm::vec3(fp.x, fp.y, -fp.z)) * 
		glm::mat4_cast(*reinterpret_cast<glm::quat*>(&fr)) *
		glm::scale(glm::mat4(1.0f), glm::vec3(fs.x, fs.y, fs.z));

	return finalMt;
}

void Pg::UI::Helper::Gizmo::ConvertWTM(glm::mat4 mt)
{
	glm::vec3 scale;
	glm::quat rotation;
	glm::vec3 position;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(mt, scale, rotation, position, skew, perspective);

	_trans->_position = { position.x, position.y, -position.z };
	_trans->_rotation = { rotation.w, -rotation.x, -rotation.y, rotation.z };
	_trans->_scale = { scale.x, scale.y, scale.z };
}

void Pg::UI::Helper::Gizmo::DrawGizmo(std::string type)
{
	ImGuizmo::SetDrawlist();

	if (_trans != nullptr)
	{
		ImGuizmo::OPERATION oper = ImGuizmo::TRANSLATE;

		if (type.compare("Translate") == 0) oper = ImGuizmo::TRANSLATE;
		if (type.compare("Rotate") == 0) oper = ImGuizmo::ROTATE;
		if (type.compare("Scale") == 0) oper = ImGuizmo::SCALE;


		ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);

		ImGuizmo::Manipulate(
			&(_vtm[0][0]),		// cameraView
			&(_ptm[0][0]),		// cameraProj
			oper,
			ImGuizmo::WORLD,
			&(_wtm[0][0])		// object Transform
		);

		//ImGuizmo::DrawCubes(
		//	&(_vtm[0][0]),		// cameraView
		//	&(_ptm[0][0]),		// cameraProj
		//	&(_wtm[0][0]),		// object Transform
		//	1
		//);

		ConvertWTM(_wtm);
	}
}
