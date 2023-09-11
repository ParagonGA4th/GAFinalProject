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

#include <windows.h>

namespace Pg::Core
{
	class CoreMain
	{
	public:
		CoreMain();

		long Initialize(void* hInstance, int cmdShow);
		void Update();
		void Finalize();

	private:

		//윈도우 관련 함수들
		ATOM RegisterClass(HINSTANCE hInstance);
		BOOL CreateWindows(HINSTANCE hInstance, int cmdShow);
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	private:
		HWND _hWnd;
		MSG _msg;
		int _screenwidth;
		int _screenheight;

		const WCHAR* _className;
		const WCHAR* _windowName;
	};
}

