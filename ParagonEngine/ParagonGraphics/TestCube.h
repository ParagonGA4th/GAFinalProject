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
	class VertexShader;
	class PixelShader;

	class TestCube
	{
	public:
		TestCube(LowDX11Storage* storage);

	public:
		void Initialize();
		void Update(float time);
		void Draw();

	private:
		LowDX11Storage* _DXStorage;

		struct TestCubeVertex
		{
			float3 position;
			float3 color;
		};

		ID3D11Buffer* VB = nullptr;
		ID3D11Buffer* IB = nullptr;

		VertexShader* _vertexShader;
		PixelShader* _pixelShader;

	private:
		void BuildBuffers();
		void BindBuffers();

		void BindShaders();
		void UnbindShaders();

		void BindInputLayout();
		void UnbindInputLayout();

	public:
		void SetVertexShader(VertexShader* shader);
		void SetPixelShader(PixelShader* shader);

		VertexShader* GetVertexShader();
		PixelShader* GetPixelShader();

	public:
		BoxCBData _cbData;
	};
}
