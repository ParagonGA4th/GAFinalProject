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
	//_ptm = glm::perspective(glm::radians(camera->_fovY), 16.0f / 9.0f, camera->_nearZ, camera->_farZ);


	// view
	_vtm = ConvertVTM(camera->GetViewMatrix());
	//DirectX::XMMATRIX viewMT = Pg::Math::PG2XM_MATRIX4X4(camera->GetViewMatrix());

	//_vtm[2][0] = -DirectX::XMVectorGetX(viewMT.r[2]);
	//_vtm[2][1] = -DirectX::XMVectorGetY(viewMT.r[2]);
	//_vtm[2][2] = -DirectX::XMVectorGetZ(viewMT.r[2]);
	//_vtm[2][3] = -DirectX::XMVectorGetW(viewMT.r[2]);

	//// 나머지 행들 복사
	//_vtm[0] = glm::make_vec4(&viewMT.r[0].m128_f32[0]);
	//_vtm[1] = glm::make_vec4(&viewMT.r[1].m128_f32[0]);
	//_vtm[3] = glm::make_vec4(&viewMT.r[3].m128_f32[0]);
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

void Pg::UI::Helper::Gizmo::ConvertPTM(glm::mat4 mt)
{

}

glm::mat4 Pg::UI::Helper::Gizmo::ConvertVTM(Pg::Math::PGFLOAT4X4 mt)
{
	//auto ftm = Pg::Math::PG2XM_MATRIX4X4(mt);

	//DirectX::XMVECTOR scale;
	//DirectX::XMVECTOR rotation;
	//DirectX::XMVECTOR position;

	//DirectX::XMMatrixDecompose(&scale, &rotation, &position, ftm);



	return glm::inverse(ConvertWTM(mt.Inverse()));
}

void Pg::UI::Helper::Gizmo::ConvertVTM(glm::mat4 mt)
{

}

glm::mat4 Pg::UI::Helper::Gizmo::ConvertWTM(Pg::Math::PGFLOAT4X4 mt)
{
	auto ftm = Pg::Math::PG2XM_MATRIX4X4(mt);

	DirectX::XMVECTOR scale;
	DirectX::XMVECTOR rotation;
	DirectX::XMVECTOR position;

	DirectX::XMMatrixDecompose(&scale, &rotation, &position, ftm);

	Pg::Math::PGFLOAT4 fs = Pg::Math::XM2PG_FLOAT4_VECTOR(scale);
	Pg::Math::PGFLOAT4 fr = Pg::Math::XM2PG_FLOAT4_VECTOR(rotation);
	Pg::Math::PGFLOAT4 fp = Pg::Math::XM2PG_FLOAT4_VECTOR(position);


	Pg::Math::PGQuaternion tFRQuat;
	tFRQuat.w = fr.w;
	tFRQuat.x = fr.x;
	tFRQuat.y = fr.y;
	tFRQuat.z = fr.z;


	Pg::Math::PGFLOAT3 tEulerRot = Pg::Math::PGQuaternionToEuler(tFRQuat);
	tEulerRot.x *= -1.f;
	tEulerRot.y *= -1.f;

	tFRQuat = Pg::Math::PGEulerToQuaternion(tEulerRot);
	fr.w = tFRQuat.w;
	fr.x = tFRQuat.x;
	fr.y = tFRQuat.y;
	fr.z = tFRQuat.z;

	//fr.x *= -1;
	//fr.y *= -1;

	auto finalMat = 
		glm::translate(glm::mat4(1.0f), 
		glm::vec3(fp.x, fp.y, -fp.z)) * glm::mat4_cast(*reinterpret_cast<glm::quat*>(&fr)) * glm::scale(glm::mat4(1.0f), 
		glm::vec3(fs.x, fs.y, fs.z));

	return finalMat;
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
	_trans->_rotation = { -rotation.x, -rotation.y, rotation.z, rotation.w };
	_trans->_scale = { scale.x, scale.y, scale.z };
}

void Pg::UI::Helper::Gizmo::DrawGizmo()
{
	if (_trans != nullptr)
	{
		//auto ftm = Pg::Math::PG2XM_MATRIX4X4(_trans->GetWorldTM());

		//DirectX::XMVECTOR scale;
		//DirectX::XMVECTOR rotation;
		//DirectX::XMVECTOR position;

		//DirectX::XMMatrixDecompose(&scale, &rotation, &position, ftm);

		//Pg::Math::PGFLOAT4 fs = Pg::Math::XM2PG_FLOAT4_VECTOR(scale);
		//Pg::Math::PGFLOAT4 fr = Pg::Math::XM2PG_FLOAT4_VECTOR(rotation);
		//Pg::Math::PGFLOAT4 fp = Pg::Math::XM2PG_FLOAT4_VECTOR(position);


		////auto t = DirectX::XMMatrixTranslation(fp.x, fp.y, -fp.z);
		////t = DirectX::XMMatrixTranspose(t);
		////DirectX::XMFLOAT4X4 tVal;
		////DirectX::XMStoreFloat4x4(&tVal, t);
		//glm::vec3 trans_vec(fp.x, fp.y, -fp.z);
		//glm::mat4 Model = glm::mat4(1.0f); // 모델 좌표계 생성
		//_wtm = glm::translate(Model, trans_vec);

		ImGuizmo::SetRect(0, 0, _displayWidth, _displayHeight);

		ImGuizmo::Manipulate(
			&(_vtm[0][0]),		// cameraView
			&(_ptm[0][0]),		// cameraProj
			ImGuizmo::TRANSLATE,
			ImGuizmo::LOCAL,
			&(_wtm[0][0])		// object Transform
		);

		ConvertWTM(_wtm);
	}
}
