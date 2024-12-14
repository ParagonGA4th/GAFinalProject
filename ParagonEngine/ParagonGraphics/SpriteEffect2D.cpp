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
		//Custom Vertex Shader���
		//VertexPositionColorTexture�� Ȱ���ؾ� �Ѵ�!

		RenderVertexShader* tCustomVS = _veSet->_veGraphicsSet->_customRenderVertexShader;
		RenderPixelShader* tCustomPS = _veSet->_veGraphicsSet->_customRenderPixelShader;

		if ((tCustomVS != nullptr) && (tCustomPS != nullptr))
		{
			// Bind�� ������� ���� ���̴�. 
			// InputLayout�� �̻��ϰ� �Ǳ� ������.
			_DXStorage->_deviceContext->VSSetShader(tCustomVS->GetVertexShader(), nullptr, 0);
			_DXStorage->_deviceContext->PSSetShader(tCustomPS->GetPixelShader(), nullptr, 0);

			_DXStorage->_deviceContext->IASetInputLayout(_veSet->_veGraphicsSet->_inputLayout);
			
			//CBuffer�� �ϴ� Custom���� ����. �ϴ� �̰͸�.
		}
		//deviceContext->PSSetConstantBuffers(...);
		//deviceContext->PSSetShaderResources(...);
	}

}
