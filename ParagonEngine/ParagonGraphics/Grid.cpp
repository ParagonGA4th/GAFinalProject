#include "Grid.h"

#include "LowDX11Storage.h"

Pg::Graphics::Grid::Grid()
	:VB(nullptr), IB(nullptr),
	_DXStorage()
{
	_DXStorage = Pg::Graphics::LowDX11Storage::GetInstance();

	Vertex vertices[100];
	for (int i = 0; i < 100; i++)
	{
		vertices[i].Pos = float3((float)(i % 10) - 5.0f, 0.0f, (float)(i / 10) - 5.0f);
		vertices[i].Color = float4(0.3f, 0.3f, 0.3f, 1.0f);	// 어두운 회색
	}

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * 100;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = vertices;

	// Create the vertex buffer.
	HRESULT hr = _DXStorage->_device->CreateBuffer(&vbd, &vinitData, &VB);

	UINT indices[40];
	for (int i = 0; i < 10; i++)
	{
		indices[i * 2] = i;
		indices[i * 2 + 1] = i + 90;
	}

	for (int i = 0; i < 10; i++)
	{
		indices[20 + (i * 2)] = i * 10;
		indices[20 + (i * 2) + 1] = i * 10 + 9;
	}

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * 40;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indices;
	hr = _DXStorage->_device->CreateBuffer(&ibd, &iinitData, &IB);
}

void Pg::Graphics::Grid::Draw()
{

}
