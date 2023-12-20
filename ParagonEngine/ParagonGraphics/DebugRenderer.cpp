#include "DebugRenderer.h"

#include "DX11Headers.h"
#include "LowDX11Logic.h"
#include "LowDX11Storage.h"
#include "MathHelper.h"

#include "../ParagonData/CameraData.h"

#include <DirectXMath.h>
#include <DirectXColors.h>

namespace Pg::Graphics
{
	using Pg::Graphics::Helper::MathHelper;

	DebugRenderer::DebugRenderer() : _DXStorage(LowDX11Storage::GetInstance()), _DXLogic(LowDX11Logic::GetInstance())
	{

	}

	void DebugRenderer::Initialize()
	{
		InitGeometry();
		InitLine();
	}

	void DebugRenderer::Render(Pg::Data::CameraData* camData)
	{
		BeginGeoPrimitiveRender();
		GeoPrimitiveRender(camData);
		EndGeoPrimitiveRender();

		BeginPrimitiveBatchRender(camData);
		LineRender();
		EndPrimitiveBatchRender();
	}

	void DebugRenderer::InitGeometry()
	{
		//Box & Sphere 만들기.
		_boxShape = DirectX::GeometricPrimitive::CreateBox(_DXStorage->_deviceContext, DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));

		size_t tTesellationFactor = 16;
		_sphereShape = DirectX::GeometricPrimitive::CreateSphere(_DXStorage->_deviceContext, 1.f, tTesellationFactor);


