#include "SystemVertexShader.h"
#include "LowDX11Storage.h"

#include <cassert>

namespace Pg::Graphics
{
	SystemVertexShader::SystemVertexShader(const std::wstring& wFilePath, ID3D11InputLayout* tInputLayout, ID3D11RasterizerState* rsState, D3D_PRIMITIVE_TOPOLOGY topology)
		: SystemShader(wFilePath), _shader(nullptr), _inputLayout(tInputLayout), _rsState(rsState), _topology(topology)
	{
		//Vertex Shader Loading
		HR(_DXStorage->_device->CreateVertexShader(_byteCode->GetBufferPointer(), _byteCode->GetBufferSize(), NULL, &_shader));
		assert(_inputLayout != nullptr);
		assert(_shader != nullptr);
	}

	SystemVertexShader::~SystemVertexShader()
	{

	}

	ID3D11VertexShader* SystemVertexShader::GetVertexShader()
	{
		return _shader;
	}

	void SystemVertexShader::Bind()
	{
		// Input Layout
		_DXStorage->_deviceContext->IASetInputLayout(_inputLayout);
		// Topology 
		_DXStorage->_deviceContext->IASetPrimitiveTopology(_topology);
		// RS.
		_DXStorage->_deviceContext->RSSetState(_rsState);
		// Shader
		_DXStorage->_deviceContext->VSSetShader(_shader, nullptr, 0);
	}

	void SystemVertexShader::Unbind()
	{
		_DXStorage->_deviceContext->VSSetShader(nullptr, nullptr, 0);
	}

	ID3D11InputLayout* SystemVertexShader::GetInputLayout()
	{
		return _inputLayout;
	}

}

