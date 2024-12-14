#include "SpriteEffect2D.h"
#include "VisualEffectRenderer.h"
#include "LowDX11Storage.h"
namespace Pg::Graphics
{
	SpriteEffect2D::SpriteEffect2D(VERenderingSet* veSet) : _veSet(veSet), _DXStorage(LowDX11Storage::GetInstance())
	{

	}

	//void SpriteEffect2D::SetTexture(unsigned int index, ID3D11ShaderResourceView*& value)
	//{
	//	_DXStorage->_deviceContext->PSSetShaderResources(index, 1, &value);
	//}
	//
	//unsigned int SpriteEffect2D::GetTextureCount()
	//{
	//	return _veSet->_veGraphicsSet->_renderTextureVec.size();
	//}

	void SpriteEffect2D::SetCustomShaderInfo()
	{
		//Custom Vertex Shader라면
		//VertexPositionColorTexture을 활용해야 한다!

		RenderVertexShader* tCustomVS = _veSet->_veGraphicsSet->_customRenderVertexShader;
		RenderPixelShader* tCustomPS = _veSet->_veGraphicsSet->_customRenderPixelShader;

		if ((tCustomVS != nullptr) && (tCustomPS != nullptr))
		{
			// Bind를 사용하지 않을 것이다. 
			// InputLayout이 이상하게 되기 때문에.
			_DXStorage->_deviceContext->VSSetShader(tCustomVS->GetVertexShader(), nullptr, 0);
			_DXStorage->_deviceContext->PSSetShader(tCustomPS->GetPixelShader(), nullptr, 0);

			_DXStorage->_deviceContext->IASetInputLayout(_veSet->_veGraphicsSet->_inputLayout);
			
			//CBuffer는 일단 Custom하지 않음. 일단 이것만.
		}
		//deviceContext->PSSetConstantBuffers(...);
		//deviceContext->PSSetShaderResources(...);
	}

}
