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
		LowDX11Storage* _DXStorage;

	public:
		BoxCBData _cbData;
	};
}
