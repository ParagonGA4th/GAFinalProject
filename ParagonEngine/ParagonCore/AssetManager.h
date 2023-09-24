#pragma once
#include "CoreSingleton.h"
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
#include <cassert>

#ifdef _DEBUG
#pragma comment(lib,"..\\x64\\Debug\\ParagonGameEngine.lib")
#else
#pragma comment(lib,"..\\x64\\Release\\ParagonGameEngine.lib")
#endif // _DEBUG

#ifdef _DEBUG
#pragma comment(lib,"..\\x64\\Debug\\ParagonGraphics.lib")
#else
#pragma comment(lib,"..\\x64\\Release\\ParagonGraphics.lib")
#endif // _DEBUG

/// <summary>
/// 게임 엔진 내의 모든 애셋/리소스 내용을 총괄할 매니저.
/// 
/// CAUTION:
/// Asset Load / Unload를 하기 위해서는 반드시 본 함수로 실행해야 한다. 
/// Graphics/Engine ResourceManager 직접 접근 금지.
/// Asset관리를 받을 대상은 용도에 따라 EngineResource / GraphicsResource를 상속받아야 하고,
/// 하위 리소스들은 내부적으로 반드시 InternalLoad / InternalUnload 함수가 구현되어 있어야 한다.
/// 리소스 로드와 가져오는 시점은 분리되어 있다.
/// 
/// NOTES:
/// AssetManager는 Graphics, Engine을 알아야 하는 상황인데, 
/// -> 다른 것도 아니고 Unload만 오류가 났던 이유는
/// 다른 Get / Create는 template function들이라 쓰일 때 오류를 일으키기 때문이다.
/// 이렇게 된다면, 개별적 리소스 매니저에 있는 함수들이 독립적으로 호출되어야 하는가 ?
/// 그러면 로직이 관리되지 않는다.
/// 이렇게 된다면, AssetManager 관리 로직이 바뀌어야 한다.
/// Core의 AssetManager가 Graphics, Engine을 아는 것은 변하지 않는다.
/// 다만, 더 이상 Graphics, Engine 내부에서 AssetManager를 통해서 리소스에 접근한다는 생각을 버리고,
/// GameEngine, Graphics에서 자체 리소스매니저를 통해서 접근한다는 생각을 해야 한다는 것인데..
/// </summary>

namespace Pg::Core
{
	class CoreMain;
}

namespace Pg::Core::Manager
{
	using Pg::Core::Resources::BaseResource;
	using Pg::Core::Resources::GraphicsResource;
	using Pg::Core::Resources::EngineResource;

	class AssetManager : public Pg::Core::Singleton<AssetManager>
	{
	public:
		AssetManager();
		~AssetManager();
		
		//AssetManager 초기화. Engine/Graphics의 리소스 매니저를 받아온다.
		void Initialize(Pg::Core::CoreMain* core);

		//리소스를 실제로 컨테이너에 로드하는 함수, 로드만 할 뿐이지 함수 자체가 리소스를 반환하지는 않는다.
        template<typename T>
		void Load(const std::string& filepath);
        
		//리소스를 언로드하는 함수.
		void Unload(const std::string& filepath);
		
		//리소스를 실제로 반환받아서 쓸 수 있게 하는 함수. 
		template<typename T>
		std::shared_ptr<T> Get(const std::string& filepath);

	private:
		std::set<std::string> _resources;

	private:
		//별도로 각가 GameEngine / Graphics 프로젝트에서 만들어졌던 리소스 매니저.
		Pg::Engine::Manager::EngineResourceManager* _engineResourceManager;
		Pg::Graphics::Manager::GraphicsResourceManager* _graphicsResourceManager;
	};

	template<typename T>
	void Pg::Core::Manager::AssetManager::Load(const std::string& filepath)
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
			//이미 리소스가 목록에 있는 상황이다. 그냥 리턴해도 상관X.
			return;
		}
		else
		{
			//String 목록에 없다. 새로 만들어야 하는 상황!

			// Engine / Graphics 나눠서 실행한다.
			if constexpr (std::is_base_of<EngineResource, T>::value && (!std::is_base_of<GraphicsResource, T>::value))
			{
				//EngineResourceManager의 리소스를 가져오는 로직.
				res = _engineResourceManager->CreateResource(path);
				
				tAssureGot = true;
			}

			if constexpr ((!std::is_base_of<EngineResource, T>::value) && std::is_base_of<GraphicsResource, T>::value)
			{
				//GraphicsResourceManager의 리소스를 가져오는 로직.
				res = _graphicsResourceManager->CreateResource(path);
				tAssureGot = true;
			}

			assert(tAssureGot && "T는 EngineResource도, GraphicsResource도 아니다.");
		}

		//성공했으니, 메인 관리 리스트에 Path String 추가.
		_resources.insert(path);
	}

	template<typename T>
	std::shared_ptr<T>
		Pg::Core::Manager::AssetManager::Get(const std::string& filepath)
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
				res = _engineResourceManager->GetResource(path);
				tAssureGot = true;
			}

			if constexpr ((!std::is_base_of<EngineResource, T>::value) && std::is_base_of<GraphicsResource, T>::value)
			{
				//GraphicsResourceManager의 리소스를 가져오는 로직.
				res = _graphicsResourceManager->GetResource(path);
				tAssureGot = true;
			}

			assert(tAssureGot && "T는 EngineResource도, GraphicsResource도 아니다.");
		}
		else
		{
			//Get하는 시점에서 리소스는 없으면 안된다.

			assert(false && "AssetManager에서, 없는 리소스를 Get하려고 함!");
		}

		//성공했으니, 메인 관리 리스트에 Path String 추가.
		_resources.insert(path);

		//이미 다이내믹 캐스트로 값이 들어온 상황.
		return res;
	}

}
