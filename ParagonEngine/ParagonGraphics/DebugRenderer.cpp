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
		InitCapsule();
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

		XMMATRIX tWorld = MathHelper::PG2XM_MATRIX(capsuleInfo->worldTM);
		XMMATRIX tView = MathHelper::PG2XM_MATRIX(camData->_viewMatrix);
		XMMATRIX tProj = MathHelper::PG2XM_MATRIX(camData->_projMatrix);

		DirectX::XMVECTOR tLineColor = MathHelper::PG2XM_VECTOR(capsuleInfo->color);

		{
			///PVD 연동 디버깅

			XMVECTOR tTrans;
			XMVECTOR tRotQuat;
			XMVECTOR tScale;
			XMMatrixDecompose(&tScale, &tRotQuat, &tTrans, tWorld);

			//Scale Fix 
			//tScale = XMVectorScale(tScale, 2);
			XMFLOAT3 infoScale = MathHelper::PG2XM_FLOAT3(capsuleInfo->scale);
			tScale = XMLoadFloat3(&infoScale);

			DirectX::XMMATRIX tZNinety = XMMatrixRotationZ(XMConvertToRadians(0.0f));
			DirectX::XMMATRIX tOriginRot = XMMatrixRotationQuaternion(tRotQuat);
			tRotQuat = XMQuaternionRotationMatrix(XMMatrixMultiply(tZNinety, tOriginRot));

			tWorld = XMMatrixAffineTransformation(tScale, XMVectorZero(), tRotQuat, tTrans);
			///

		}
		_capsuleShape->Draw(tWorld, tView, tProj, tLineColor, nullptr, true);
	}
	
	void DebugRenderer::DrawLine(Pg::Data::LineInfo* lineInfo)
	{
		_primitiveBatch->DrawLine(
			DirectX::VertexPositionColor(MathHelper::PG2XM_FLOAT3(lineInfo->beginPoint), MathHelper::PG2XM_FLOAT4(lineInfo->color)),
			DirectX::VertexPositionColor(MathHelper::PG2XM_FLOAT3(lineInfo->endPoint), MathHelper::PG2XM_FLOAT4(lineInfo->color)));
	}

	void DebugRenderer::InitCapsule()
	{
		float radius = 0.5f; // 캡슐의 반지름
		float height = 1.0f; // 캡슐의 높이
		int stackCount = 5; // 수평 분할
		int sliceCount = 20; // 수직 분할

		DirectX::GeometricPrimitive::VertexCollection vertices;
		DirectX::GeometricPrimitive::IndexCollection indices;

		// 상단 반구 정점
		vertices.push_back(DirectX::VertexPositionNormalTexture{
		   DirectX::SimpleMath::Vector3{0.0f, radius + height * 0.5f, 0.0f},
		   DirectX::SimpleMath::Vector3{0.f, 1.f, 0.f},DirectX::SimpleMath::Vector2{0.f,0.f} });

		for (int i = 1; i <= stackCount; i++)
		{
			// 윗방향 벡터와의 각도
			float upTheta = DirectX::XM_PI * 0.5f * (i / static_cast<float>(stackCount));

			float xzsize = radius * sinf(upTheta);
			float ysize = radius * cosf(upTheta);

			for (int j = 0; j < sliceCount; j++)
			{
				float zTheta = DirectX::XM_PI * 2.0f * (j / static_cast<float>(sliceCount));

				float x = xzsize * sinf(zTheta);
				float y = ysize + height * 0.5f;
				float z = xzsize * cosf(zTheta);

				vertices.push_back(DirectX::VertexPositionNormalTexture{
				   DirectX::SimpleMath::Vector3{x, y, z},
				   DirectX::SimpleMath::Vector3{0.f, 1.f, 0.f},
				   DirectX::SimpleMath::Vector2{0.f,0.f} }
				);
			}
		}

		size_t middleIdx = vertices.size();

		// 하단 반구 정점
		for (int i = stackCount; i >= 1; i--)
		{
			// 윗방향 벡터와의 각도
			float upTheta = DirectX::XM_PI * 0.5f * (i / static_cast<float>(stackCount));

			float xzsize = radius * sinf(upTheta);
			float ysize = radius * cosf(upTheta);

			for (int j = 0; j < sliceCount; j++)
			{
				float zTheta = DirectX::XM_PI * 2.0f * (j / static_cast<float>(sliceCount));

				float x = xzsize * sinf(zTheta);
				float y = ysize + height * 0.5f;
				float z = xzsize * cosf(zTheta);

				vertices.push_back(DirectX::VertexPositionNormalTexture{
				   DirectX::SimpleMath::Vector3(x, -y, z),
				   DirectX::SimpleMath::Vector3{0.f, 1.f, 0.f},
				   DirectX::SimpleMath::Vector2{0.f,0.f} }
				);
			}
		}

		vertices.push_back(DirectX::VertexPositionNormalTexture{
		   DirectX::SimpleMath::Vector3{0.0f, -(radius + height * 0.5f), 0.0f},
			DirectX::SimpleMath::Vector3{0.f, 1.f, 0.f},
				   DirectX::SimpleMath::Vector2{0.f,0.f} }
		);

		// 상단 반구 인덱스
		for (int i = 0; i < sliceCount; i++) {
			int a = 0;
			int b = 1 + i;
			int c = 1 + ((i + 1) % sliceCount);

			indices.push_back(a);
			indices.push_back(b);
			indices.push_back(c);
		}

		for (int i = 1; i < stackCount; i++) {
			for (int j = 0; j < sliceCount; j++) {
				int a = 1 + (i - 1) * sliceCount + j;
				int b = 1 + (i - 1) * sliceCount + ((j + 1) % sliceCount);
				int c = 1 + i * sliceCount + j;
				int d = 1 + i * sliceCount + ((j + 1) % sliceCount);

				indices.push_back(a);
				indices.push_back(c);
				indices.push_back(d);

				indices.push_back(a);
				indices.push_back(d);
				indices.push_back(b);
			}
		}

		// 실린더 부분 인덱스
		for (int i = 0; i < sliceCount; i++)
		{
			int a = middleIdx - sliceCount + i;
			int b = middleIdx - sliceCount + ((i + 1) % sliceCount);
			int c = middleIdx + i;
			int d = middleIdx + ((i + 1) % sliceCount);

			indices.push_back(a);
			indices.push_back(c);
			indices.push_back(d);

			indices.push_back(a);
			indices.push_back(d);
			indices.push_back(b);
		}

		// 하단 반구 인덱스
		for (int i = 1; i < stackCount; i++) {
			for (int j = 0; j < sliceCount; j++) {
				int a = middleIdx + (i - 1) * sliceCount + j;
				int b = middleIdx + (i - 1) * sliceCount + ((j + 1) % sliceCount);
				int c = middleIdx + i * sliceCount + j;
				int d = middleIdx + i * sliceCount + ((j + 1) % sliceCount);

				indices.push_back(a);
				indices.push_back(c);
				indices.push_back(d);

				indices.push_back(a);
				indices.push_back(d);
				indices.push_back(b);
			}
		}

		for (int i = 0; i < sliceCount; i++) {
			int a = vertices.size() - 1;
			int b = vertices.size() - 1 - sliceCount + i;
			int c = vertices.size() - 1 - sliceCount + ((i + 1) % sliceCount);

			indices.push_back(b);
			indices.push_back(a);
			indices.push_back(c);
		}

		//
		_capsuleShape = DirectX::GeometricPrimitive::CreateCustom(_DXStorage->_deviceContext, vertices, indices);
	}



}