#include "Gizmo.h"

#include "imgui.h"
#include "ImGuizmo.h"



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
	_ptm = ConvertPTM(camera->GetProjMatrix());	

	// view
	_vtm = ConvertVTM(camera->GetViewMatrix());
}

void Pg::UI::Helper::Gizmo::SetTransform(Pg::Data::Transform* trans)
{
	_trans = trans;
	_wtm = ConvertWTM(trans->GetWorldTM());
}

void Pg::UI::Helper::Gizmo::SetWindowSize(float width, float height)
{
	_displayWidth = width;
	_displayHeight = height;
}

glm::mat4 Pg::UI::Helper::Gizmo::ConvertPTM(Pg::Math::PGFLOAT4X4 mt)
{
	glm::mat4 finalTM;

	finalTM[0][0] = mt._11; finalTM[0][1] = mt._12; finalTM[0][2] = mt._13; finalTM[0][3] = mt._14;
	finalTM[1][0] = mt._21; finalTM[1][1] = mt._22; finalTM[1][2] = mt._23; finalTM[1][3] = mt._24;
	finalTM[2][0] = mt._31; finalTM[2][1] = mt._32; finalTM[2][2] = -mt._33; finalTM[2][3] = -mt._34;
	finalTM[3][0] = mt._41; finalTM[3][1] = mt._42; finalTM[3][2] = mt._43; finalTM[3][3] = mt._44;

	return finalTM;
}

glm::mat4 Pg::UI::Helper::Gizmo::ConvertVTM(Pg::Math::PGFLOAT4X4 mt)
{
	return glm::inverse(ConvertWTM(mt.Inverse()));
}

glm::mat4 Pg::UI::Helper::Gizmo::ConvertWTM(Pg::Math::PGFLOAT4X4 mt)
{
	auto ftm = Pg::Math::PG2XM_MATRIX4X4(mt.Transpose());

	DirectX::XMVECTOR ds;
	DirectX::XMVECTOR dr;
	DirectX::XMVECTOR dp;

	DirectX::XMMatrixDecompose(&ds, &dr, &dp, ftm);

	Pg::Math::PGFLOAT4 fs = Pg::Math::XM2PG_FLOAT4_VECTOR(ds);
	Pg::Math::PGFLOAT4 fr = Pg::Math::XM2PG_FLOAT4_VECTOR(dr);
	Pg::Math::PGFLOAT4 fp = Pg::Math::XM2PG_FLOAT4_VECTOR(dp);
	
	fr.x *= -1;
	fr.y *= -1;

	//Pg::Math::PGQuaternion tFRQuat;
	//tFRQuat.w = fr.w;
	//tFRQuat.x = fr.x;
	//tFRQuat.y = fr.y;
	//tFRQuat.z = fr.z;

	//Pg::Math::PGFLOAT3 tEulerRot = Pg::Math::PGQuaternionToEuler(tFRQuat);
	//tEulerRot.x *= -1;
	//tEulerRot.y *= -1;

	//glm::mat4 finalMt = glm::scale(glm::mat4(1.0f), glm::vec3(fs.x, fs.y, fs.z));
	//finalMt = glm::rotate(finalMt, fr.x, glm::vec3(tEulerRot.x, tEulerRot.y, tEulerRot.z));
	//finalMt = glm::translate(finalMt, glm::vec3(fp.x, fp.y, -fp.z));

	auto finalMt = 
		glm::scale(glm::mat4(1.0f), glm::vec3(fs.x, fs.y, fs.z)) *
		glm::mat4_cast(*reinterpret_cast<glm::quat*>(&fr)) *
		glm::translate(glm::mat4(1.0f), glm::vec3(fp.x, fp.y, -fp.z));

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

	_trans->_position = { position.x, position.y, position.z };
	_trans->_rotation = { rotation.w, -rotation.x, -rotation.y, rotation.z };
	_trans->_scale = { scale.x, scale.y, scale.z };
}

void Pg::UI::Helper::Gizmo::DrawGizmo()
{
	if (_trans != nullptr)
	{
		ImGuizmo::SetRect(0, 0, _displayWidth, _displayHeight);

		ImGuizmo::Manipulate(
			&(_vtm[0][0]),		// cameraView
			&(_ptm[0][0]),		// cameraProj
			ImGuizmo::TRANSLATE,
			ImGuizmo::LOCAL,
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