		//Capsule 만들기.
		{
			//Capsule을 위한 Cylinder를 만들기.
			{
				_cylinderShape = DirectX::GeometricPrimitive::CreateCylinder(_DXStorage->_deviceContext, 1.f, 1.f, tTesellationFactor);
			}

			//Top Hemisphere 만들기.
			{
				using namespace DirectX;

				GeometricPrimitive::VertexCollection vertices;
				GeometricPrimitive::IndexCollection indices;
				GeometricPrimitive::CreateSphere(vertices, indices,
					1.f, tTesellationFactor / 2);

				for (size_t i = 0; i < vertices.size(); i++)
				{
					vertices[i].position.y = fabs(vertices[i].position.y);
				}

				_topHemisphereShape = GeometricPrimitive::CreateCustom(_DXStorage->_deviceContext, vertices, indices);
			}

			//Bottom Hemisphere 만들기.
			{
				using namespace DirectX;

				GeometricPrimitive::VertexCollection vertices;
				GeometricPrimitive::IndexCollection indices;
				GeometricPrimitive::CreateSphere(vertices, indices,
					1.f, tTesellationFactor/2);

				for (size_t i = 0; i < vertices.size(); i++)
				{
					vertices[i].position.y = -fabs(vertices[i].position.y);
				}

				_bottomHemisphereShape = GeometricPrimitive::CreateCustom(_DXStorage->_deviceContext, vertices, indices);
			}
		}
	}

	void DebugRenderer::InitLine()
	{
		//Line 관련된 인스턴스 만들기.

		//DrawLine에 활용!
		_primitiveBatch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>(_DXStorage->_deviceContext);
		_basicEffect = std::make_unique<DirectX::BasicEffect>(_DXStorage->_device);
		_basicEffect->SetVertexColorEnabled(true);

		void const* shaderByteCode;
		size_t byteCodeLength;

		_basicEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

		HR(_DXStorage->_device->CreateInputLayout(DirectX::VertexPositionColor::InputElements,
			DirectX::VertexPositionColor::InputElementCount,
			shaderByteCode, byteCodeLength,
			&_debugLineInputLayout));

		_commonStates = std::make_unique<DirectX::CommonStates>(_DXStorage->_device);
	}

	void DebugRenderer::BeginGeoPrimitiveRender()
	{
		//
	}

	void DebugRenderer::GeoPrimitiveRender(Pg::Data::CameraData* camData)
	{
		//무조건 Wireframe으로 그린다.
		//DebugSystem이랑 연동되면 실제 Collider 정보랑 연동되어 출력될 예정.
		//2023.11.21 : Box.

		//Box Collider Rendering (연동됨)
		for (int i = 0; i < _boxColVector->size(); i++)
		{
			DrawBox(camData, _boxColVector->at(i));
		}

		for (int i = 0; i < _sphereColVector->size(); i++)
		{
			DrawSphere(camData, _sphereColVector->at(i));
		}
		
		for (int i = 0; i < _capsuleColVector->size(); i++)
		{
			DrawCapsule(camData, _capsuleColVector->at(i));
		}

		//Pg::Data::CapsuleInfo t;
		//t.color = { 0,1,0,1 };
		//
		//DirectX::XMMATRIX tTempScale = DirectX::XMMatrixScaling(1.f, 1.f, 1.f);
		//DirectX::XMMATRIX tTempRot;
		//{
		//	using namespace DirectX;
		//	static float tRotAmount = 0.f;
		//	tRotAmount += 1.f;
		//	float tActualRot = fmod(tRotAmount, 360.f);
		//
		//	DirectX::XMFLOAT3 tCylinderEulerDegRot = { tRotAmount, 0.f, 0.f };
		//	DirectX::XMFLOAT3 tCylinderEulerRadRot = { XMConvertToRadians(tCylinderEulerDegRot.x), XMConvertToRadians(tCylinderEulerDegRot.y),XMConvertToRadians(tCylinderEulerDegRot.z) };
		//	DirectX::XMVECTOR tCylinderEulerRadRotVec = DirectX::XMLoadFloat3(&tCylinderEulerRadRot);
		//	//DirectX::XMFLOAT3 tCylinderScale = { 2.f,.f, 1.f };
		//	float tHeight = 2.0f;
		//	float tRadius = 1.0f;
		//
		//	//<>//
		//	//Cylinder 연산에 필요할 것들. (위 내용과 자동 연동)
		//	DirectX::XMVECTOR tRotQuat = XMQuaternionRotationRollPitchYawFromVector(tCylinderEulerRadRotVec);
		//	tTempRot = DirectX::XMMatrixRotationQuaternion(tRotQuat);
		//}
		//auto tTempTrans = DirectX::XMMatrixTranslation(0.f, 2.f, -3.f);
		//
		//{
		//	using namespace DirectX;
		//	DirectX::XMMATRIX tWorldTM = tTempScale * tTempRot * tTempTrans;
		//	std::memcpy(&(t.worldTM), &tWorldTM, sizeof(DirectX::XMMATRIX));
		//}
		//
		//DrawCapsule(camData, &t);
	}

	void DebugRenderer::EndGeoPrimitiveRender()
	{
		//
	}

	void DebugRenderer::BeginPrimitiveBatchRender(Pg::Data::CameraData* camData)
	{
		_basicEffect->SetWorld(DirectX::XMMatrixIdentity());
		_basicEffect->SetView(MathHelper::PG2XM_MATRIX(camData->_viewMatrix));
		_basicEffect->SetProjection(MathHelper::PG2XM_MATRIX(camData->_projMatrix));

		_basicEffect->Apply(_DXStorage->_deviceContext);

		_DXStorage->_deviceContext->IASetInputLayout(_debugLineInputLayout);
		_primitiveBatch->Begin();
	}

	void DebugRenderer::LineRender()
	{
		//DebugSystem이랑 연동되면 실제 Line 정보랑 연동되어 출력될 예정.

		for (int i = 0; i < _lineColVector->size(); i++)
		{
			Pg::Data::LineInfo it = _lineColVector->at(i);
			DrawLine(&it);
		}
	}

	void DebugRenderer::EndPrimitiveBatchRender()
	{
		_primitiveBatch->End();
	}

	void DebugRenderer::GetDebugBoxGeometryData(const std::vector<Pg::Data::BoxInfo*>& const boxColVec)
	{
		_boxColVector = &boxColVec;
	}

	void DebugRenderer::GetDebugSphereGeometryData(const std::vector<Pg::Data::SphereInfo*>& const sphereColVec)
	{
		_sphereColVector = &sphereColVec;
	}

	void DebugRenderer::GetDebugCapsuleGeometryData(const std::vector<Pg::Data::CapsuleInfo*>& const capsuleColVec)
	{
		_capsuleColVector = &capsuleColVec;
	}

	void DebugRenderer::GetDebugLineGeometryData(const std::vector<Pg::Data::LineInfo>& const lineColVec)
	{
		_lineColVector = &lineColVec;
	}

	void DebugRenderer::DrawBox(Pg::Data::CameraData* camData, Pg::Data::BoxInfo* boxInfo)
	{
		using namespace DirectX;

		XMMATRIX tWorld = MathHelper::PG2XM_MATRIX(boxInfo->worldTM);
		XMMATRIX tView = MathHelper::PG2XM_MATRIX(camData->_viewMatrix);
		XMMATRIX tProj = MathHelper::PG2XM_MATRIX(camData->_projMatrix);

		DirectX::XMVECTOR tLineColor = MathHelper::PG2XM_VECTOR(boxInfo->color);

		{
			///PVD 연동 디버깅

			XMVECTOR tTrans;
			XMVECTOR tRotQuat;
			XMVECTOR tScale;
			XMMatrixDecompose(&tScale, &tRotQuat, &tTrans, tWorld);

			////Scale Fix 
			//tScale = XMVectorScale(tScale, 2);
			XMFLOAT3 infoScale = MathHelper::PG2XM_FLOAT3(boxInfo->scale);
			tScale = XMLoadFloat3(&infoScale);

			DirectX::XMMATRIX tZNinety = XMMatrixRotationZ(XMConvertToRadians(0.0f));
			DirectX::XMMATRIX tOriginRot = XMMatrixRotationQuaternion(tRotQuat);
			tRotQuat = XMQuaternionRotationMatrix(XMMatrixMultiply(tZNinety, tOriginRot));

			tWorld = XMMatrixAffineTransformation(tScale, XMVectorZero(), tRotQuat, tTrans);
			///

		}

		_boxShape->Draw(tWorld, tView, tProj, tLineColor, nullptr, true);
	}

	void DebugRenderer::DrawSphere(Pg::Data::CameraData* camData, Pg::Data::SphereInfo* sphereInfo)
	{
		using namespace DirectX;

		XMMATRIX tWorld = MathHelper::PG2XM_MATRIX(sphereInfo->worldTM);
		XMMATRIX tView = MathHelper::PG2XM_MATRIX(camData->_viewMatrix);
		XMMATRIX tProj = MathHelper::PG2XM_MATRIX(camData->_projMatrix);

		DirectX::XMVECTOR tLineColor = MathHelper::PG2XM_VECTOR(sphereInfo->color);

		{
			///PVD 연동 디버깅

			XMVECTOR tTrans;
			XMVECTOR tRotQuat;
			XMVECTOR tScale;
			XMMatrixDecompose(&tScale, &tRotQuat, &tTrans, tWorld);

			//Scale Fix 
			//tScale = XMVectorScale(tScale, 2);
			XMFLOAT3 infoScale = MathHelper::PG2XM_FLOAT3(sphereInfo->scale);
			tScale = XMLoadFloat3(&infoScale);

			DirectX::XMMATRIX tZNinety = XMMatrixRotationZ(XMConvertToRadians(0.0f));
			DirectX::XMMATRIX tOriginRot = XMMatrixRotationQuaternion(tRotQuat);
			tRotQuat = XMQuaternionRotationMatrix(XMMatrixMultiply(tZNinety, tOriginRot));

			tWorld = XMMatrixAffineTransformation(tScale, XMVectorZero(), tRotQuat, tTrans);
			///

		}
		_sphereShape->Draw(tWorld, tView, tProj, tLineColor, nullptr, true);
	}

	void DebugRenderer::DrawCapsule(Pg::Data::CameraData* camData, Pg::Data::CapsuleInfo* capsuleInfo)
	{
		using namespace DirectX;

		////매개변수 역할.
	
		
		XMMATRIX tCapsuleWorldTM = MathHelper::PG2XM_MATRIX(capsuleInfo->worldTM);

		DirectX::XMVECTOR tCylinderScaleVec;
		DirectX::XMFLOAT3 tCylinderScale = { 1.f, 1.f, 1.f };
		DirectX::XMVECTOR tCylinderEulerRadRotVec;
		DirectX::XMVECTOR tCylinderPosVec;
		DirectX::XMFLOAT3 tCylinderPos = {0.f,0.f,0.f};
		XMMatrixDecompose(&tCylinderScaleVec, &tCylinderEulerRadRotVec, &tCylinderPosVec, tCapsuleWorldTM);

		//Scale Fix 
		//tCylinderScaleVec = XMVectorScale(tCylinderScaleVec, 2);

		XMStoreFloat3(&tCylinderScale, tCylinderScaleVec);
		XMStoreFloat3(&tCylinderPos, tCylinderPosVec);

		tCylinderScale.x = capsuleInfo->scale.z;
		tCylinderScale.y = capsuleInfo->scale.x;
		tCylinderScale.z = capsuleInfo->scale.y;

		float tHeight = 1.0f;
		float tRadius = 1.0f;

		//float tHeight = capsuleInfo->scale.x /2.f;
		//float tRadius = (capsuleInfo->scale.y + capsuleInfo->scale.z) / 2.0f;

		//<>//
		//Cylinder 연산에 필요할 것들. (위 내용과 자동 연동)
		DirectX::XMVECTOR tRotQuat = XMQuaternionRotationRollPitchYawFromVector(tCylinderEulerRadRotVec);
		{
			using namespace DirectX;
			///PVD 연동 디버깅
			DirectX::XMMATRIX tZNinety = XMMatrixRotationZ(XMConvertToRadians(0.0f));
			DirectX::XMMATRIX tOriginRot = XMMatrixRotationQuaternion(tRotQuat);
			tRotQuat = XMQuaternionRotationMatrix(XMMatrixMultiply(tZNinety, tOriginRot));
			///
		}
		
		//오브젝트의 Translation을 기준으로 돌아야 한다.
		DirectX::XMMATRIX tTransformRotMat = DirectX::XMMatrixRotationQuaternion(tRotQuat);

		DirectX::XMMATRIX tTransformScaleMat = DirectX::XMMatrixScaling(tCylinderScale.x, tCylinderScale.y, tCylinderScale.z);
		DirectX::XMMATRIX tSelfScaleMat = DirectX::XMMatrixScaling(tRadius, tHeight, tRadius);
		DirectX::XMMATRIX tHeightlessScaleMat = DirectX::XMMatrixScaling(tRadius, 1.f, tRadius);

		XMMATRIX tView = MathHelper::PG2XM_MATRIX(camData->_viewMatrix);
		XMMATRIX tProj = MathHelper::PG2XM_MATRIX(camData->_projMatrix);

		//Cylinder Collider Rendering
		{
			//Cylinder World Mat
			DirectX::XMMATRIX tScale = tTransformScaleMat * tSelfScaleMat;
			DirectX::XMMATRIX tRot = tTransformRotMat;
			DirectX::XMMATRIX tTrans = DirectX::XMMatrixTranslation(tCylinderPos.x, tCylinderPos.y, tCylinderPos.z);
			DirectX::XMMATRIX tWorld = tScale * tRot * tTrans;
			//tWorld *= tTrans;

			_cylinderShape->Draw(tWorld, tView, tProj, Colors::Green, nullptr, true);
		}
		//Hemispheres
		//Top Hemisphere Collider Rendering
		{
			DirectX::XMFLOAT3 tTransOffsetFT = { 0.f, tHeight / 2.f, 0.f };
			DirectX::XMVECTOR tTransOffsetVec = DirectX::XMLoadFloat3(&tTransOffsetFT);

			//이 tTransVec가 tCylinderPos를 중심으로 Rotation을 거쳐야 한다.
			DirectX::XMMATRIX tTransOffsetMat = DirectX::XMMatrixTranslationFromVector(tTransOffsetVec);

			DirectX::XMMATRIX tTrans = DirectX::XMMatrixTranslation(tCylinderPos.x, tCylinderPos.y, tCylinderPos.z);
			DirectX::XMMATRIX tRot = tTransformRotMat;
			DirectX::XMMATRIX tScale = tTransformScaleMat * tHeightlessScaleMat;
			DirectX::XMMATRIX tWorld = tTransOffsetMat * tScale * tRot * tTrans;

			_topHemisphereShape->Draw(tWorld, tView, tProj, Colors::Green, nullptr, true);
		}

		//Bottom Hemisphere Collider Rendering
		{
			DirectX::XMFLOAT3 tTransOffsetFT = { 0.f, -tHeight / 2.f, 0.f };
			DirectX::XMVECTOR tTransOffsetVec = DirectX::XMLoadFloat3(&tTransOffsetFT);

			//이 tTransVec가 tCylinderPos를 중심으로 Rotation을 거쳐야 한다.
			DirectX::XMMATRIX tTransOffsetMat = DirectX::XMMatrixTranslationFromVector(tTransOffsetVec);

			DirectX::XMMATRIX tTrans = DirectX::XMMatrixTranslation(tCylinderPos.x, tCylinderPos.y, tCylinderPos.z);
			DirectX::XMMATRIX tRot = tTransformRotMat;
			DirectX::XMMATRIX tScale = tTransformScaleMat * tHeightlessScaleMat;
			DirectX::XMMATRIX tWorld = tTransOffsetMat * tScale * tRot * tTrans;

			_bottomHemisphereShape->Draw(tWorld, tView, tProj, Colors::Green, nullptr, true);
		}
	}

	void DebugRenderer::DrawLine(Pg::Data::LineInfo* lineInfo)
	{
		_primitiveBatch->DrawLine(
			DirectX::VertexPositionColor(MathHelper::PG2XM_FLOAT3(lineInfo->beginPoint), MathHelper::PG2XM_FLOAT4(lineInfo->color)),
			DirectX::VertexPositionColor(MathHelper::PG2XM_FLOAT3(lineInfo->endPoint), MathHelper::PG2XM_FLOAT4(lineInfo->color)));
	}
	

}