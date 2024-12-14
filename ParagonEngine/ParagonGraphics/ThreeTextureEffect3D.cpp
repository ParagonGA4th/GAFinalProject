#include "ThreeTextureEffect3D.h"
#include "VisualEffectRenderer.h"
namespace Pg::Graphics
{
	ThreeTextureEffect3D::ThreeTextureEffect3D(ID3D11Device* device, VERenderingSet* veSet) : BaseCustomEffect(veSet)
	{

	}

	void ThreeTextureEffect3D::Apply(ID3D11DeviceContext* deviceContext)
	{

	}

	void ThreeTextureEffect3D::GetVertexShaderBytecode(void const** pShaderByteCode, size_t* pByteCodeLength)
	{
		assert(_veSet->_veGraphicsSet->_customRenderVertexShader != nullptr);
		_veSet->_veGraphicsSet->_customRenderVertexShader->GetVertexShaderBytecode(pShaderByteCode, pByteCodeLength);
	}

	void XM_CALLCONV ThreeTextureEffect3D::SetWorld(DirectX::FXMMATRIX value)
	{

	}

	void XM_CALLCONV ThreeTextureEffect3D::SetView(DirectX::FXMMATRIX value)
	{

	}

	void XM_CALLCONV ThreeTextureEffect3D::SetProjection(DirectX::FXMMATRIX value)
	{

	}

	void XM_CALLCONV ThreeTextureEffect3D::SetMatrices(DirectX::FXMMATRIX world, DirectX::CXMMATRIX view, DirectX::CXMMATRIX projection)
	{

	}

	void ThreeTextureEffect3D::SetTexture(unsigned int index, ID3D11ShaderResourceView*& value)
	{

	}

}