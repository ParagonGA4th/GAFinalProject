#pragma once

#include "DX11Headers.h"

namespace Pg::Graphics
{
	struct CBDataBase
	{
		float4x4 worldMatrix;
		float4x4 viewMatrix;
		float4x4 projectionMatrix;
		float4x4 viewProjMatrix;
	};

	struct Vertex
	{
		float3 Pos;
		float4 Color;
	};
}

namespace Pg::Graphics
{
	class LowDX11Storage;
	class VertexShader;
	class PixelShader;

	class RenderableObject
	{
	public:
		RenderableObject();
		virtual ~RenderableObject();

	public:
		void Initialize();
		void Update(float time);

		virtual void Draw();

	protected:
		LowDX11Storage* _DXStorage;

		ID3D11Buffer* VB;
		ID3D11Buffer* IB;
		VertexShader* _vertexShader;
		PixelShader* _pixelShader;

	protected:
		virtual void BuildBuffers();
		virtual void BindBuffers();

		void BindShaders();
		void UnbindShaders();

		virtual void BindInputLayout();
		void UnbindInputLayout();

	public:
		void AssignVertexShader(VertexShader* shader);
		void AssignPixelShader(PixelShader* shader);

		VertexShader* GetVertexShader();
		PixelShader* GetPixelShader();
	};

}