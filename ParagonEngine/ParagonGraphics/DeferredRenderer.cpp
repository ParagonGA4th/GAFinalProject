#include "DeferredRenderer.h"

#include "TestCube.h"
#include "VertexShader.h"
#include "PixelShader.h"

#include "GBuffer.h"
#include "LowDX11Storage.h"

#include "LayoutDefine.h"

#include "../ParagonData/GameObject.h"
#include "../ParagonData/Transform.h"
#include "../ParagonData/CameraData.h"
#include "RenderObjectBase.h"

#include "RenderObject3D.h"

#include "ConstantBufferDefine.h"

#include "GeometryGenerator.h"

Pg::Graphics::DeferredRenderer::DeferredRenderer()
{
	cube = new TestCube();
}

void Pg::Graphics::DeferredRenderer::Initialize()
{
	_DXStorage = LowDX11Storage::GetInstance();

	cube->Initialize();

	_gBuffers.emplace_back(new GBuffer(DXGI_FORMAT_R32G32B32A32_TYPELESS, DXGI_FORMAT_R32G32B32A32_FLOAT));
	_gBuffers.emplace_back(new GBuffer(DXGI_FORMAT_R32G32B32A32_TYPELESS, DXGI_FORMAT_R32G32B32A32_FLOAT));
	_gBuffers.emplace_back(new GBuffer(DXGI_FORMAT_R32G32B32A32_TYPELESS, DXGI_FORMAT_R32G32B32A32_FLOAT));
	_gBuffers.emplace_back(new GBuffer(DXGI_FORMAT_R32G32B32A32_TYPELESS, DXGI_FORMAT_R32G32B32A32_FLOAT));
	_gBuffers.emplace_back(new GBuffer(DXGI_FORMAT_R16G16B16A16_TYPELESS, DXGI_FORMAT_R16G16B16A16_FLOAT));

	for (auto& e : _gBuffers)
	{
		_RTVs.emplace_back(e->GetRTV());
	}

	for (auto& e : _gBuffers)
	{
		_SRVs.emplace_back(e->GetSRV());
	}

	for (int i = 0; i < _gBuffers.size(); ++i)
	{
		NullRTV.emplace_back(nullptr);
	}

	for (int i = 0; i < _gBuffers.size(); ++i)
	{
		NullSRV.emplace_back(nullptr);
	}

	// 1st Pass
	_firstVS = new VertexShader(L"../Builds/x64/debug/FirstStatic_VS.cso");
	_firstVS->_inputLayout = LayoutDefine::GetStatic1stLayout();
	_firstVS->AssignConstantBuffer(&cube->_cbData);
	cube->AssignVertexShader(_firstVS);

	_firstPS = new PixelShader(L"../Builds/x64/debug/FirstStage_PS.cso");

	// 2nd Pass
	_secondVS = new VertexShader(L"../Builds/x64/debug/SecondStage_VS.cso");
	_secondVS->_inputLayout = LayoutDefine::Get2ndLayout();
	_secondPS = new PixelShader(L"../Builds/x64/debug/SecondStage_PS.cso");
}

void Pg::Graphics::DeferredRenderer::BeginRender()
{
	// Set Depth Stencil State
	_DXStorage->_deviceContext->OMSetDepthStencilState(_DXStorage->_depthStencilState, 0);

	_DXStorage->_deviceContext->ClearDepthStencilView(_DXStorage->_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0.0f);
	ClearGBuffers();

	_DXStorage->_deviceContext->OMSetRenderTargets(1, &(_DXStorage->_mainRTV), _DXStorage->_depthStencilView);
}

void Pg::Graphics::DeferredRenderer::RenderFirstPass(Pg::Data::GameObject* object, Pg::Data::CameraData& camData)
{
	BindFirstPass();

	// 3D 오브젝트 렌더
	cube->Draw(object->_transform, camData);

	UnbindFirstPass();
}

void Pg::Graphics::DeferredRenderer::RenderSecondPass()
{
	BindSecondPass();

	_DXStorage->_deviceContext->DrawIndexed(6, 0, 0);

	//_DXStorage->_deviceContext->OMSetBlendState(nullptr, NULL, 0xffffffff);
	_DXStorage->_deviceContext->OMSetRenderTargets(1, &(_DXStorage->_mainRTV), _DXStorage->_depthStencilView);

	UnbindSecondPass();
}

