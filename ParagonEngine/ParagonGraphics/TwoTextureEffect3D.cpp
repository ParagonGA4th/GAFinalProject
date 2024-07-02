#include "TwoTextureEffect3D.h"
#include "VisualEffectRenderer.h"
namespace Pg::Graphics
{
	TwoTextureEffect3D::TwoTextureEffect3D(ID3D11Device* device, VERenderingSet* veSet) : BaseCustomEffect(veSet)
	{

	}

	void TwoTextureEffect3D::Apply(ID3D11DeviceContext* deviceContext)
	{

	}

	void TwoTextureEffect3D::GetVertexShaderBytecode(void const** pShaderByteCode, size_t* pByteCodeLength)
	{
		assert(_veSet->_veGraphicsSet->_customRenderVertexShader != nullptr);
		_veSet->_veGraphicsSet->_customRenderVertexShader->GetVertexShaderBytecode(pShaderByteCode, pByteCodeLength);
	}

	void XM_CALLCONV TwoTextureEffect3D::SetWorld(DirectX::FXMMATRIX value)
	{

	}

	void XM_CALLCONV TwoTextureEffect3D::SetView(DirectX::FXMMATRIX value)
	{

	}

	void XM_CALLCONV TwoTextureEffect3D::SetProjection(DirectX::FXMMATRIX value)
	{

	}

	void XM_CALLCONV TwoTextureEffect3D::SetMatrices(DirectX::FXMMATRIX world, DirectX::CXMMATRIX view, DirectX::CXMMATRIX projection)
	{

	}

	void TwoTextureEffect3D::SetTexture(unsigned int index, ID3D11ShaderResourceView*& value)
	{

	}

}