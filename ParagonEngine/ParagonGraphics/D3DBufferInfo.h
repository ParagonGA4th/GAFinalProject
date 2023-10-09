#pragma once
#include <d3d11.h>
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
			_vertexBuffer(nullptr), _indexBuffer(nullptr), _indexCount(0) {}
		
		//Vertex Buffer
		ID3D11Buffer* _vertexBuffer;
		
		//Index Buffer
		ID3D11Buffer* _indexBuffer;

		unsigned int _indexCount;

		//Vertex Starting Points Per Mesh (N번째 인덱스부터 Draw)
		std::vector<unsigned int> _vertexOffsetVector;

		//Index Starting Points Per Mesh (N번째 인덱스부터 Draw)
		std::vector<unsigned int> _indexOffsetVector;
	};
}
