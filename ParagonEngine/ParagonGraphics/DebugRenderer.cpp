#include "DebugRenderer.h"

#include "DX11Headers.h"
#include "LowDX11Logic.h"
#include "LowDX11Storage.h"
#include "MathHelper.h"
#include "RenderObjectWireframeList.h"
#include "WireframeRenderObject.h"
#include "LayoutDefine.h"

#include "../ParagonData/CameraData.h"
#include <dxtk/VertexTypes.h>
#include <DirectXMath.h>
#include <DirectXColors.h>

namespace Pg::Graphics
{
	using Pg::Graphics::Helper::MathHelper;

	DebugRenderer::DebugRenderer(D3DCarrier* d3dCarrier) : BaseSpecificRenderer(d3dCarrier), _DXStorage(LowDX11Storage::GetInstance()), _DXLogic(LowDX11Logic::GetInstance())
	{

	}

	void DebugRenderer::Initialize()
	{
		CreateSystemVertexShaders();
		CreateDepthWriteOffDSS();
		InitGeometry();
		InitLine();
	}

	void DebugRenderer::SetupRenderPasses()
	{

	}

	void DebugRenderer::RenderContents(void* renderObjectList, void* optionalRequirement, Pg::Data::CameraData* camData)
	{
		Render((RenderObjectWireframeList*)renderObjectList, camData);
	}

	void DebugRenderer::Render(RenderObjectWireframeList* wireframeList, Pg::Data::CameraData* camData)
	{
		_DXStorage->_deviceContext->OMSetRenderTargets(1, &(_carrier->_quadMainRT->GetRTV()), _carrier->_quadMainGDS->GetDSV());

		WireframeObjRender(wireframeList, camData);

		BeginGeoPrimitiveRender();
		GeoPrimitiveRender(camData);
		EndGeoPrimitiveRender();

		BeginPrimitiveBatchRender(camData);
		LineRender();
		EndPrimitiveBatchRender();

		BeginDebug2dRender(camData);
		Debug2dRender();
		EndDebug2dRender();
	}

	void DebugRenderer::ConfirmCarrierData()
	{

	}

	void DebugRenderer::InitGeometry()
	{
		//Box & Sphere 만들기.
		_boxShape = DirectX::GeometricPrimitive::CreateBox(_DXStorage->_deviceContext, DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));

		size_t tTesellationFactor = 16;
		_sphereShape = DirectX::GeometricPrimitive::CreateSphere(_DXStorage->_deviceContext, 1.f, tTesellationFactor);

		//Capsule 만들기.
		InitCapsule();


