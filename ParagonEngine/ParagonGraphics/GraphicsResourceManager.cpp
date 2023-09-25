#include "GraphicsResourceManager.h"
#include "AssetBasic2DLoader.h"
#include "AssetBasic3DLoader.h"

#ifdef _DEBUG
#pragma comment(lib,"..\\x64\\Debug\\ParagonCore.lib")
#else
#pragma comment(lib,"..\\x64\\Release\\ParagonCore.lib")
#endif // _DEBUG

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
}
