#pragma once
#include <d3d11.h>

/// <summary>
/// 단순히 VB/IB를 묶어놓은 클래스.
/// 하드웨어적으로 렌더에 필요한 버퍼들(VB/IB) 묶어놓음.
/// </summary>

namespace Pg::Graphics
{
	struct D3DBuffers
	{
	public:
		D3DBuffers() : 
			_vertexBuffer(nullptr), _indexBuffer(nullptr) {}
		
		ID3D11Buffer* _vertexBuffer;
		ID3D11Buffer* _indexBuffer;
	};
}
