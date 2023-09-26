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

	class Axis
	{
	public:
		Axis();

	public:
		void Draw();

	private:
		LowDX11Storage* _DXStorage;

		ID3D11Buffer* VB;
		ID3D11Buffer* IB;
	};
}