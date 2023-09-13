#include "AssetManager.h"

namespace Pg::Core::Manager
{
	AssetManager::AssetManager()
	{

	}

	AssetManager::~AssetManager()
	{

	}

	void AssetManager::Unload(const std::string& path)
	{
		//리소스 활용을 위해 weak_ptr.lock()으로 체크.
		auto res = resources[path].lock();

		//std::shared_ptr이 null이 아니라면, 발동.
		if (res)
		{
			res->InternalUnload();
			resources.erase(path);
		}
	}



}
