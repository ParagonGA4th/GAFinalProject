#include "AssetManager.h"
#include "CoreMain.h"
#include "IEngine.h"
#include "IGraphics.h"

#include "../ParagonGraphics/GraphicsResourceManager.h"
#include "../ParagonGameEngine/EngineResourceManager.h"

#include "../ParagonUtil/ResourceHelper.h"

//<ResourcesList>
#include "../ParagonGraphics/RenderMaterial.h"
//</ResourcesList>

#include <algorithm>
#include <cassert>

#ifdef _DEBUG
#pragma comment(lib,"..\\Builds\\x64\\Debug\\ParagonUtil.lib")
#else
#pragma comment(lib,"..\\Builds\\x64\\Release\\ParagonUtil.lib")
#endif // _DEBUG

namespace Pg::Core::Manager
{
	using Pg::Util::Helper::ResourceHelper;

	AssetManager::AssetManager()
	{
		//
	}

	AssetManager::~AssetManager()
	{

	}

	void AssetManager::Initialize(Pg::Core::CoreMain* core) 
	{
		_coreMain = core;
		_perFrameToLoadResources.reserve(30);
		_perFrameToUnloadResources.reserve(30);
	}

	void AssetManager::Update(Pg::Core::IEngine* engine, Pg::Core::IGraphics* graphics)
	{

	}

	bool AssetManager::IsExistResource(const std::string& filepath, Pg::Core::Enums::eAssetDefine define)
	{
		auto res = std::find_if(_resourceMap.begin(), _resourceMap.end(),
			[&filepath, &define](const std::pair<std::string, Pg::Core::Enums::eAssetDefine>& val)
			-> bool {return ((val.first == filepath) && (val.second == define)); });

		bool tIsFound = (res != _resourceMap.end()) ? true : false;
		return tIsFound;
	}

	void AssetManager::LoadResource(const std::string& filepath, Pg::Core::Enums::eAssetDefine define)
	{
		auto tInfo = std::make_pair(filepath, define);
		this->_perFrameToLoadResources.emplace_back(tInfo);
	}

	void AssetManager::UnloadResource(const std::string& filepath)
	{
		auto tInfo = filepath;
		this->_perFrameToUnloadResources.emplace_back(tInfo);
	}

}
