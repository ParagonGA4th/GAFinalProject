#include "EngineResourceManager.h"

#ifdef _DEBUG
#pragma comment(lib,"..\\x64\\Debug\\ParagonCore.lib")
#else
#pragma comment(lib,"..\\x64\\Release\\ParagonCore.lib")
#endif // _DEBUG

namespace Pg::Engine::Manager
{
	EngineResourceManager::EngineResourceManager()
	{

	}

	EngineResourceManager::~EngineResourceManager()
	{

	}

	bool EngineResourceManager::DeleteResource(const std::string& path)
	{
		//리소스 활용을 위해 weak_ptr.lock()으로 체크.
		auto res = _resources[path].lock();

		//만약 지울 수 있는 Resource가 있으면?
		if (res)
		{
			res->InternalUnload();
			_resources.erase(path);

			return true;
		}
		else
		{
			return false;
		}
	}
}
