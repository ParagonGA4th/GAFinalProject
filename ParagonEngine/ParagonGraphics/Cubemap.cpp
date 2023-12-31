#include "Cubemap.h"

#include "DX11Headers.h"
#include "LowDX11Storage.h"

void Pg::Graphics::Cubemap::BuildBuffers()
{
	std::vector<CubemapVertex> VBData;
	std::vector<int> IBData;

	VBData.emplace_back(CubemapVertex{ float3 {-100.0f, 100.0f, -100.0f}, float2{0.0f, 0.0f} });
	VBData.emplace_back(CubemapVertex{ float3 {100.0f, 100.0f, -100.0f}, float2{0.0f, 0.0f} });
	VBData.emplace_back(CubemapVertex{ float3 {-100.0f, 100.0f, 100.0f}, float2{0.0f, 0.0f} });
	VBData.emplace_back(CubemapVertex{ float3 {100.0f, 100.0f, 100.0f}, float2{0.0f, 0.0f} });
	VBData.emplace_back(CubemapVertex{ float3 {100.0f, -100.0f, -100.0f}, float2{0.0f, 0.0f} });
	VBData.emplace_back(CubemapVertex{ float3 {-100.0f, -100.0f, -100.0f}, float2{0.0f, 0.0f} });
	VBData.emplace_back(CubemapVertex{ float3 {-100.0f, -100.0f, 100.0f}, float2{0.0f, 0.0f} });
	VBData.emplace_back(CubemapVertex{ float3 {100.0f, -100.0f, 100.0f}, float2{0.0f, 0.0f} });

	IBData.emplace_back(0);
	IBData.emplace_back(1);
	IBData.emplace_back(2);

	IBData.emplace_back(2);
	IBData.emplace_back(1);
	IBData.emplace_back(3);

	IBData.emplace_back(7);
	IBData.emplace_back(2);
	IBData.emplace_back(3);

	IBData.emplace_back(2);
	IBData.emplace_back(7);
	IBData.emplace_back(6);

	IBData.emplace_back(3);
	IBData.emplace_back(1);
	IBData.emplace_back(4);

	IBData.emplace_back(4);
	IBData.emplace_back(7);
	IBData.emplace_back(3);

	IBData.emplace_back(5);
	IBData.emplace_back(2);
	IBData.emplace_back(6);

	IBData.emplace_back(5);
	IBData.emplace_back(0);
	IBData.emplace_back(2);

	IBData.emplace_back(7);
	IBData.emplace_back(4);
	IBData.emplace_back(6);

	IBData.emplace_back(6);
	IBData.emplace_back(4);
	IBData.emplace_back(5);

	IBData.emplace_back(0);
	IBData.emplace_back(4);
	IBData.emplace_back(1);

	IBData.emplace_back(0);
	IBData.emplace_back(5);
	IBData.emplace_back(4);

	// Buffer Description
	D3D11_BUFFER_DESC VBDesc;
	VBDesc.Usage = D3D11_USAGE_DEFAULT;
	VBDesc.ByteWidth = VBData.size() * sizeof(CubemapVertex);
	VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VBDesc.CPUAccessFlags = 0;
	VBDesc.MiscFlags = 0;

	// Subresource Data
	D3D11_SUBRESOURCE_DATA VBInitData;
	VBInitData.pSysMem = &(VBData[0]);
	VBInitData.SysMemPitch = 0;
	VBInitData.SysMemSlicePitch = 0;

	// Create the vertex buffer.
	HRESULT hr = _DXStorage->_device->CreateBuffer(&VBDesc, &VBInitData, &VB);

	// Buffer Description
	D3D11_BUFFER_DESC IBDesc;
	IBDesc.Usage = D3D11_USAGE_DEFAULT;
	IBDesc.ByteWidth = IBData.size() * sizeof(int);
	IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IBDesc.CPUAccessFlags = 0;
	IBDesc.MiscFlags = 0;

	// Subresource Data
	D3D11_SUBRESOURCE_DATA IBInitData;
	IBInitData.pSysMem = &(IBData[0]);
	IBInitData.SysMemPitch = 0;
	IBInitData.SysMemSlicePitch = 0;

	// Create the Index buffer.
	hr = _DXStorage->_device->CreateBuffer(&IBDesc, &IBInitData, &IB);

}

void Pg::Graphics::Cubemap::BindBuffers()
{
	UINT stride = sizeof(CubemapVertex);
	UINT offset = 0;

	_DXStorage->_deviceContext->IASetVertexBuffers(0, 1, &VB, &stride, &offset);
	_DXStorage->_deviceContext->IASetIndexBuffer(IB, DXGI_FORMAT_R32_UINT, 0);
}

void Pg::Graphics::Cubemap::Render()
{
	BindInputLayout();
	BindShaders();

	BindBuffers();

	_DXStorage->_deviceContext->RSSetState(_DXStorage->_solidState);
	
	_DXStorage->_deviceContext->DrawIndexed(36, 0, 0);

	UnbindShaders();
	UnbindInputLayout();
}

void Pg::Graphics::Cubemap::Initialize()
{
	BuildBuffers();
	BindBuffers();

	SetTexture(L"../Resources/Textures/room.dds");
}

Pg::Graphics::Cubemap::Cubemap()
	:RenderableObject()
{

}

Pg::Graphics::Cubemap::~Cubemap()
{

}
