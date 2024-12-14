#pragma once
#include <dxtk/Effects.h>
#include "BaseCustomEffect.h"

/// DXTK 3D Rendering�� ����, (2D Plane ����)
/// �ؽ��� �� �� ���� ��� Ȱ�� (Custom)
//https://github.com/microsoft/DirectXTK/wiki/Authoring-an-Effect
//https://github.com/microsoft/DirectXTK/wiki/IEffectMatrices

namespace Pg::Graphics
{
	class OneTextureEffect3D : public DirectX::IEffect, public DirectX::IEffectMatrices, public BaseCustomEffect
	{
	public:
		explicit OneTextureEffect3D(ID3D11Device* device, VERenderingSet* veSet);

		virtual void Apply(
			ID3D11DeviceContext* deviceContext) override;

		virtual void GetVertexShaderBytecode(
			void const** pShaderByteCode,
			size_t* pByteCodeLength) override;

		void XM_CALLCONV SetWorld(DirectX::FXMMATRIX value) override;
		void XM_CALLCONV SetView(DirectX::FXMMATRIX value) override;
		void XM_CALLCONV SetProjection(DirectX::FXMMATRIX value) override;
		void XM_CALLCONV SetMatrices(DirectX::FXMMATRIX world, DirectX::CXMMATRIX view, DirectX::CXMMATRIX projection) override;

		virtual void SetTexture(unsigned int index, ID3D11ShaderResourceView*& value) override;
		virtual unsigned int GetTextureCount() override { return 1; };
	};
}

