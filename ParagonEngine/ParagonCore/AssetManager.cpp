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
		//실제 Loading 로직.
		for (auto& it : _perFrameToLoadResources)
		{
			if (Pg::Util::Helper::ResourceHelper::IsGraphicsResource(it.second))
			{
				graphics->LoadResource(it.first, it.second);
			}
			else
			{
				engine->LoadResource(it.first, it.second);
			}
			
			//실제로 로드되었으면, AssetManager의 목록에 연동해서 관리해야 한다.
			this->_resourceMap.insert(std::make_pair(it.first, it.second));
		}

		//실제 Unloading 로직.
		for (auto& it : _perFrameToUnloadResources)
		{
			graphics->UnloadResource(it);
			engine->UnloadResource(it);

			//실제로 언로드되었으면, AssetManager의 목록에 연동해서 관리해야 한다.
			this->_resourceMap.erase(it);
		}

		//다음 Iteration을 위해 Clear.
		_perFrameToLoadResources.clear();
		_perFrameToUnloadResources.clear();

	}

	bool AssetManager::IsExistResource(const std::string& filepath)
	{
		auto res = std::find_if(_resourceMap.begin(), _resourceMap.end(),
			[&filepath](const std::pair<std::string, Pg::Core::Enums::eAssetDefine>& val)
			-> bool {return (val.first == filepath); });

		bool tIsFound = (res != _resourceMap.end()) ? true : false;
		return tIsFound;
	}

	void AssetManager::LoadResource(const std::string& filepath, Pg::Core::Enums::eAssetDefine define)
	{
		//Load하기 전에, 이미 목록에 없는지 체크!
		if (!IsExistResource(filepath))
		{
			auto tInfo = std::make_pair(filepath, define);
			this->_perFrameToLoadResources.emplace_back(tInfo);
		}
	}

	void AssetManager::UnloadResource(const std::string& filepath)
	{
		//Unload하기 전에, 이미 목록에 있는지 체크!
		if (IsExistResource(filepath))
		{
			auto tInfo = filepath;
			this->_perFrameToUnloadResources.emplace_back(tInfo);
		}
	}

}
