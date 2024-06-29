#pragma once
#include <dxtk/Effects.h>

/// DXTK 3D Rendering을 위해, (2D Plane 렌더)
/// 텍스쳐 한 개 쓰는 경우 활용 (Custom)
//https://github.com/microsoft/DirectXTK/wiki/Authoring-an-Effect
//https://github.com/microsoft/DirectXTK/wiki/IEffectMatrices

namespace Pg::Graphics
{
	class OneTextureEffect3D : public DirectX::IEffect, public DirectX::IEffectMatrices
	{
	public:
		explicit OneTextureEffect3D(ID3D11Device* device);

		virtual void Apply(
			ID3D11DeviceContext* deviceContext) override;

		virtual void GetVertexShaderBytecode(
			void const** pShaderByteCode,
			size_t* pByteCodeLength) override;

		void XM_CALLCONV SetWorld(DirectX::FXMMATRIX value) override;
		void XM_CALLCONV SetView(DirectX::FXMMATRIX value) override;
		void XM_CALLCONV SetProjection(DirectX::FXMMATRIX value) override;
		void XM_CALLCONV SetMatrices(DirectX::FXMMATRIX world, DirectX::CXMMATRIX view, DirectX::CXMMATRIX projection) override;

		void SetTexture(unsigned int index, ID3D11ShaderResourceView* value);
	};
}

