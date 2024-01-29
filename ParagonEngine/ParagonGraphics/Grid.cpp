#include "Grid.h"
#include "LayoutDefine.h"
#include "LowDX11Storage.h"
#include "GeometryGenerator.h"

namespace Pg::Graphics
{
	Grid::Grid() : WireframeRenderObject()
	{
		//
	}

	Grid::~Grid()
	{
		//
	}

	void Grid::BuildBuffers()
	{
		//Creating a Grid via Geometry Generator
		GeometryGenerator::MeshData_PosColor _MeshData;

		GeometryGenerator tGeometryGenerator;

		float width = 300.f;
		float depth = 300.f;
		UINT m = 70;
		UINT n = 70;
		tGeometryGenerator.CreateGrid(width, depth, m, n, _MeshData);

		//Index Size ±â·Ï.
		_indexSize = _MeshData.Indices.size();

		//Creating VB & IB.
		std::vector<LayoutDefine::VinWireframePrimitive> vertices;

		for (auto& e : _MeshData.Vertices)
		{
			vertices.emplace_back(LayoutDefine::VinWireframePrimitive(e._position, e._color));
		}

		D3D11_BUFFER_DESC vbd;
		vbd.Usage = D3D11_USAGE_IMMUTABLE;
		vbd.ByteWidth = sizeof(LayoutDefine::VinWireframePrimitive) * vertices.size();
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbd.CPUAccessFlags = 0;
		vbd.MiscFlags = 0;
		vbd.StructureByteStride = 0;
		D3D11_SUBRESOURCE_DATA vinitData;
		vinitData.pSysMem = vertices.data();

		// Create the vertex buffer.
		HRESULT hr = _DXStorage->_device->CreateBuffer(&vbd, &vinitData, &_VB);

		D3D11_BUFFER_DESC ibd;
		ibd.Usage = D3D11_USAGE_IMMUTABLE;
		ibd.ByteWidth = sizeof(UINT) * _MeshData.Indices.size();
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags = 0;
		ibd.MiscFlags = 0;
		ibd.StructureByteStride = 0;
		D3D11_SUBRESOURCE_DATA iinitData;
		iinitData.pSysMem = _MeshData.Indices.data();
		hr = _DXStorage->_device->CreateBuffer(&ibd, &iinitData, &_IB);
	}
}


