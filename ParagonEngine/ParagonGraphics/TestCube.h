#pragma once

#include "DX11Headers.h"

struct BoxCBData
{
	float4x4 worldMatrix;
	float4x4 viewMatrix;
	float4x4 projectionMatrix;
	float4x4 viewProjMatrix;
};

namespace Pg::Graphics
{
	class LowDX11Storage;

	class TestCube
	{
	public:
		TestCube(LowDX11Storage* storage);

	public:
		void Update(float time);
		void Draw();

	private:

		struct MeshVertex
		{
			float3 position;
			float3 color;
		};

		LowDX11Storage* _DXStorage;

		ID3D11Buffer* VB = nullptr;
		ID3D11Buffer* IB = nullptr;

	public:
		BoxCBData _cbData;
	};
}
