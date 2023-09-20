#include "TestCube.h"

#include "DX11Headers.h"
#include "LowDX11Storage.h"

Pg::Graphics::TestCube::TestCube(LowDX11Storage* storage)
	:_DXStorage(storage)
{
	struct MeshVertex
	{
		float3 position;
		float3 color;
	};

	std::vector<MeshVertex> VBData;
	std::vector<int> IBData;

	VBData.emplace_back(MeshVertex{ float3 {-1.0f, 1.0f, -1.0f}, float3{1.0f, 0.0f, 0.0f} });
	VBData.emplace_back(MeshVertex{ float3 {1.0f, 1.0f, -1.0f}, float3{0.0f, 1.0f, 0.0f} });
	VBData.emplace_back(MeshVertex{ float3 {-1.0f, 1.0f, 1.0f}, float3{0.0f, 0.0f, 1.0f} });
	VBData.emplace_back(MeshVertex{ float3 {1.0f, 1.0f, 1.0f}, float3{1.0f, 1.0f, 0.0f} });
	VBData.emplace_back(MeshVertex{ float3 {1.0f, -1.0f, -1.0f}, float3{0.0f, 1.0f, 1.0f} });
	VBData.emplace_back(MeshVertex{ float3 {-1.0f, -1.0f, -1.0f}, float3{1.0f, 0.0f, 1.0f} });
	VBData.emplace_back(MeshVertex{ float3 {-1.0f, -1.0f, 1.0f}, float3{1.0f, 1.0f, 1.0f} });
	VBData.emplace_back(MeshVertex{ float3 {1.0f, -1.0f, 1.0f}, float3{0.0f, 0.0f, 0.0f} });

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
	VBDesc.ByteWidth = VBData.size() * sizeof(MeshVertex);
	VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VBDesc.CPUAccessFlags = 0;
	VBDesc.MiscFlags = 0;

	// Subresource Data
	D3D11_SUBRESOURCE_DATA VBInitData;
	VBInitData.pSysMem = &(VBData[0]);
	VBInitData.SysMemPitch = 0;
	VBInitData.SysMemSlicePitch = 0;

	ID3D11Buffer* VB;

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

	ID3D11Buffer* IB;

	// Create the Index buffer.
	hr = _DXStorage->_device->CreateBuffer(&IBDesc, &IBInitData, &IB);

	UINT stride = sizeof(MeshVertex);
	UINT offset = 0;

	_DXStorage->_deviceContext->IASetVertexBuffers(0, 1, &VB, &stride, &offset);
	_DXStorage->_deviceContext->IASetIndexBuffer(IB, DXGI_FORMAT_R32_UINT, 0);

	/// InputLayout ¹ÙÀÎµù

	_DXStorage->_deviceContext->IASetInputLayout(_DXStorage->_vertexShader->_inputLayout);
	_DXStorage->_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Pg::Graphics::TestCube::Draw()
{
	_DXStorage->_deviceContext->DrawIndexed(36, 0, 0);
}

void Pg::Graphics::TestCube::Update(float time)
{

}
