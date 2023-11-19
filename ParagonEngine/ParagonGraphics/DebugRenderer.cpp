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

		BeginLineRender(camData);
		LineRender();
		EndLineRender();
	}

	void DebugRenderer::InitGeometry()
	{
		//Box & Sphere 만들기.
		_boxShape = DirectX::GeometricPrimitive::CreateBox(_DXStorage->_deviceContext, DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
		
		size_t tTesellationFactor = 8;
		_sphereShape = DirectX::GeometricPrimitive::CreateSphere(_DXStorage->_deviceContext, 1.f, tTesellationFactor);

		//Capsule 만들기. -> TBA.

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
		//DebugSystem이랑 연동되면 실제 Collider 정보랑 연동되어 출력될 예정.
		using namespace DirectX;
		
		XMMATRIX tBoxWorld = XMMatrixScaling(2.f, 1.f, 2.f) * XMMatrixTranslation(0.f, 0.f, 0.f);
		XMMATRIX tSphereWorld = XMMatrixTranslation(1.f, 2.f, 0.f);

		XMMATRIX tView = MathHelper::PG2XM_MATRIX(camData->_viewMatrix);
		XMMATRIX tProj = MathHelper::PG2XM_MATRIX(camData->_projMatrix);

		//무조건 Wireframe으로 그린다.
		_boxShape->Draw(tBoxWorld, tView, tProj, Colors::Yellow, nullptr, true);
		_sphereShape->Draw(tSphereWorld, tView, tProj, Colors::Yellow, nullptr, true);
	}

	void DebugRenderer::EndGeoPrimitiveRender()
	{
		//
	}

	void DebugRenderer::BeginLineRender(Pg::Data::CameraData* camData)
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
		DirectX::XMFLOAT3 tSecond = { 0.f, 2.f, 3.f };
		DirectX::XMFLOAT4 tColor;
		DirectX::XMStoreFloat4(&tColor, DirectX::Colors::White);

		_primitiveBatch->DrawLine(
			DirectX::VertexPositionColor(tFirst, tColor),
			DirectX::VertexPositionColor(tSecond, tColor));
	}

	void DebugRenderer::EndLineRender()
	{
		_primitiveBatch->End();
	}



}