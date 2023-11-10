#pragma once

#include "constantBuffer.h"

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
		void Render(float time);

		virtual void Render(Pg::Data::Transform& transform, Pg::Data::CameraData& camData);
		virtual void Render();

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

	public:
		// 상수 버퍼들을 저장하는 벡터
		std::vector< ConstantBufferBase* > _constantBuffers;

		// 상수 버퍼 데이터를 추가하는 함수
		template <typename T>
		void CreateConstantBuffer(T* cbData)
		{
			ConstantBufferBase* tCBuffer = new ConstantBuffer<T>(cbData);
			_constantBuffers.emplace_back(tCBuffer);
		}
	};

}