		InitPlane();
		//Box & Sphere 만들기.
		//_planeShape = DirectX::GeometricPrimitive::CreateBox(_DXStorage->_deviceContext, DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
	}

	void DebugRenderer::InitLine()
	{
		//Line 관련된 인스턴스 만들기.

		//DrawLine에 활용!
		_primitiveBatch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>(_DXStorage->_deviceContext);
		_basicEffect = std::make_unique<DirectX::BasicEffect>(_DXStorage->_device);
		_basicEffect->SetVertexColorEnabled(true);
		_basicEffect->SetLightingEnabled(false);

		//GeometricPrim Effect. -> NavMesh만 사용.
		_GeometricPrimEffect = std::make_unique<DirectX::BasicEffect>(_DXStorage->_device);
		_GeometricPrimEffect->SetLightingEnabled(false);

		_basicEffect2d = std::make_unique<DirectX::BasicEffect>(_DXStorage->_device);
		_basicEffect2d->SetVertexColorEnabled(true);
		_basicEffect2d->SetLightingEnabled(false);

		void const* shaderByteCode;
		size_t byteCodeLength;

		_basicEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

		HR(_DXStorage->_device->CreateInputLayout(DirectX::VertexPositionColor::InputElements,
			DirectX::VertexPositionColor::InputElementCount,
			shaderByteCode, byteCodeLength,
			&_debugLineInputLayout));

		_commonStates = std::make_unique<DirectX::CommonStates>(_DXStorage->_device);
	}

	void DebugRenderer::WireframeObjRender(RenderObjectWireframeList* wireframeList, Pg::Data::CameraData* camData)
	{
		//Layout, Topology, Shader, RS
		_primitiveVS->Bind();
		_primitivePS->Bind();

		_DXStorage->_deviceContext->OMSetDepthStencilState(_depthWriteOffDSS, 0xFF);

		for (auto& it : wireframeList->_list)
		{
			it->UpdateConstantBuffers(camData);
			it->BindConstantBuffers();
			it->Render();
			it->UnbindConstantBuffers();
		}

		_DXStorage->_deviceContext->OMSetDepthStencilState(nullptr, 0xFF);

		_primitiveVS->Unbind();
		_primitivePS->Unbind();
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

		for (int i = 0; i < _planeColVector->size(); i++)
		{
			DrawPlane(camData, _planeColVector->at(i));
		}

		//개별적으로 NavMesh Render.
		for (int i = 0; i < _navMeshVector->size(); i++)
		{
			DrawNavMesh(camData, _navMeshVector->at(i));
		}
	}

	void DebugRenderer::EndGeoPrimitiveRender()
	{

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

		for (int i = 0; i < _rayCastColVector->size(); i++)
		{
			DrawRayCast(_rayCastColVector->at(i));
		}
	}

	void DebugRenderer::EndPrimitiveBatchRender()
	{
		_primitiveBatch->End();
	}

	void DebugRenderer::BeginDebug2dRender(Pg::Data::CameraData* camData)
	{
		_basicEffect2d->Apply(_DXStorage->_deviceContext);

		_primitiveBatch->Begin();
	}

	void DebugRenderer::Debug2dRender()
	{
		for (int i = 0; i < _box2dVector->size(); i++)
		{
			Pg::Data::Box2DInfo it = _box2dVector->at(i);
			DrawBox2D(it);
		}
	}

	void DebugRenderer::EndDebug2dRender()
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

	void DebugRenderer::GetDebugRayCastGeometryData(const std::vector<Pg::Data::RayCastInfo>& const rayCastColVec)
	{
		_rayCastColVector = &rayCastColVec;
	}

	void DebugRenderer::GetDebugBox2dGeometryData(const std::vector<Pg::Data::Box2DInfo>& const box2DColVec)
	{
		_box2dVector = &box2DColVec;
	}

	void DebugRenderer::GetDebugNavMeshGeometryData(const std::vector<Pg::Data::NavMeshInfo*>& const navMeshVec)
	{
		_navMeshVector = &navMeshVec;
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

	void DebugRenderer::DrawNavMesh(Pg::Data::CameraData* camData, Pg::Data::NavMeshInfo* navInfo)
	{
		using DirectX::VertexPositionNormalTexture;

		//이 경우에는, 독특하게 미리 동일 이름으로 만들어 놓은 RenderData가 있는지를 기반으로 결정한다.
		if (!_navMeshPrimitiveVector.contains(navInfo->path))
		{
			//없으면 지금 만든다. NavMesh.
			DirectX::GeometricPrimitive::VertexCollection tVertices;
			DirectX::GeometricPrimitive::IndexCollection tIndices;

			for (auto& it : *navInfo->vertices)
			{
				tVertices.push_back(VertexPositionNormalTexture(DirectX::XMFLOAT3(it.x, it.y, it.z), { 0,1,0 }, { 0,0 }));
			}

			for (auto& it : *navInfo->indices)
			{
				tIndices.push_back(it);
			}

			_navMeshPrimitiveVector.insert(std::make_pair(navInfo->path, DirectX::GeometricPrimitive::CreateCustom(_DXStorage->_deviceContext, tVertices, tIndices)));
		}

		DirectX::XMMATRIX tView = MathHelper::PG2XM_MATRIX(camData->_viewMatrix);
		DirectX::XMMATRIX tProj = MathHelper::PG2XM_MATRIX(camData->_projMatrix);

		DirectX::XMFLOAT4 tColorF = { 0.2196f, 0.7019f, 0.8901f, 1.0f }; // 하늘.
		DirectX::XMVECTOR tColor = DirectX::XMLoadFloat4(&tColorF);

		_navMeshPrimitiveVector.at(navInfo->path)->Draw(DirectX::XMMatrixIdentity(), tView, tProj, tColor, nullptr, true);
	}

	void DebugRenderer::DrawLine(Pg::Data::LineInfo* lineInfo)
	{
		_primitiveBatch->DrawLine(
			DirectX::VertexPositionColor(MathHelper::PG2XM_FLOAT3(lineInfo->beginPoint), MathHelper::PG2XM_FLOAT4(lineInfo->color)),
			DirectX::VertexPositionColor(MathHelper::PG2XM_FLOAT3(lineInfo->endPoint), MathHelper::PG2XM_FLOAT4(lineInfo->color)));
	}

	void DebugRenderer::DrawRayCast(Pg::Data::RayCastInfo rayCastInfo)
	{
		Pg::Math::PGFLOAT3 tBeginPoint = rayCastInfo.origin;
		Pg::Math::PGFLOAT3 tEndPoint;
		DirectX::XMFLOAT4 tColor;

		if (rayCastInfo.isHit)
		{
			tEndPoint = rayCastInfo.hitPoint;
			DirectX::XMStoreFloat4(&tColor, DirectX::Colors::Gold);
		}
		else
		{
			tEndPoint = (rayCastInfo.origin) + (rayCastInfo.dir) * (rayCastInfo.length);
			DirectX::XMStoreFloat4(&tColor, DirectX::Colors::MediumVioletRed);
		}


		_primitiveBatch->DrawLine(
			DirectX::VertexPositionColor(MathHelper::PG2XM_FLOAT3(tBeginPoint), tColor),
			DirectX::VertexPositionColor(MathHelper::PG2XM_FLOAT3(tEndPoint), tColor));
	}

	void DebugRenderer::DrawPlane(Pg::Data::CameraData* camData, Pg::Data::PlaneInfo* planeInfo)
	{
		using namespace DirectX;

		XMMATRIX tWorld = MathHelper::PG2XM_MATRIX(planeInfo->worldTM);
		XMMATRIX tView = MathHelper::PG2XM_MATRIX(camData->_viewMatrix);
		XMMATRIX tProj = MathHelper::PG2XM_MATRIX(camData->_projMatrix);

		DirectX::XMVECTOR tLineColor = MathHelper::PG2XM_VECTOR(planeInfo->color);

		{
			///PVD 연동 디버깅

			XMVECTOR tTrans;
			XMVECTOR tRotQuat;
			XMVECTOR tScale;
			XMMatrixDecompose(&tScale, &tRotQuat, &tTrans, tWorld);

			//Scale Fix 
			//tScale = XMVectorScale(tScale, 2);
			XMFLOAT3 infoScale = MathHelper::PG2XM_FLOAT3(planeInfo->scale);
			tScale = XMLoadFloat3(&infoScale);

			DirectX::XMMATRIX tZNinety = XMMatrixRotationZ(XMConvertToRadians(0.0f));
			DirectX::XMMATRIX tOriginRot = XMMatrixRotationQuaternion(tRotQuat);
			tRotQuat = XMQuaternionRotationMatrix(XMMatrixMultiply(tZNinety, tOriginRot));

			tWorld = XMMatrixAffineTransformation(tScale, XMVectorZero(), tRotQuat, tTrans);
			///

		}
		_planeShape->Draw(tWorld, tView, tProj, tLineColor, nullptr, true);
	}

	void DebugRenderer::DrawBox2D(Pg::Data::Box2DInfo box2dInfo)
	{
		float tScreenWidth = _DXStorage->_screenWidth;
		float tScreenHeight = _DXStorage->_screenHeight;

		// Z는 0.5로 고정.
		//스크린 전체의 LT가 (-1,-1) // RB가 (1,1)로 생각하자.
		//이를 기초한 비율이어야!
		//float tLeftRatio = ((box2dInfo.LT.x / tScreenWidth) - 0.5f) * 2.f;
		//float tTopRatio = ((box2dInfo.LT.y / tScreenHeight) - 0.5f) * 2.f;
		//float tRightRatio = ((box2dInfo.RB.x / tScreenWidth) - 0.5f) * 2.f;
		//float tBottomRatio = ((box2dInfo.RB.y / tScreenHeight) - 0.5f) * 2.f;

		float tLeftRatio = ((box2dInfo.LT.x / tScreenWidth) * 2.f) - 1.0f;
		float tTopRatio = ((box2dInfo.LT.y / tScreenHeight) * 2.f) - 1.0f;
		float tRightRatio = ((box2dInfo.RB.x / tScreenWidth) * 2.f) - 1.0f;
		float tBottomRatio = ((box2dInfo.RB.y / tScreenHeight) * 2.f) - 1.0f;

		//float tLeftRatio = -1.0f;
		//float tTopRatio = -1.0f; // 늘어나면 y+
		//float tRightRatio = 1.0f;
		//float tBottomRatio = 1.0f; // 늘어나면 y-

		//float tLeftRatio = -0.5f;
		//float tTopRatio = -0.3f; // 늘어나면 y+
		//float tRightRatio = 0.5f;
		//float tBottomRatio = 0.5f; // 늘어나면 y-

		DirectX::XMFLOAT4 tColor = PG2XM_FLOAT4(box2dInfo.color);

		// -1 ~ 1의 범위의 NDC로 초기화.	
		DirectX::VertexPositionColor v1(DirectX::XMFLOAT3(tLeftRatio, -tBottomRatio, 0.5f), tColor);
		DirectX::VertexPositionColor v2(DirectX::XMFLOAT3(tRightRatio, -tBottomRatio, 0.5f), tColor);
		DirectX::VertexPositionColor v3(DirectX::XMFLOAT3(tRightRatio, -tTopRatio, 0.5f), tColor);
		DirectX::VertexPositionColor v4(DirectX::XMFLOAT3(tLeftRatio, -tTopRatio, 0.5f), tColor);

		_primitiveBatch->DrawLine(v1, v2);
		_primitiveBatch->DrawLine(v2, v3);
		_primitiveBatch->DrawLine(v3, v4);
		_primitiveBatch->DrawLine(v4, v1);
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
		   DirectX::SimpleMath::Vector3{1.f, 1.f, 1.f},DirectX::SimpleMath::Vector2{0.f,0.f} });

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
				   DirectX::SimpleMath::Vector3{1.f, 1.f, 1.f},
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
				   DirectX::SimpleMath::Vector3{1.f, 1.f, 1.f},
				   DirectX::SimpleMath::Vector2{0.f,0.f} }
				);
			}
		}

		vertices.push_back(DirectX::VertexPositionNormalTexture{
		   DirectX::SimpleMath::Vector3{0.0f, -(radius + height * 0.5f), 0.0f},
			DirectX::SimpleMath::Vector3{1.f, 1.f, 1.f},
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

	void DebugRenderer::GetDebugPlaneGeometryData(const std::vector<Pg::Data::PlaneInfo*>& const planeColVec)
	{
		_planeColVector = &planeColVec;
	}

	void DebugRenderer::InitPlane()
	{
		DirectX::GeometricPrimitive::VertexCollection vertices;
		DirectX::GeometricPrimitive::IndexCollection indices;

		//0
		vertices.push_back(DirectX::VertexPositionNormalTexture{
		   DirectX::SimpleMath::Vector3{-0.5f,0.f, 0.5f},
		   DirectX::SimpleMath::Vector3{0.9f, 0.9f, 0.9f},	DirectX::SimpleMath::Vector2{0.f,0.f} });

		//1
		vertices.push_back(DirectX::VertexPositionNormalTexture{
			DirectX::SimpleMath::Vector3{0.5f,0.f, 0.5f},
			 DirectX::SimpleMath::Vector3{0.9f, 0.9f, 0.9f},	DirectX::SimpleMath::Vector2{0.f,0.f} });

		//2
		vertices.push_back(DirectX::VertexPositionNormalTexture{
			DirectX::SimpleMath::Vector3{0.5f,0.f, -0.5f},
			DirectX::SimpleMath::Vector3{0.9f, 0.9f, 0.9f},	DirectX::SimpleMath::Vector2{0.f,0.f} });

		//3
		vertices.push_back(DirectX::VertexPositionNormalTexture{
			DirectX::SimpleMath::Vector3{-0.5f, 0.f, -0.5f},
		DirectX::SimpleMath::Vector3{0.9f, 0.9f, 0.9f},	DirectX::SimpleMath::Vector2{0.f,0.f} });

		//012
		//023
		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);
		indices.push_back(0);
		indices.push_back(2);
		indices.push_back(3);


		_planeShape = DirectX::GeometricPrimitive::CreateCustom(_DXStorage->_deviceContext, vertices, indices);

	}

	void DebugRenderer::CreateSystemVertexShaders()
	{
		_primitiveVS = std::make_unique<SystemVertexShader>(L"../Builds/x64/debug/PrimitiveVS.cso", LayoutDefine::GetWireframePrimitiveLayout(),
			LowDX11Storage::GetInstance()->_wireframeState, D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		_primitivePS = std::make_unique<SystemPixelShader>(L"../Builds/x64/debug/PrimitivePS.cso");
	}


	void DebugRenderer::CreateDepthWriteOffDSS()
	{
		//Write Mode: Depth를 끄고 출력한다.
		D3D11_DEPTH_STENCIL_DESC tDepthWriteOffDesc = CD3D11_DEPTH_STENCIL_DESC{ D3D11_DEFAULT };
		tDepthWriteOffDesc.DepthEnable = TRUE;
		tDepthWriteOffDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		tDepthWriteOffDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

		HR(_DXStorage->_device->CreateDepthStencilState(&tDepthWriteOffDesc, &_depthWriteOffDSS));
	}

	
	







}