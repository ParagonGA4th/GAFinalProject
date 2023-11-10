#include "Grid.h"
#include "LowDX11Storage.h"

Pg::Graphics::Grid::Grid()
	: RenderableObject(),
	_cbData()
{
	tGeometryGenerator.CreateGrid(20.f, 20.f, 10, 10, _MeshData);
}

Pg::Graphics::Grid::~Grid()
{

}

void Pg::Graphics::Grid::BuildBuffers()
{
	std::vector<Vertex> vertices;
	Vertex v;

	for (auto& e : _MeshData.Vertices)
	{
		v.Color = e._color;
		v.Pos = e._position;
		vertices.emplace_back(v);
	}

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * vertices.size();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = vertices.data();

	// Create the vertex buffer.
	HRESULT hr = _DXStorage->_device->CreateBuffer(&vbd, &vinitData, &VB);

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * _MeshData.Indices.size();
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = _MeshData.Indices.data();
	hr = _DXStorage->_device->CreateBuffer(&ibd, &iinitData, &IB);
}

void Pg::Graphics::Grid::BindBuffers()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	_DXStorage->_deviceContext->IASetVertexBuffers(0, 1, &VB, &stride, &offset);
	_DXStorage->_deviceContext->IASetIndexBuffer(IB, DXGI_FORMAT_R32_UINT, 0);
}

void Pg::Graphics::Grid::Render()
{
	BindInputLayout();
	BindShaders();

	BindBuffers();

	_DXStorage->_deviceContext->RSSetState(_DXStorage->_wireframeState);

	_DXStorage->_deviceContext->DrawIndexed(_MeshData.Indices.size(), 0, 0);

	UnbindShaders();
	UnbindInputLayout();
}

void Pg::Graphics::Grid::BindInputLayout()
{
	_DXStorage->_deviceContext->IASetInputLayout(_vertexShader->_inputLayout);
	_DXStorage->_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
}

void Pg::Graphics::Grid::SetGridSize(float width, float depth, UINT m, UINT n)
{
	tGeometryGenerator.CreateGrid(width, depth, m, n, _MeshData);
	BuildBuffers();
}

