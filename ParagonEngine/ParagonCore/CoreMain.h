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

#include "IEngine.h"
#include "../ParagonUtil/Log.h"
#include <memory>

namespace Pg::Core
{
	class CoreMain
	{
	public:
		CoreMain();
		~CoreMain();

		long Initialize(int screenWidth, int screenHeight);
		void Update();
		void BeginRender();    
		void Render();
		void EndRender();
		void Finalize();

	private:
		const WCHAR* _className;
		const WCHAR* _windowName;

	private:
		
		std::unique_ptr<IEngine> _engine;				//게임 엔진
		std::unique_ptr<IEngine> _graphics;				//그래픽스 엔진
		std::unique_ptr<Pg::Util::Debug::Log> _logger;
	};
}

