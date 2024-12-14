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
/// AssetManager�� ���� ����Ǵ� ���� ���ҽ� ���� ���� �Ŵ���. ������ ��� �Ұ�.
/// ���� ���, ����/������ �����ϰ� �ȴ�.
/// </summary>

//���� ����, Namespace �ٸ����� Friend Class Ȱ���ϱ� ����.
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

		//���ҽ��� �����Ѵ�. 
		template<typename T>
		std::shared_ptr<T> CreateResource(const std::string& path, Pg::Data::Enums::eAssetDefine define);

		template<typename T>
		std::shared_ptr<T> GetResource(const std::string& path);

		//���ҽ��� ��ε��ϴ� �Լ�. AssetManager���� ���ÿ� �ߵ�. ���� �����ϸ� True ��ȯ.
		inline bool DeleteResource(const std::string& path);

	private:
		std::unordered_map<std::string, std::weak_ptr<Pg::Data::Resources::EngineResource>> _resources;

	};

	template<typename T>
	std::shared_ptr<T>
		Pg::Engine::Manager::EngineResourceManager::CreateResource(const std::string& path, Pg::Data::Enums::eAssetDefine define)
	{
		//�̹� AssetManager�� ���������� static�ϰ� üũ �Ϸ�.
		//AssetManager�� ��ϰ� ������ �Ǿ�� �Ѵ�.

		auto res = _resources[path].lock();

		assert((!res) && "������ �̹� ������� ���ҽ��� ���� ������ �ٽ� ������� �ߴٴ� ���̴�. ������ ���ľ�.");

		// ������, ���ø����� ���� ������ ���� �� Load.
		_resources[path] = res = std::make_shared<T>(define, path);
		res->InternalLoad();

		//�� ���·� ��ȯ����� �Ѵ�.
		auto return_value = std::dynamic_pointer_cast<T>(res);
		if (!return_value)
		{
			throw std::runtime_error(std::string("[GameEngine] ���ҽ� '") + path + "'�� �ش� Ÿ������ ��ȯ�ϴ� ���� �Ұ���!!");
		}
		return return_value;
	}

	template<typename T>
	std::shared_ptr<T>
		Pg::Engine::Manager::EngineResourceManager::GetResource(const std::string& path)
	{
		//shared_ptr�� ��ȯ.
		auto res = _resources[path].lock();

		assert(res && "������ ���� ���ҽ��� ���� ������ ��������� �ߴٴ� ��. ������ ���ľ�.");

		//�����·� ��ȯ
		auto return_value = std::dynamic_pointer_cast<T>(res);
		if (!return_value)
		{
			throw std::runtime_error(std::string("[GameEngine] ���ҽ� '") + path + "'�� �ش� Ÿ������ ��ȯ�ϴ� ���� �Ұ���!!");
		}
		return return_value;
	}

	bool EngineResourceManager::DeleteResource(const std::string& path)
	{
		//���ҽ� Ȱ���� ���� weak_ptr.lock()���� üũ.
		auto res = _resources[path].lock();

		//���� ���� �� �ִ� Resource�� ������?
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


