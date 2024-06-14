#include "SystemPixelShader.h"
#include "LowDX11Storage.h"
#include <cassert>

namespace Pg::Graphics
{
	SystemPixelShader::SystemPixelShader(const std::wstring& wFilePath)
		: SystemShader(wFilePath), _shader(nullptr)
	{
		//Vertex Shader Loading
		HR(_DXStorage->_device->CreatePixelShader(_byteCode->GetBufferPointer(), _byteCode->GetBufferSize(), NULL, &_shader));
		assert(_shader != nullptr);
	}

	SystemPixelShader::~SystemPixelShader()
	{

	}

	ID3D11PixelShader* SystemPixelShader::GetPixelShader()
	{
		return _shader;
	}

	void SystemPixelShader::Bind()
	{
		_DXStorage->_deviceContext->PSSetShader(_shader, nullptr, 0);
	}

	void SystemPixelShader::Unbind()
	{
		_DXStorage->_deviceContext->PSSetShader(nullptr, nullptr, 0);
	}

}