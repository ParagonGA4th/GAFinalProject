#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>
/// <summary>
/// 단순히 VB/IB를 묶어놓은 클래스.
/// 하드웨어적으로 렌더에 필요한 버퍼들(VB/IB) 묶어놓음.
/// </summary>

namespace Pg::Graphics
{
	struct D3DBufferInfo
	{
	public:
		D3DBufferInfo() : 
			_vertexBuffer(nullptr), _indexBuffer(nullptr), 
			_meshCount(0), _totalVertexCount(0), _totalIndexCount(0) {}
		
		//Vertex Buffer
		ID3D11Buffer* _vertexBuffer;
		
		//Index Buffer
		ID3D11Buffer* _indexBuffer;

		//전체 Mesh의 개수.
		unsigned int _meshCount;

		unsigned int _totalVertexCount;
		unsigned int _totalIndexCount;

		//Vertex Starting Points Per Mesh (N번째 인덱스부터 Draw)
		std::vector<unsigned int> _vertexOffsetVector;

		//Index Starting Points Per Mesh (N번째 인덱스부터 Draw)
		std::vector<unsigned int> _indexOffsetVector;

		//각 Mesh의 인덱스에 대응해서, Material ID가 몇인지 확인!
		std::vector<unsigned int> _materialIDVector;

		////(*일단은 Static 기준) 각 Mesh의 인덱스에 대응해서, 적용해야 하는 World Matrix 목록.
		//std::vector<DirectX::XMMATRIX> _worldMatrixVector;
	};
}
