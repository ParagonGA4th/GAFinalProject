#include "RenderVertexShader.h"
#include "LowDX11Storage.h"
#include "GraphicsResourceManager.h"
#include "AssetCombinedLoader.h"
#include <filesystem>

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
		using Pg::Graphics::Manager::GraphicsResourceManager;
		using Pg::Graphics::Loader::AssetCombinedLoader;

		GraphicsResourceManager* tResManager = Pg::Graphics::Manager::GraphicsResourceManager::Instance();
		AssetCombinedLoader* tComLoader = tResManager->GetCombinedLoader();
		tComLoader->LoadRenderVertexShader(_filePath, this);

#if defined(DEBUG) | defined(_DEBUG)
		std::string tName = std::filesystem::path(_filePath).filename().string();
		tName += "_VS";
		HR(_shader->SetPrivateData(WKPDID_D3DDebugObjectName, tName.length(), tName.data()));
#endif
	}

	void RenderVertexShader::InternalUnload()
	{

	}

	void RenderVertexShader::Bind(ID3D11InputLayout* customInputLayout)
	{
		//nullptr일 때는 Custom Input Layout 활용.
		if (customInputLayout == nullptr)
		{
			// Input Layout
			_DXStorage->_deviceContext->IASetInputLayout(_inputLayout);
		}
		else
		{
			// Custom Input Layout
			_DXStorage->_deviceContext->IASetInputLayout(customInputLayout);
		}

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

	void RenderVertexShader::GetVertexShaderBytecode(void const** pShaderByteCode, size_t* pByteCodeLength)
	{
		*pShaderByteCode = _byteCode->GetBufferPointer();
		*pByteCodeLength = _byteCode->GetBufferSize();
	}

	ID3D11VertexShader* RenderVertexShader::GetVertexShader()
	{
		return _shader;
	}

}