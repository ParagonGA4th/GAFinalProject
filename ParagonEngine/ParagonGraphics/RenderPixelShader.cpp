#include "RenderPixelShader.h"
#include "LowDX11Storage.h"
#include "GraphicsResourceManager.h"
#include "AssetCombinedLoader.h"

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
		using Pg::Graphics::Manager::GraphicsResourceManager;
		using Pg::Graphics::Loader::AssetCombinedLoader;

		GraphicsResourceManager* tResManager = Pg::Graphics::Manager::GraphicsResourceManager::Instance();
		AssetCombinedLoader* tComLoader = tResManager->GetCombinedLoader();
		tComLoader->LoadRenderPixelShader(_filePath, this);
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
