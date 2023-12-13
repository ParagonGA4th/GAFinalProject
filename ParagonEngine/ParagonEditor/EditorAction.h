#pragma once
#include <windows.h>
#include <memory>

namespace Pg::Editor::Manager
{
	class EditorManager;
	class ProcessManager;
	class FileManager;
}

namespace Pg::Editor
{
	class Event;
}

namespace Pg::Editor::Core
{
	class EditorAction
	{
	public:
		EditorAction();
		~EditorAction();

		void Initialize();
		void Loop();
		void Finalize();

	private:
		ATOM WindowRegisterClass(HINSTANCE hInstance);
		BOOL CreateWindows(HINSTANCE hInstance);
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	private:
		HWND _hWnd;
		MSG _msg;

		float _screenWidth;
		float _screenHeight;

		const WCHAR* _appName;
	private:
		// WndProc에 접근 하기 위해 static 사용
		std::unique_ptr<Pg::Editor::Manager::EditorManager> _editorManager;
		std::unique_ptr<Pg::Editor::Manager::ProcessManager> _processManager;
		std::unique_ptr<Pg::Editor::Manager::FileManager> _fileManager;

		std::unique_ptr < Pg::Editor::Event> _editorEvent;
	};
}