// #TW_WIP AssetManager에서 언로드가 오류를 일으켜, 임시로 링크에러를 피하기 위해 주석 처리한 상태. Fix, To-DO!

#include "AssetManager.h"
#include "CoreMain.h"

#include "../ParagonGraphics/GraphicsMain.h"
#include "../ParagonGameEngine/EngineMain.h"

#include "../ParagonGraphics/GraphicsResourceManager.h"
#include "../ParagonGameEngine/EngineResourceManager.h"

#include "../ParagonUtil/ResourceHelper.h"

//<ResourcesList>
#include "../ParagonGraphics/RenderMaterial.h"
//</ResourcesList>

#include <cassert>

#ifdef _DEBUG
#pragma comment(lib,"..\\Builds\\x64\\Debug\\ParagonUtil.lib")
#else
#pragma comment(lib,"..\\Builds\\x64\\Release\\ParagonUtil.lib")
#endif // _DEBUG

#ifdef _DEBUG
#pragma comment(lib,"..\\Builds\\x64\\Debug\\ParagonGameEngine.lib")
#else
#pragma comment(lib,"..\\Builds\\x64\\Release\\ParagonGameEngine.lib")
#endif // _DEBUG

#ifdef _DEBUG
#pragma comment(lib,"..\\Builds\\x64\\Debug\\ParagonGraphics.lib")
#else
#pragma comment(lib,"..\\Builds\\x64\\Release\\ParagonGraphics.lib")
#endif // _DEBUG

namespace Pg::Core::Manager
{
	AssetManager::AssetManager() :
		_engineResourceManager(nullptr), _graphicsResourceManager(nullptr)
	{
		//
	}

	void AssetManager::Initialize(Pg::Core::CoreMain* core)
	{
		IEngine* tEngine = core->_engine.get();
		IGraphics* tGraphics = core->_graphics.get();

		Pg::Engine::EngineMain* tEngineMain = static_cast<Pg::Engine::EngineMain*>(tEngine);
		Pg::Graphics::GraphicsMain* tGraphicsMain = static_cast<Pg::Graphics::GraphicsMain*>(tGraphics);

		assert(tEngineMain);
		assert(tGraphicsMain);

		this->_engineResourceManager = tEngineMain->GetEngineResourceManager();
		this->_graphicsResourceManager = tGraphicsMain->GetGraphicsResourceManager();
	}

	AssetManager::~AssetManager()
	{

	}

	//원래는 Templated Class라 Header로 가야 하지만, CPP Forward Declaration 방법으로 문제 해결.
	template<typename T>
	void Pg::Core::Manager::AssetManager::Load(const std::string& filepath)
	{
		static_assert(std::is_base_of<BaseResource, T>::value, "T는 BaseResource의 자식이어야 함!");
		static_assert(!std::is_same<BaseResource, T>::value, "T는 BaseResource 자체가 될 수는 없습니다.");
		static_assert(!std::is_same<EngineResource, T>::value, "T는 EngineResource 자체가 될 수는 없습니다.");
		static_assert(!std::is_same<GraphicsResource, T>::value, "T는 GraphicsResource 자체가 될 수는 없습니다.");

		//Path 표기 방식 일원화.
		std::string path = Pg::Util::Helper::ResourceHelper::ForcePathUniform(filepath);

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
				res = _engineResourceManager->CreateResource<T>(path);

				tAssureGot = true;
			}

			if constexpr ((!std::is_base_of<EngineResource, T>::value) && std::is_base_of<GraphicsResource, T>::value)
			{
				//GraphicsResourceManager의 리소스를 가져오는 로직.
				res = _graphicsResourceManager->CreateResource<T>(path);
				tAssureGot = true;
			}

			assert(tAssureGot && "T는 EngineResource도, GraphicsResource도 아니다.");
		}

		//성공했으니, 메인 관리 리스트에 Path String 추가.
		_resources.insert(path);
	}

	//원래는 Templated Class라 Header로 가야 하지만, CPP Forward Declaration 방법으로 문제 해결.
	template<typename T>
	std::shared_ptr<T>
		Pg::Core::Manager::AssetManager::Get(const std::string& filepath)
	{
		static_assert(std::is_base_of<BaseResource, T>::value, "T는 BaseResource의 자식이어야 함!");
		static_assert(!std::is_same<BaseResource, T>::value, "T는 BaseResource 자체가 될 수는 없습니다.");
		static_assert(!std::is_same<EngineResource, T>::value, "T는 EngineResource 자체가 될 수는 없습니다.");
		static_assert(!std::is_same<GraphicsResource, T>::value, "T는 GraphicsResource 자체가 될 수는 없습니다.");

		//Path 표기 방식 일원화.
		std::string path = Pg::Util::Helper::ResourceHelper::ForcePathUniform(filepath);

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
				res = _engineResourceManager->GetResource<T>(path);
				tAssureGot = true;
			}

			if constexpr ((!std::is_base_of<EngineResource, T>::value) && std::is_base_of<GraphicsResource, T>::value)
			{
				//GraphicsResourceManager의 리소스를 가져오는 로직.
				res = _graphicsResourceManager->GetResource<T>(path);
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

	void AssetManager::Unload(const std::string& filepath)
	{
		//Path 표기 방식 일원화.
		std::string path = Pg::Util::Helper::ResourceHelper::ForcePathUniform(filepath);

		//우선적으로 일단 리스트에 값이 있는지를 확인.
		if (this->_resources.contains(path))
		{
			//명시적으로 두 인스턴스에 모두 삭제(및 언로드)를 진행해준다. 
			//템플릿을 쓰지 않기 위해, 둘 다 삭제 코드, 내부적으로 해당하는지 아닌지 반환.
			
			bool tIsEngineDelete = _engineResourceManager->DeleteResource(path);
			bool tIsGraphicsDelete = _graphicsResourceManager->DeleteResource(path);
			assert((tIsEngineDelete ^ tIsGraphicsDelete) && "하나만 삭제하는 경우가 발생하지 않음, 오류");
		
			//이제 메인 AssetManager에서 값을 삭제.
			_resources.erase(path);
		}
		else
		{
			//리스트에 값이 없다는 뜻이다. 언로드될 수도 없음.
			return;
		}
	}

	///실제로 AssetManager에서 템플릿 인스턴스화될 클래스들은 CPP에서 전방참조된다.
	///List가 추가될 수록, 컴파일러가 알아차릴 수 있게 목록이 추가되어야. 
	///오류가 나지 않게, 실제로 쓰이는 코드들만 CPP에서 전방선언되어야 한다.
	//template void AssetManager::Load<Pg::Graphics::RenderMaterial>(const std::string&);
	//template std::shared_ptr<Pg::Graphics::RenderMaterial> 
	//	AssetManager::Get<Pg::Graphics::RenderMaterial>(const std::string&);
}
