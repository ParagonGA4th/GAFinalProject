//#include "DemoGeometryGenerator.h"
////#include "D3DBufferInfo.h"
//#include "LayoutDefine.h"
//#include "LowDX11Storage.h"
//
//namespace Pg::Graphics::Helper
//{
//	
//	void DemoGeometryGenerator::CreateBoxBuffer(D3DBufferInfo* bufferInfo)
//	{
//		using DirectX::XMFLOAT3;
//		using DirectX::XMFLOAT4;
//		//
//		// Create the vertices.
//		//
//		
//		//Width/Height/Depth는 하드코딩.
//		float width = 3.0f;
//		float height = 3.0f;
//		float depth = 3.0f;
//
//		LayoutDefine::Vin1stStatic v[24];
//
//		float w2 = 0.5f * width;
//		float h2 = 0.5f * height;
//		float d2 = 0.5f * depth;
//
//		// Fill in the front face vertex data.
//		v[0] = LayoutDefine::Vin1stStatic(XMFLOAT3(-w2, -h2, -d2));
//		v[1] = LayoutDefine::Vin1stStatic(XMFLOAT3(-w2, +h2, -d2));
//		v[2] = LayoutDefine::Vin1stStatic(XMFLOAT3(+w2, +h2, -d2));
//		v[3] = LayoutDefine::Vin1stStatic(XMFLOAT3(+w2, -h2, -d2));
//
//		// Fill in the back face vertex data.
//		v[4] = LayoutDefine::Vin1stStatic(XMFLOAT3(-w2, -h2, +d2));
//		v[5] = LayoutDefine::Vin1stStatic(XMFLOAT3(+w2, -h2, +d2));
//		v[6] = LayoutDefine::Vin1stStatic(XMFLOAT3(+w2, +h2, +d2));
//		v[7] = LayoutDefine::Vin1stStatic(XMFLOAT3(-w2, +h2, +d2));
//
//		// Fill in the top face vertex data.
//		v[8] = LayoutDefine::Vin1stStatic(XMFLOAT3(-w2, +h2, -d2));
//		v[9] = LayoutDefine::Vin1stStatic(XMFLOAT3(-w2, +h2, +d2));
//		v[10] = LayoutDefine::Vin1stStatic(XMFLOAT3(+w2, +h2, +d2));
//		v[11] = LayoutDefine::Vin1stStatic(XMFLOAT3(+w2, +h2, -d2));
//
//		// Fill in the bottom face vertex data.
//		v[12] = LayoutDefine::Vin1stStatic(XMFLOAT3(-w2, -h2, -d2));
//		v[13] = LayoutDefine::Vin1stStatic(XMFLOAT3(+w2, -h2, -d2));
//		v[14] = LayoutDefine::Vin1stStatic(XMFLOAT3(+w2, -h2, +d2));
//		v[15] = LayoutDefine::Vin1stStatic(XMFLOAT3(-w2, -h2, +d2));
//
//		// Fill in the left face vertex data.
//		v[16] = LayoutDefine::Vin1stStatic(XMFLOAT3(-w2, -h2, +d2));
//		v[17] = LayoutDefine::Vin1stStatic(XMFLOAT3(-w2, +h2, +d2));
//		v[18] = LayoutDefine::Vin1stStatic(XMFLOAT3(-w2, +h2, -d2));
//		v[19] = LayoutDefine::Vin1stStatic(XMFLOAT3(-w2, -h2, -d2));
//
//		// Fill in the right face vertex data.
//		v[20] = LayoutDefine::Vin1stStatic(XMFLOAT3(+w2, -h2, -d2));
//		v[21] = LayoutDefine::Vin1stStatic(XMFLOAT3(+w2, +h2, -d2));
//		v[22] = LayoutDefine::Vin1stStatic(XMFLOAT3(+w2, +h2, +d2));
//		v[23] = LayoutDefine::Vin1stStatic(XMFLOAT3(+w2, -h2, +d2));
//
//		auto tDXStorage = LowDX11Storage::GetInstance();
//
//		D3D11_BUFFER_DESC vbd;
//		vbd.Usage = D3D11_USAGE_IMMUTABLE;
//		vbd.ByteWidth = sizeof(LayoutDefine::Vin1stStatic) * _countof(v);
//		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//		vbd.CPUAccessFlags = 0;
//		vbd.MiscFlags = 0;
//		vbd.StructureByteStride = 0;
//		D3D11_SUBRESOURCE_DATA vinitData;
//		vinitData.pSysMem = v;
//
//		// Create the vertex buffer.
//		HRESULT hr = tDXStorage->_device->CreateBuffer(&vbd, &vinitData, &(bufferInfo->_vertexBuffer));
//
//		//
//		// Create the indices.
//		//
//
//		UINT i[36];
//
//		// Fill in the front face index data
//		i[0] = 0; i[1] = 1; i[2] = 2;
//		i[3] = 0; i[4] = 2; i[5] = 3;
//
//		// Fill in the back face index data
//		i[6] = 4; i[7] = 5; i[8] = 6;
//		i[9] = 4; i[10] = 6; i[11] = 7;
//
//		// Fill in the top face index data
//		i[12] = 8; i[13] = 9; i[14] = 10;
//		i[15] = 8; i[16] = 10; i[17] = 11;
//
//		// Fill in the bottom face index data
//		i[18] = 12; i[19] = 13; i[20] = 14;
//		i[21] = 12; i[22] = 14; i[23] = 15;
//
//		// Fill in the left face index data
//		i[24] = 16; i[25] = 17; i[26] = 18;
//		i[27] = 16; i[28] = 18; i[29] = 19;
//
//		// Fill in the right face index data
//		i[30] = 20; i[31] = 21; i[32] = 22;
//		i[33] = 20; i[34] = 22; i[35] = 23;
//
//		D3D11_BUFFER_DESC ibd;
//		ibd.Usage = D3D11_USAGE_IMMUTABLE;
//		ibd.ByteWidth = sizeof(UINT) * _countof(i);
//		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
//		ibd.CPUAccessFlags = 0;
//		ibd.MiscFlags = 0;
//		ibd.StructureByteStride = 0;
//		D3D11_SUBRESOURCE_DATA iinitData;
//		iinitData.pSysMem = i;
//		hr = tDXStorage->_device->CreateBuffer(&ibd, &iinitData, &(bufferInfo->_indexBuffer));
//
//	}
//
//}