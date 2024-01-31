//#include "VertexShader.h"
//
//#include "LowDX11Storage.h"
//
//#include "LayoutDefine.h"
//
//HRESULT hr = NULL;
//
//Pg::Graphics::VertexShader::VertexShader(Pg::Data::Enums::eAssetDefine define, const std::string& filePath)
//	: RenderShader(define, filePath)
//{
//	_DXStorage = LowDX11Storage::GetInstance();
//	CreateShader();
//	_inputLayout = LayoutDefine::GetStatic1stLayout();
//}
//
//Pg::Graphics::VertexShader::VertexShader(Pg::Data::Enums::eAssetDefine define, const std::string& filePath, D3D11_INPUT_ELEMENT_DESC* vertexDesc)
//	:RenderShader(define, filePath),
//	_vertexDesc(vertexDesc)
//{
//	_DXStorage = LowDX11Storage::GetInstance();
//	CreateShader();
//
//	// InputLayout »ý¼º
//	hr = _DXStorage->_device->CreateInputLayout(_vertexDesc, 2, _byteCode->GetBufferPointer(),
//		_byteCode->GetBufferSize(), &(_inputLayout));
//}
//
//void Pg::Graphics::VertexShader::Bind()
//{
//	// Input Layout
//	_DXStorage->_deviceContext->IASetInputLayout(_inputLayout);
//
//	// Shader
//	_DXStorage->_deviceContext->VSSetShader(_shader, nullptr, 0);
//
//}
//
//void Pg::Graphics::VertexShader::CreateShader()
//{
//	_DXStorage->_device->CreateVertexShader(_byteCode->GetBufferPointer(), _byteCode->GetBufferSize(), NULL, &_shader);
//}
//
//void Pg::Graphics::VertexShader::UnBind()
//{
//	_DXStorage->_deviceContext->VSSetShader(nullptr, nullptr, 0);
//
//}
//
