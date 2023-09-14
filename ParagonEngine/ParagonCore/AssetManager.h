#pragma once
#include "Singleton.h"
#include "BaseResource.h"
#include "EngineResource.h"
#include "GraphicsResource.h"
#include "ResourceHelper.h"

#include "../ParagonGraphics/GraphicsResourceManager.h"
#include "../ParagonGameEngine/EngineResourceManager.h"

#include <string>
#include <memory>
#include <set>
#include <stdexcept>
#include <type_traits>

#ifdef _DEBUG
#pragma comment(lib,"..\\x64\\Debug\\ParagonGraphics.lib")
#else
#pragma comment(lib,"..\\x64\\Release\\ParagonGraphics.lib")
#endif // _DEBUG

#ifdef _DEBUG
#pragma comment(lib,"..\\x64\\Debug\\ParagonGameEngine.lib")
#else
#pragma comment(lib,"..\\x64\\Release\\ParagonGameEngine.lib")
#endif // _DEBUG

/// <summary>
/// 게임 엔진 내의 모든 애셋/리소스 내용을 총괄할 매니저.
/// 
/// CAUTION:
/// Asset Load / Unload를 하기 위해서는 반드시 본 함수로 실행해야 한다. 
/// Graphics/Engine ResourceManager 직접 접근 금지.
/// Asset관리를 받을 대상은 용도에 따라 EngineResource / GraphicsResource를 상속받아야 하고,
/// 내부적으로 반드시 InternalLoad / InternalUnload 함수가 구현되어 있어야 한다.
/// </summary>

namespace Pg::Core::Manager
{
	using Pg::Core::Resources::BaseResource;
	using Pg::Core::Resources::GraphicsResource;
	using Pg::Core::Resources::EngineResource;

	using Pg::Engine::Manager::EngineResourceManager;
	using Pg::Graphics::Manager::GraphicsResourceManager;

	class AssetManager : public Singleton<AssetManager>
	{
	public:
		AssetManager();
		~AssetManager();

		//리소스를 실제로 컨테이너에 로드하는 함수,
        template<typename T>
		std::shared_ptr<T> Load(const std::string& filepath);
        
		//리소스를 언로드하는 함수.
		void Unload(const std::string& filepath);

	private:
		std::set<std::string> _resources;
	};

	template<typename T>
	std::shared_ptr<T>
		Pg::Core::Manager::AssetManager::Load(const std::string& filepath)
	{
		static_assert(std::is_base_of<BaseResource, T>::value, "T는 BaseResource의 자식이어야 함!");
		static_assert(!std::is_same<BaseResource, T>::value, "T는 BaseResource 자체가 될 수는 없습니다.");
		static_assert(!std::is_same<EngineResource, T>::value, "T는 EngineResource 자체가 될 수는 없습니다.");
		static_assert(!std::is_same<GraphicsResource, T>::value, "T는 GraphicsResource 자체가 될 수는 없습니다.");
		
		//Path 표기 방식 일원화.
		std::string path = Pg::Core::Helper::ResourceHelper::ForcePathUniform(filepath);

		std::shared_ptr<T> res = nullptr;

		//무조건적으로 해당되는 리소스값이 반환되는지 체크한다.
		bool tAssureGot = false;

		//일단 Resource가 String 형태로 저장되어 있는지 확인.
		if (this->_resources.contains(path))
		{
			//이미 정보가 있다. 이 정보만을 가져와서 출력하면 되는 것!

			// Engine / Graphics 나눠서 실행한다.
			if constexpr (std::is_base_of<EngineResource, T>::value && (!std::is_base_of<GraphicsResource, T>::value))
			{
				//EngineResourceManager의 리소스를 가져오는 로직.
				res = EngineResourceManager::Instance()->GetResource(path);
				tAssureGot = true;
			}

			if constexpr ((!std::is_base_of<EngineResource, T>::value) && std::is_base_of<GraphicsResource, T>::value)
			{
				//GraphicsResourceManager의 리소스를 가져오는 로직.
				res = GraphicsResourceManager::Instance()->GetResource(path);
				tAssureGot = true;
			}

			assert(tAssureGot && "T는 EngineResource도, GraphicsResource도 아니다.");
		}
		else
		{
			//String 목록에 없다. 새로 만들어야 하는 상황!

			// Engine / Graphics 나눠서 실행한다.
			if constexpr (std::is_base_of<EngineResource, T>::value && (!std::is_base_of<GraphicsResource, T>::value))
			{
				//EngineResourceManager의 리소스를 가져오는 로직.
				res = EngineResourceManager::Instance()->CreateResource(path);
				
				tAssureGot = true;
			}

			if constexpr ((!std::is_base_of<EngineResource, T>::value) && std::is_base_of<GraphicsResource, T>::value)
			{
				//GraphicsResourceManager의 리소스를 가져오는 로직.
				res = GraphicsResourceManager::Instance()->CreateResource(path);
				tAssureGot = true;
			}

			assert(tAssureGot && "T는 EngineResource도, GraphicsResource도 아니다.");
		}

		//성공했으니, 메인 관리 리스트에 Path String 추가.
		_resources.insert(path);

		//이미 다이내믹 캐스트로 값이 들어온 상황.
		return res;
	}

}
