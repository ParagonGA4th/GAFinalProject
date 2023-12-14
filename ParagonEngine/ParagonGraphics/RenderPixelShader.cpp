#include "RenderPixelShader.h"
#include "LowDX11Storage.h"
#include "LayoutDefine.h"

namespace Pg::Graphics
{
	using Pg::Data::Enums::eAssetDefine;

	RenderPixelShader::RenderPixelShader(Pg::Data::Enums::eAssetDefine define, const std::string& filePath) :
		GraphicsResource(define, typeid(this).name(), filePath)
	{
		_DXStorage = LowDX11Storage::GetInstance();
	}

	RenderPixelShader::~RenderPixelShader()
	{
		//
	}

	void RenderPixelShader::InternalLoad()
	{
		std::wstring wFilePath;
		wFilePath.assign(_filePath.begin(), _filePath.end());

		//Blob으로 D3D 파일을 읽어온다. (Load)
		HR(D3DReadFileToBlob(wFilePath.c_str(), &_byteCode));

		//Pixel Shader 갖고 오기.
		HR(_DXStorage->_device->CreatePixelShader(_byteCode->GetBufferPointer(), _byteCode->GetBufferSize(), NULL, &_shader));
	}

	void RenderPixelShader::InternalUnload()
	{

	}

	void RenderPixelShader::Bind()
	{
		_DXStorage->_deviceContext->PSSetShader(_shader, nullptr, 0);
	}

	void RenderPixelShader::Unbind()
	{
		_DXStorage->_deviceContext->PSSetShader(nullptr, nullptr, 0);
	}
}
