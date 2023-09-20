#include "VertexShader.h"

#include "LowDX11Storage.h"

HRESULT hr = NULL;

Pg::Graphics::VertexShader::VertexShader(LowDX11Storage* storage, std::wstring CSOFilePath, D3D11_INPUT_ELEMENT_DESC* vertexDesc)
	: Shader(storage, CSOFilePath),
	_vertexDesc(vertexDesc)
{
	CreateShader();

	// InputLayout »ý¼º
	hr = _DXStorage->_device->CreateInputLayout(_vertexDesc, 2, _byteCode->GetBufferPointer(),
		_byteCode->GetBufferSize(), &(_inputLayout));
}


void Pg::Graphics::VertexShader::Bind()
{
	_DXStorage->_deviceContext->VSSetShader(_shader, nullptr, 0);
}

void Pg::Graphics::VertexShader::CreateShader()
{
	_DXStorage->_device->CreateVertexShader(_byteCode->GetBufferPointer(), _byteCode->GetBufferSize(), NULL, &_shader);
}

