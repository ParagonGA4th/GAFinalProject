#include "Forward3DRenderer.h"

#include "DX11Headers.h"
#include "LowDX11Logic.h"
#include "LowDX11Storage.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "MathHelper.h"

#include "../ParagonData/CameraData.h"

#include "Grid.h"
#include "Axis.h"
#include "Cubemap.h"



namespace Pg::Graphics
{
	Grid* grid;
	Axis* axis;
	Cubemap* cubemap;

	Pg::Graphics::Forward3DRenderer::Forward3DRenderer()
		: _DXStorage(LowDX11Storage::GetInstance()), _DXLogic(LowDX11Logic::GetInstance())
	{

	}

	void Pg::Graphics::Forward3DRenderer::Initialize()
	{
		D3D11_INPUT_ELEMENT_DESC HelperDesc[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		VertexShader* helperVS = new VertexShader(Pg::Data::Enums::eAssetDefine::_RENDERSHADER, "../Builds/x64/debug/VertexShader.cso", HelperDesc);
		PixelShader* helperPS = new PixelShader(Pg::Data::Enums::eAssetDefine::_RENDERSHADER, "../Builds/x64/debug/PixelShader.cso");

		// Grid
		grid = new Grid();
		grid->Initialize();

		// Axis
		axis = new Axis();
		axis->Initialize();

		helperVS->AssignConstantBuffer(&(grid->_cbData));

		grid->AssignVertexShader(helperVS);
		grid->AssignPixelShader(helperPS);

		axis->AssignVertexShader(helperVS);
		axis->AssignPixelShader(helperPS);

		// Cubemap
		D3D11_INPUT_ELEMENT_DESC CubemapvertexDesc[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		VertexShader* CubemapVS = new VertexShader(Pg::Data::Enums::eAssetDefine::_RENDERSHADER, "../Builds/x64/debug/CubemapVS.cso", CubemapvertexDesc);
		PixelShader* CubemapPS = new PixelShader(Pg::Data::Enums::eAssetDefine::_RENDERSHADER, "../Builds/x64/debug/CubemapPS.cso");

		cubemap = new Cubemap();
		cubemap->Initialize();

		CubemapVS->AssignConstantBuffer(&(cubemap->_cbData));
		cubemap->AssignVertexShader(CubemapVS);
		cubemap->AssignPixelShader(CubemapPS);
	}

	void Pg::Graphics::Forward3DRenderer::BeginRender()
	{
		_DXStorage->_deviceContext->OMSetRenderTargets(1, &(_DXStorage->_mainRTV), _DXStorage->_depthStencilView);
	}

	void Pg::Graphics::Forward3DRenderer::Render(Pg::Data::CameraData camData)
	{
		BeginRender();

		///
		DirectX::XMFLOAT4X4 tWorldTM;
		DirectX::XMMATRIX tWorldTMMat = DirectX::XMMatrixIdentity();

		DirectX::XMFLOAT4X4 tViewTM = Helper::MathHelper::PG2XM_FLOAT4X4(camData._viewMatrix);
		DirectX::XMMATRIX tViewTMMat = DirectX::XMLoadFloat4x4(&tViewTM);

		DirectX::XMFLOAT4X4 tProjTM = Helper::MathHelper::PG2XM_FLOAT4X4(camData._projMatrix);
		DirectX::XMMATRIX tProjTMMat = DirectX::XMLoadFloat4x4(&tProjTM);

		DirectX::XMFLOAT3 tCameraPosition = Helper::MathHelper::PG2XM_FLOAT3(camData._position);
		DirectX::XMVECTOR tCameraPositionVec = DirectX::XMLoadFloat3(&tCameraPosition);
		DirectX::XMMATRIX tCameraPositionMat = DirectX::XMMatrixTranslationFromVector(tCameraPositionVec);

		float tCamDistance = 0.0f;
		DirectX::XMStoreFloat(&tCamDistance, DirectX::XMVector3Length(tCameraPositionVec));

		// Cubemap
		DirectX::XMStoreFloat4x4(&(cubemap->_cbData.worldMatrix), DirectX::XMMatrixMultiply(tWorldTMMat, tCameraPositionMat));
		DirectX::XMStoreFloat4x4(&(cubemap->_cbData.viewProjMatrix), DirectX::XMMatrixMultiply(tViewTMMat, tProjTMMat));

		// Grid
		DirectX::XMStoreFloat4x4(&(grid->_cbData.worldMatrix), tWorldTMMat);
		DirectX::XMStoreFloat4x4(&(grid->_cbData.viewProjMatrix), DirectX::XMMatrixMultiply(tViewTMMat, tProjTMMat));
		grid->SetGridSize(20.0f, 20.0f, 10, 10);

		// Axis
		DirectX::XMStoreFloat4x4(&(axis->_cbData.worldMatrix), tWorldTMMat);
		DirectX::XMStoreFloat4x4(&(axis->_cbData.viewProjMatrix), DirectX::XMMatrixMultiply(tViewTMMat, tProjTMMat));

		// ·»´õ
		cubemap->Render();
		grid->Render();
		axis->Render();
	}
}


