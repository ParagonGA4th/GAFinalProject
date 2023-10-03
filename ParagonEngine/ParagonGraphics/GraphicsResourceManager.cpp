#include "GraphicsResourceManager.h"
#include "AssetBasic2DLoader.h"
#include "AssetBasic3DLoader.h"

namespace Pg::Graphics::Manager
{
	GraphicsResourceManager::GraphicsResourceManager()
	{
		_asset2DLoader = std::make_unique<Pg::Graphics::Loader::AssetBasic2DLoader>();
		_asset3DLoader = std::make_unique<Pg::Graphics::Loader::AssetBasic3DLoader>();
	}

	GraphicsResourceManager::~GraphicsResourceManager()
	{

	}

	Pg::Graphics::Loader::AssetBasic3DLoader* GraphicsResourceManager::GetBasic3DLoader()
	{
		assert(_asset3DLoader);
		return _asset3DLoader.get();
	}

	Pg::Graphics::Loader::AssetBasic2DLoader* GraphicsResourceManager::GetBasic2DLoader()
	{
		assert(_asset2DLoader);
		return _asset2DLoader.get();
	}

}
