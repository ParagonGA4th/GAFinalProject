#include "VertexShader.h"

#include "LowDX11Storage.h"

#include "LayoutDefine.h"

HRESULT hr = NULL;

Pg::Graphics::VertexShader::VertexShader(std::wstring CSOFilePath, D3D11_INPUT_ELEMENT_DESC* vertexDesc)
	: RenderShader(CSOFilePath),
	_vertexDesc(vertexDesc)
{
	CreateShader();

	// InputLayout »ý¼º
	hr = _DXStorage->_device->CreateInputLayout(_vertexDesc, 2, _byteCode->GetBufferPointer(),
		_byteCode->GetBufferSize(), &(_inputLayout));
}

Pg::Graphics::VertexShader::VertexShader(std::wstring CSOFilePath)
	:RenderShader(CSOFilePath)
{
	_DXStorage = LowDX11Storage::GetInstance();
	CreateShader();
	_inputLayout = LayoutDefine::GetStatic1stLayout();
}

void Pg::Graphics::VertexShader::Bind()
{
	// Input Layout
	_DXStorage->_deviceContext->IASetInputLayout(_inputLayout);

	// Shader
	_DXStorage->_deviceContext->VSSetShader(_shader, nullptr, 0);
}

void Pg::Graphics::VertexShader::CreateShader()
{
	_DXStorage->_device->CreateVertexShader(_byteCode->GetBufferPointer(), _byteCode->GetBufferSize(), NULL, &_shader);
}

void Pg::Graphics::VertexShader::UnBind()
{
	_DXStorage->_deviceContext->VSSetShader(nullptr, nullptr, 0);
}

