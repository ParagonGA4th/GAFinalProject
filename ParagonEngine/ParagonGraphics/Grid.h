#pragma once
#include "DX11Headers.h"

namespace Pg::Graphics
{
	class LowDX11Storage;

	struct Vertex
	{
		float3 Pos;
		float4 Color;
	};

	class Grid
	{
	public:
		Grid();

	public:
		void Draw();
		
		// VB, IB 생성 및 바인딩
		// InputLayout 

	private:
		LowDX11Storage* _DXStorage;

		ID3D11Buffer* VB;
		ID3D11Buffer* IB;
	};
}