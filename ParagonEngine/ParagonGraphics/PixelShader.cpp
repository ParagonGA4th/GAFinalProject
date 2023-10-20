#include "PixelShader.h"

#include "LowDX11Storage.h"

Pg::Graphics::PixelShader::PixelShader(std::wstring CSOFilePath) : RenderShader(CSOFilePath)
{
	_DXStorage = LowDX11Storage::GetInstance();
	CreateShader();
}

void Pg::Graphics::PixelShader::Bind()
{
	_DXStorage->_deviceContext->PSSetShader(_shader, nullptr, 0);
}

void Pg::Graphics::PixelShader::CreateShader()
{
	_DXStorage->_device->CreatePixelShader(_byteCode->GetBufferPointer(), _byteCode->GetBufferSize(), NULL, &_shader);
}

void Pg::Graphics::PixelShader::UnBind()
{
	_DXStorage->_deviceContext->PSSetShader(nullptr, nullptr, 0);
}