void Pg::Graphics::DeferredRenderer::BindFirstPass()
{
	// Bind Shaders
	_firstVS->Bind();
	_firstPS->Bind();

	_DXStorage->_deviceContext->RSSetState(_DXStorage->_solidState);
	_DXStorage->_deviceContext->PSSetShaderResources(0, 1, &NullSRV[0]);

	// SetRenderTarget
	_DXStorage->_deviceContext->OMSetRenderTargets(_RTVs.size(), _RTVs.data(), _DXStorage->_depthStencilView);
}

void Pg::Graphics::DeferredRenderer::UnbindFirstPass()
{
	_firstVS->UnBind();
	_firstPS->UnBind();

	_DXStorage->_deviceContext->OMSetRenderTargets(_RTVs.size(), NullRTV.data(), _DXStorage->_depthStencilView);
}

void Pg::Graphics::DeferredRenderer::BindSecondPass()
{
	// Build Quad
	BuildFullscreenQuad();

	// Bind Shaders
	_secondVS->Bind();
	_secondPS->Bind();

	// Set Shader Resources to Sample
	_DXStorage->_deviceContext->VSSetShaderResources(0, _SRVs.size(), _SRVs.data());
	_DXStorage->_deviceContext->PSSetShaderResources(0, _SRVs.size(), _SRVs.data());

	// Set Sampler State
	_DXStorage->_deviceContext->PSSetSamplers(0, 1, &_DXStorage->_defaultSamplerState);

	_DXStorage->_deviceContext->ClearDepthStencilView(_DXStorage->_tempDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0.0f);

	// Render to Main Render Target
	_DXStorage->_deviceContext->OMSetRenderTargets(1, &_DXStorage->_mainRTV, _DXStorage->_tempDepthStencilView);
}

void Pg::Graphics::DeferredRenderer::UnbindSecondPass()
{
	_secondVS->UnBind();
	_secondPS->UnBind();

	_DXStorage->_deviceContext->VSSetShaderResources(0, _SRVs.size(), NullSRV.data());
	_DXStorage->_deviceContext->PSSetShaderResources(0, _SRVs.size(), NullSRV.data());

	_DXStorage->_deviceContext->OMSetRenderTargets(_RTVs.size(), NullRTV.data(), _DXStorage->_depthStencilView);
}

void Pg::Graphics::DeferredRenderer::BuildFullscreenQuad()
{
	GeometryGenerator* tGeometryGenerator = new GeometryGenerator();
	GeometryGenerator::MeshData_PosNormalTex tMeshData;
	tGeometryGenerator->GenerateFullscreenQuad(tMeshData);

	ID3D11Buffer* VB;
	ID3D11Buffer* IB;

	// Buffer Description
	D3D11_BUFFER_DESC VBDesc;
	VBDesc.Usage = D3D11_USAGE_DEFAULT;
	VBDesc.ByteWidth = tMeshData.Vertices.size() * sizeof(GeometryGenerator::MeshData_PosNormalTex);
	VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VBDesc.CPUAccessFlags = 0;
	VBDesc.MiscFlags = 0;

	// Subresource Data
	D3D11_SUBRESOURCE_DATA VBInitData;
	VBInitData.pSysMem = tMeshData.Vertices.data();
	VBInitData.SysMemPitch = 0;
	VBInitData.SysMemSlicePitch = 0;

	// Create the vertex buffer.
	HRESULT hr = _DXStorage->_device->CreateBuffer(&VBDesc, &VBInitData, &VB);

	// Buffer Description
	D3D11_BUFFER_DESC IBDesc;
	IBDesc.Usage = D3D11_USAGE_DEFAULT;
	IBDesc.ByteWidth = tMeshData.Indices.size() * sizeof(int);
	IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IBDesc.CPUAccessFlags = 0;
	IBDesc.MiscFlags = 0;

	// Subresource Data
	D3D11_SUBRESOURCE_DATA IBInitData;
	IBInitData.pSysMem = tMeshData.Indices.data();
	IBInitData.SysMemPitch = 0;
	IBInitData.SysMemSlicePitch = 0;

	// Create the Index buffer.
	hr = _DXStorage->_device->CreateBuffer(&IBDesc, &IBInitData, &IB);

	// Bind Buffers
	UINT stride = sizeof(GeometryGenerator::GeomVertex_PosNormalTex);
	UINT offset = 0;
	_DXStorage->_deviceContext->IASetVertexBuffers(0, 1, &VB, &stride, &offset);
	_DXStorage->_deviceContext->IASetIndexBuffer(IB, DXGI_FORMAT_R32_UINT, 0);
}

void Pg::Graphics::DeferredRenderer::ClearGBuffers()
{
	for (auto& e : _RTVs)
	{
		_DXStorage->_deviceContext->ClearRenderTargetView(e, _DXStorage->_backgroundColor);
	}
}