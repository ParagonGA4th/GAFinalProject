#include "RenderVertexShader.h"
#include "LowDX11Storage.h"
#include "LayoutDefine.h"

namespace Pg::Graphics
{
	using Pg::Data::Enums::eAssetDefine;
	
	RenderVertexShader::RenderVertexShader(Pg::Data::Enums::eAssetDefine define, const std::string& filePath) :
		GraphicsResource(define, typeid(this).name(), filePath)
	{
		_DXStorage = LowDX11Storage::GetInstance();
	}
	
	RenderVertexShader::~RenderVertexShader()
	{
		//
	}

	void RenderVertexShader::InternalLoad()
	{
		std::wstring wFilePath;
		wFilePath.assign(_filePath.begin(), _filePath.end());

		//Blob으로 D3D 파일을 읽어온다. (Load)
		HR(D3DReadFileToBlob(wFilePath.c_str(), &_byteCode));

		//Vertex Shader 갖고 오기.
		_DXStorage->_device->CreateVertexShader(_byteCode->GetBufferPointer(), _byteCode->GetBufferSize(), NULL, &_shader);

		//InputLayout : 정한 기본으로 갖고 오기.
		_inputLayout = LayoutDefine::GetDeferredQuadLayout();
	}

	void RenderVertexShader::InternalUnload()
	{

	}

	void RenderVertexShader::Bind()
	{
		// Input Layout
		_DXStorage->_deviceContext->IASetInputLayout(_inputLayout);
		// Shader
		_DXStorage->_deviceContext->VSSetShader(_shader, nullptr, 0);
		// Topology 
		_DXStorage->_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		// RS.
		_DXStorage->_deviceContext->RSSetState(_DXStorage->_solidState);
	}

	void RenderVertexShader::Unbind()
	{
		_DXStorage->_deviceContext->VSSetShader(nullptr, nullptr, 0);
	}
}