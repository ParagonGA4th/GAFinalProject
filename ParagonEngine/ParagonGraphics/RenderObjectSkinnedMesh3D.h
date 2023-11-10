#pragma once
#include "RenderObject3D.h"
#include "ConstantBufferDefine.h"

#include <d3d11.h>

/// <summary>
/// SkinnedMeshRenderer 컴포넌트와 1대1 대응하는 렌더오브젝트.
/// </summary>

namespace Pg::Data
{
	class SkinnedMeshRenderer;
}

namespace Pg::Graphics
{
	class Asset3DModelData;
}

namespace Pg::Graphics
{
	class RenderObjectSkinnedMesh3D : public RenderObject3D
	{
	public:
		RenderObjectSkinnedMesh3D(Pg::Data::BaseRenderer* baseRenderer);
		virtual ~RenderObjectSkinnedMesh3D();

		//virtual void Render(Pg::Data::CameraData* camData) override;

		virtual void BindBuffers() override;
		virtual void UpdateConstantBuffers(Pg::Data::CameraData* camData) override;
		virtual void BindConstantBuffers() override;
		virtual void UnbindConstantBuffers() override;



	private:
		Asset3DModelData* _modelData = nullptr;

	private:
		void UpdateConstantBuffer(Pg::Data::CameraData* camData);
	private:
		void CreateSamplerState();
		void CreateVertexPixelShader();
		void CreateRasterizerState();
		void CreateConstantBuffer();

	private:

		ConstantBufferDefine::cbPerObjectBase* _constantBufferStruct;

		//렌더러와 연결되면서 지워질 것이다!
		ID3D11Device* _device = nullptr;
		ID3D11DeviceContext* _devCon = nullptr;

		ID3D11VertexShader* _vertexShader = nullptr;
		ID3D11PixelShader* _pixelShader = nullptr;

		D3D11_SUBRESOURCE_DATA _cbufferSubresourceData;
		ID3D11Buffer* _constantBuffer = nullptr;
		ID3D11SamplerState* _samplerState = nullptr;
		ID3D11RasterizerState* _rasterizerState = nullptr;
	};

}

