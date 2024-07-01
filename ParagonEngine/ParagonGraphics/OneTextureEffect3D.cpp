#include "OneTextureEffect3D.h"
#include "VisualEffectRenderer.h"
#include "RenderVertexShader.h"

namespace Pg::Graphics
{
	OneTextureEffect3D::OneTextureEffect3D(ID3D11Device* device, VERenderingSet* veSet) : BaseCustomEffect(veSet)
	{

	}

	void OneTextureEffect3D::Apply(ID3D11DeviceContext* deviceContext)
	{

	}

	void OneTextureEffect3D::GetVertexShaderBytecode(void const** pShaderByteCode, size_t* pByteCodeLength)
	{
		assert(_veSet->_veGraphicsSet->_customRenderVertexShader != nullptr);
		_veSet->_veGraphicsSet->_customRenderVertexShader->GetVertexShaderBytecode(pShaderByteCode, pByteCodeLength);
	}

	void XM_CALLCONV OneTextureEffect3D::SetWorld(DirectX::FXMMATRIX value)
	{

	}

	void XM_CALLCONV OneTextureEffect3D::SetView(DirectX::FXMMATRIX value)
	{

	}

	void XM_CALLCONV OneTextureEffect3D::SetProjection(DirectX::FXMMATRIX value)
	{

	}

	void XM_CALLCONV OneTextureEffect3D::SetMatrices(DirectX::FXMMATRIX world, DirectX::CXMMATRIX view, DirectX::CXMMATRIX projection)
	{

	}

	void OneTextureEffect3D::SetTexture(unsigned int index, ID3D11ShaderResourceView*& value)
	{

	}

}
