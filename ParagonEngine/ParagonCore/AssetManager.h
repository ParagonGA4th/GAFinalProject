#pragma once
#include "Singleton.h"
#include "BaseResource.h"

#include <string>
#include <memory>
#include <unordered_map>
#include <stdexcept>
#include <type_traits>

/// <summary>
/// 게임 엔진 내의 모든 애셋/리소스 내용을 총괄할 매니저.
/// </summary>

namespace Pg::Core::Manager
{
	using Pg::Core::Resources::BaseResource;

	class AssetManager : public Singleton<AssetManager>
	{
	public:
		AssetManager();
		~AssetManager();

		//리소스를 실제로 컨테이너에 로드하는 함수,
        template<typename T>
		std::shared_ptr<T> Load(const std::string& path);
        
		//리소스를 언로드하는 함수.
		void Unload(const std::string& path);

	private:
		std::unordered_map<std::string, std::weak_ptr<BaseResource>> resources;
		



	};

	template<typename T>
	std::shared_ptr<T>
		Pg::Core::Manager::AssetManager::Load(const std::string& path)
	{
		static_assert(std::is_base_of<BaseResource, T>::value, "T는 BaseResource의 자식이어야 함!");

		//리소스 활용을 위해 weak_ptr.lock()으로 체크.
		auto res = resources[path].lock();

		//std::shared_ptr이 null이면 발동.
		if (!res) 
		{
			// 없으면, 템플릿으로 들어온 값으로 생성 및 Load.
			resources[path] = res = std::make_shared<T>(path);
			res->InternalLoad();
		}

		//스마트 포인터로 다이나믹 캐스팅.
		auto return_value = std::dynamic_pointer_cast<T>(res);
		if (!return_value) 
		{
			throw std::runtime_error(std::string("리소스 '") + path + "' 가 이미 다른 타입으로 로드되어 있음!");
		}
		return return_value;
	}

}
