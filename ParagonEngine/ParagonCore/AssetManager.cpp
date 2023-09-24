// #TW_WIP AssetManager에서 언로드가 오류를 일으켜, 임시로 링크에러를 피하기 위해 주석 처리한 상태. Fix, To-DO!

#include "AssetManager.h"
#include "CoreMain.h"

#include "../ParagonGraphics/GraphicsMain.h"
#include "../ParagonGameEngine/EngineMain.h"

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

	void AssetManager::Unload(const std::string& filepath)
	{
		//Path 표기 방식 일원화.
		std::string path = Pg::Core::Helper::ResourceHelper::ForcePathUniform(filepath);

		//우선적으로 일단 리스트에 값이 있는지를 확인.
		if (this->_resources.contains(path))
		{
			//명시적으로 두 인스턴스에 모두 삭제(및 언로드)를 진행해준다. 
			//템플릿을 쓰지 않기 위해, 둘 다 삭제 코드, 내부적으로 해당하는지 아닌지 반환.

			///[TW] WORKINGONIT.
			//bool tIsEngineDelete = _engineResourceManager->DeleteResource(path);
			//bool tIsGraphicsDelete = _graphicsResourceManager->DeleteResource(path);
			//assert((tIsEngineDelete ^ tIsGraphicsDelete) && "하나만 삭제하는 경우가 발생하지 않음, 오류");
		
			//이제 메인 AssetManager에서 값을 삭제.
			_resources.erase(path);
		}
		else
		{
			//리스트에 값이 없다는 뜻이다. 언로드될 수도 없음.
			return;
		}
	}
}
