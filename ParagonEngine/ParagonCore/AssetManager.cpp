#include "AssetManager.h"
#include "CoreMain.h"

#include "../ParagonGraphics/GraphicsResourceManager.h"
#include "../ParagonGameEngine/EngineResourceManager.h"

#include "../ParagonUtil/ResourceHelper.h"

//<ResourcesList>
#include "../ParagonGraphics/RenderMaterial.h"
//</ResourcesList>

#include <cassert>

#ifdef _DEBUG
#pragma comment(lib,"..\\Builds\\x64\\Debug\\ParagonUtil.lib")
#else
#pragma comment(lib,"..\\Builds\\x64\\Release\\ParagonUtil.lib")
#endif // _DEBUG

namespace Pg::Core::Manager
{
	AssetManager::AssetManager()
	{
		//
	}

	void AssetManager::Initialize(Pg::Core::CoreMain* core) : _coreMain(core)
	{
		
	}

	AssetManager::~AssetManager()
	{

	}
}
