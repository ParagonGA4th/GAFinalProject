#pragma once
#include <windows.h>

namespace Pg::Editor
{
	class IEditor;
}

namespace Pg::Client::Core
{
	class Application
	{
	public:
		Application();
		~Application();

		void Initialize(HINSTANCE hInstance);
		void Finalize();

		void Run();
	private:
		ATOM RegisterClass(HINSTANCE hInstance);
		BOOL CreateWindows(HINSTANCE hInstance);
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	private:
		HWND _hWnd;

		float _screenWidth;
		float _screenHeight;

		const WCHAR* _appName;

	private:
		Pg::Editor::IEditor* _editor;
	};
}

