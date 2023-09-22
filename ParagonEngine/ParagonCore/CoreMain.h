#pragma once
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

#ifdef _DEBUG
#pragma comment(lib,"..\\x64\\Debug\\ParagonUtil.lib")
#else
#pragma comment(lib,"..\\x64\\Release\\ParagonUtil.lib")
#endif // _DEBUG

#ifdef _DEBUG
#pragma comment(lib,"..\\x64\\Debug\\ParagonAPI.lib")
#else
#pragma comment(lib,"..\\x64\\Release\\ParagonAPI.lib")
#endif // _DEBUG

#include "IEngine.h"
#include "IGraphics.h"
#include "IUtil.h"
#include "IAPI.h"
#include "TimeManager.h"
#include "../ParagonUtil/Log.h"

#include "../ParagonGameEngine/InputSystem.h"
#include "../ParagonGameEngine/WorkSpace.h"

#include <memory>

/// <summary>
/// 코어
/// 2023. 9. 8. 변지상
/// </summary>
namespace Pg::Core
{
	class CoreMain
	{
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

	private:
		
		std::unique_ptr<IEngine> _engine;					//게임 엔진
		std::unique_ptr<IGraphics> _graphics;				//그래픽스 엔진
		//std::unique_ptr<Pg::Util::Debug::Log> _logger;		//로거
		//Pg::Engine::Input::InputSystem* _inputSystem;
		std::unique_ptr<IUtil> _util;
		std::unique_ptr<IAPI> _api;

		Time::TimeManager* _timeManager;					//델타타임

		//임시 : WORKSPACE
		Pg::Engine::WorkSpace* _work = nullptr;

	public:
		ID3D11Device* GetGraphicsDevice();
		ID3D11DeviceContext* GetGraphicsDeviceContext();
	};
}

