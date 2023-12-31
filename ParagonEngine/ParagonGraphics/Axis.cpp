#include "Axis.h"

#include "LowDX11Storage.h"

Pg::Graphics::Axis::Axis()
	:RenderableObject(),
	_cbData()
{

}

Pg::Graphics::Axis::~Axis()
{

}

void Pg::Graphics::Axis::BuildBuffers()
{
	Vertex vertices[] =
	{
		{ float3(0.0f, 0.0f, 0.0f), float4(1.0f, 0.0f, 0.0f, 1.0f) },	// x�� (����)
		{ float3(10.0f, 0.0f, 0.0f), float4(1.0f, 0.0f, 0.0f, 1.0f) },

		{ float3(0.0f, 0.0f, 0.0f), float4(0.0f, 1.0f, 0.0f, 1.0f) },	// y�� (�ʷ�)
		{ float3(0.0f, 10.0f, 0.0f), float4(0.0f, 1.0f, 0.0f, 1.0f) },

		{ float3(0.0f, 0.0f, 0.0f), float4(0.0f, 0.0f, 1.0f, 1.0f)	},	// z�� (�Ķ�)
		{ float3(0.0f, 0.0f, 10.0f), float4(0.0f, 0.0f, 1.0f, 1.0f) }
	};

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * 6;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = vertices;

	// Create the vertex buffer.
	HRESULT hr = _DXStorage->_device->CreateBuffer(&vbd, &vinitData, &VB);

	UINT indices[] = {
		// x��
		0, 1,

		// y��
		2, 3,

		// z��
		4, 5,
	};

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * 6;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indices;
	hr = _DXStorage->_device->CreateBuffer(&ibd, &iinitData, &IB);
}

void Pg::Graphics::Axis::BindBuffers()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	_DXStorage->_deviceContext->IASetVertexBuffers(0, 1, &VB, &stride, &offset);
	_DXStorage->_deviceContext->IASetIndexBuffer(IB, DXGI_FORMAT_R32_UINT, 0);
}

void Pg::Graphics::Axis::Render()
{
	BindInputLayout();
	BindShaders();

	BindBuffers();

	_DXStorage->_deviceContext->DrawIndexed(6, 0, 0);

	UnbindShaders();
	UnbindInputLayout();
}

void Pg::Graphics::Axis::BindInputLayout()
{
	_DXStorage->_deviceContext->IASetInputLayout(_vertexShader->_inputLayout);
	_DXStorage->_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
}
