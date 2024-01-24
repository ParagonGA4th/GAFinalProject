#include "RenderVertexShader.h"
#include "LowDX11Storage.h"
#include "GraphicsResourceManager.h"
#include "AssetCombinedLoader.h"

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