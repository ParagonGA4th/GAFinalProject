#include "Axis.h"
#include "LayoutDefine.h"
#include "LowDX11Storage.h"

namespace Pg::Graphics
{
	Axis::Axis() : WireframeRenderObject()
	{
		_indexSize = 6;
	}

	Axis::~Axis()
	{

	}

	void Axis::BuildBuffers()
	{
		LayoutDefine::VinWireframePrimitive vertices[] =
		{
			//일괄적인 Wireframe의 역수 (0.35의 역수)
			{ float3(0.0f, 0.0f, 0.0f), float4(2.85714f, 0.0f, 0.0f, 1.0f) },	// x축 (빨강)
			{ float3(30.f, 0.0f, 0.0f), float4(2.85714f, 0.0f, 0.0f, 1.0f) },

			{ float3(0.0f, 0.0f, 0.0f), float4(0.0f, 2.85714f, 0.0f, 1.0f) },	// y축 (초록)
			{ float3(0.0f, 30.f, 0.0f), float4(0.0f, 2.85714f, 0.0f, 1.0f) },

			{ float3(0.0f, 0.0f, 0.0f), float4(0.0f, 0.0f, 2.85714f, 1.0f) },	// z축 (파랑)
			{ float3(0.0f, 0.0f, 30.f), float4(0.0f, 0.0f, 2.85714f, 1.0f) }
		};

		D3D11_BUFFER_DESC vbd;
		vbd.Usage = D3D11_USAGE_IMMUTABLE;
		vbd.ByteWidth = sizeof(LayoutDefine::VinWireframePrimitive) * 6;
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbd.CPUAccessFlags = 0;
		vbd.MiscFlags = 0;
		vbd.StructureByteStride = 0;
		D3D11_SUBRESOURCE_DATA vinitData;
		vinitData.pSysMem = vertices;

		// Create the vertex buffer.
		HRESULT hr = _DXStorage->_device->CreateBuffer(&vbd, &vinitData, &_VB);

		UINT indices[] = {
			// x축
			0, 1,

			// y축
			2, 3,

			// z축
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
		hr = _DXStorage->_device->CreateBuffer(&ibd, &iinitData, &_IB);
	}
}