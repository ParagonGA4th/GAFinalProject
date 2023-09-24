#pragma once
#include "CoreSingleton.h"
#include "BaseResource.h"
#include "EngineResource.h"
#include "GraphicsResource.h"
#include "ResourceHelper.h"

#include <string>
#include <memory>
#include <set>
#include <stdexcept>
#include <type_traits>
#include <cassert>

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
/// 
/// Template Class Forward Declaration 방법으로 해결된다면, 
/// 사용되는 모든 저장 클래스들은 CPP 하단부에 정의되어야 한다.
/// </summary>

namespace Pg::Core
{
	class CoreMain;
}

namespace Pg::Engine
{
	namespace Manager
	{
		class EngineResourceManager;
	}
}

namespace Pg::Graphics
{
	namespace Manager
	{
		class GraphicsResourceManager;
	}
	
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
}
