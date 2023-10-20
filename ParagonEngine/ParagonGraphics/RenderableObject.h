#pragma once

#include "DX11Headers.h"
#include <string>

namespace Pg::Graphics
{
	struct CBDataBase
	{
		DirectX::XMFLOAT4X4 worldMatrix;
		DirectX::XMFLOAT4X4 viewMatrix;
		DirectX::XMFLOAT4X4 projectionMatrix;
		DirectX::XMFLOAT4X4 viewProjMatrix;
	};

	struct Vertex
	{
		float3 Pos;
		float4 Color;
	};
}

namespace Pg::Data
{
	class Transform;
	class CameraData;
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
		virtual void Initialize();
		void Draw(float time);

		virtual void Draw(Pg::Data::Transform& transform, Pg::Data::CameraData& camData);
		virtual void Draw();

	protected:
		LowDX11Storage* _DXStorage;

		ID3D11Buffer* VB;
		ID3D11Buffer* IB;
		VertexShader* _vertexShader;
		PixelShader* _pixelShader;

		ID3D11SamplerState* _samplerState;

	protected:
		virtual void BuildBuffers();
		virtual void BindBuffers();

		void BindShaders();
		void UnbindShaders();

		virtual void BindInputLayout();
		void UnbindInputLayout();

		virtual void SetTexture(std::wstring filepath);
		ID3D11ShaderResourceView* SRV;

	public:
		void AssignVertexShader(VertexShader* shader);
		void AssignPixelShader(PixelShader* shader);

		VertexShader* GetVertexShader();
		PixelShader* GetPixelShader();
	};

}