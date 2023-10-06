#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include <stdexcept>
#include <type_traits>
#include <cassert>

#include "../ParagonData/EngineResource.h"
#include "../ParagonProcess/CoreSingleton.h"

/// <summary>
/// AssetManager에 의해 제어되는 엔진 리소스 관리 전담 매니저. 독단적 사용 불가.
/// 관리 대신, 생성/삭제만 전담하게 된다.
/// </summary>

//전방 선언, Namespace 다름에도 Friend Class 활용하기 위해.
namespace Pg::Core
{
	class ProcessMain;
	namespace Manager
	{
		class AssetManager;
	}
}

namespace Pg::Engine
{
	class EngineMain;
}

namespace Pg::Engine::Manager
{
	class EngineResourceManager : public Pg::Core::Singleton<EngineResourceManager>
	{
		friend class Pg::Core::Manager::AssetManager;
		friend class Pg::Engine::EngineMain;
	public:
		EngineResourceManager();
		~EngineResourceManager();


	private:

		//리소스를 생성한다. 
		template<typename T>
		std::shared_ptr<T> CreateResource(const std::string& path, Pg::Data::Enums::eAssetDefine define);

		template<typename T>
		std::shared_ptr<T> GetResource(const std::string& path);

		//리소스를 언로드하는 함수. AssetManager에서 동시에 발동. 삭제 성공하면 True 반환.
		inline bool DeleteResource(const std::string& path);

	private:
		std::unordered_map<std::string, std::weak_ptr<Pg::Data::Resources::EngineResource>> _resources;

	};

	template<typename T>
	std::shared_ptr<T>
		Pg::Engine::Manager::EngineResourceManager::CreateResource(const std::string& path, Pg::Data::Enums::eAssetDefine define)
	{
		//이미 AssetManager의 시점에서는 static하게 체크 완료.
		//AssetManager의 목록과 연동이 되어야 한다.

		auto res = _resources[path].lock();

		assert((!res) && "막히면 이미 만들어진 리소스를 로직 상으로 다시 만드려고 했다는 뜻이다. 로직을 고쳐야.");

		// 없으면, 템플릿으로 들어온 값으로 생성 및 Load.
		_resources[path] = res = std::make_shared<T>(define, path);
		res->InternalLoad();

		//원 형태로 반환해줘야 한다.
		auto return_value = std::dynamic_pointer_cast<T>(res);
		if (!return_value)
		{
			throw std::runtime_error(std::string("[GameEngine] 리소스 '") + path + "'를 해당 타입으로 변환하는 것이 불가능!!");
		}
		return return_value;
	}

	template<typename T>
	std::shared_ptr<T>
		Pg::Engine::Manager::EngineResourceManager::GetResource(const std::string& path)
	{
		//shared_ptr로 변환.
		auto res = _resources[path].lock();

		assert(res && "막히면 없는 리소스를 로직 상으로 가져오라고 했다는 뜻. 로직을 고쳐야.");

		//원형태로 변환
		auto return_value = std::dynamic_pointer_cast<T>(res);
		if (!return_value)
		{
			throw std::runtime_error(std::string("[GameEngine] 리소스 '") + path + "'를 해당 타입으로 변환하는 것이 불가능!!");
		}
		return return_value;
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


