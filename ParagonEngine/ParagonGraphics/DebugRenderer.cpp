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
		using namespace DirectX;

		XMMATRIX tView = MathHelper::PG2XM_MATRIX(camData->_viewMatrix);
		XMMATRIX tProj = MathHelper::PG2XM_MATRIX(camData->_projMatrix);

		//Box Collider Rendering (연동됨)
		for (int i = 0; i < _boxColVector->size(); i++)
		{
			DirectX::XMVECTOR tLineColor = MathHelper::PG2XM_VECTOR(_boxColVector->at(i)->color);
			_boxShape->Draw(MathHelper::PG2XM_MATRIX(_boxColVector->at(i)->worldTM), tView, tProj, tLineColor, nullptr, true);
		}

		//Capsule Rendering
		{
			//매개변수 역할.
			DirectX::XMFLOAT3 tCylinderPos = { 0.f, 2.f, -3.f };
			//DirectX::XMFLOAT3 tCylinderPos = { 0.f, 0.f,0.f};

			static float tRotAmount = 0.f;
			tRotAmount += 1.f;
			float tActualRot = fmod(tRotAmount, 360.f);

			DirectX::XMFLOAT3 tCylinderEulerDegRot = { tRotAmount, 0.f, 0.f };

			DirectX::XMFLOAT3 tCylinderEulerRadRot = { XMConvertToRadians(tCylinderEulerDegRot.x), XMConvertToRadians(tCylinderEulerDegRot.y),XMConvertToRadians(tCylinderEulerDegRot.z) };
			DirectX::XMVECTOR tCylinderEulerRadRotVec = DirectX::XMLoadFloat3(&tCylinderEulerRadRot);
			DirectX::XMFLOAT3 tCylinderScale = { 1.f, 1.f, 1.f };
			//DirectX::XMFLOAT3 tCylinderScale = { 2.f,.f, 1.f };
			float tHeight = 2.0f;
			float tRadius = 1.0f;

			//<>//
			//Cylinder 연산에 필요할 것들. (위 내용과 자동 연동)
			DirectX::XMVECTOR tRotQuat = XMQuaternionRotationRollPitchYawFromVector(tCylinderEulerRadRotVec);
			//오브젝트의 Translation을 기준으로 돌아야 한다.
			DirectX::XMMATRIX tTransformRotMat = DirectX::XMMatrixRotationQuaternion(tRotQuat);

			DirectX::XMMATRIX tTransformScaleMat = DirectX::XMMatrixScaling(tCylinderScale.x, tCylinderScale.y, tCylinderScale.z);
			DirectX::XMMATRIX tSelfScaleMat = DirectX::XMMatrixScaling(tRadius, tHeight, tRadius);
			DirectX::XMMATRIX tHeightlessScaleMat = DirectX::XMMatrixScaling(tRadius, 1.f, tRadius);
	
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

		DirectX::XMFLOAT3 tFirst = { 0.f, 0.f, 0.f };
		DirectX::XMFLOAT3 tSecond = { 0.f, 2.f, -3.f };
		DirectX::XMFLOAT4 tColor;
		DirectX::XMStoreFloat4(&tColor, DirectX::Colors::White);

		_primitiveBatch->DrawLine(
			DirectX::VertexPositionColor(tFirst, tColor),
			DirectX::VertexPositionColor(tSecond, tColor));
	}

	void DebugRenderer::EndPrimitiveBatchRender()
	{
		_primitiveBatch->End();
	}

	void DebugRenderer::GetDebugBoxGeometryData(const std::vector<Pg::Data::BoxInfo*>& const boxColVec)
	{
		_boxColVector = &boxColVec;
	}



}