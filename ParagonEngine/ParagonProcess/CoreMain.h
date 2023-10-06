
#include "IEngine.h"
#include "IGraphics.h"
#include "IUtil.h"
#include "IAPI.h"
#include "TimeManager.h"
#include "../ParagonUtil/Log.h"

#include "../ParagonGameEngine/InputSystem.h"
#include "../ParagonGameEngine/WorkSpace.h"
#include <d3d11.h>

#include <memory>

//struct ID3D11Device;
//struct ID3D11DeviceContext;

/// <summary>
/// 코어
/// 2023. 9. 8. 변지상
/// 
/// ->
/// </summary>

namespace Pg::Core
{
	class Scene;
	class EngineGraphicsAdapter;
	class EditorAdapter;

	namespace Manager
	{
		class AssetManager;
	}
}

namespace Pg::Core
{
	class CoreMain
	{
		friend class Pg::Core::Manager::AssetManager;
	public:
		CoreMain();
		~CoreMain();

		long Initialize(void* hwnd, int screenWidth, int screenHeight);
		void Update();
		void BeginRender();    
		void Render();
		void EndRender();
		void Finalize();

	private:
		const WCHAR* _className;
		const WCHAR* _windowName;

	public:
		void OnWindowResized(int screenWidth, int screenHeight);
		ID3D11Device* GetGraphicsDevice();
		ID3D11DeviceContext* GetGraphicsDeviceContext();

		//Engine & Graphics <-> CoreMain 교류하는 어댑터 리턴.
		EngineGraphicsAdapter* GetEngineGraphicsAdapter();

		//Editor <-> CoreMain 교류하는 어댑터 리턴.
		EditorAdapter* GetEditorAdapter();

	private:
		std::unique_ptr<IUtil> _util;
		std::unique_ptr<IAPI> _api;

		Time::TimeManager* _timeManager;					//델타타임
		Manager::AssetManager* _assetManager;				//어셋매니저

		//임시 : WORKSPACE
		Pg::Engine::WorkSpace* _work = nullptr;

	private:
		std::unique_ptr<EngineGraphicsAdapter> _engineGraphicsAdapter;
		std::unique_ptr<EditorAdapter> _editorAdapter;
	};
}

