#pragma once
#include <windows.h>
#include <memory>
#include <vector>
#include <string>

namespace Pg::Editor::Manager { class IEditorManager; }
namespace Pg::Editor::System { class FileSystem; }
namespace Pg::Editor { class Event; }

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
		void SetCustomCursor();

	private:
		HWND _hWnd;
		MSG _msg;
		static HCURSOR _cursor;

		float _screenWidth;
		float _screenHeight;

		//const WCHAR* _appName;
		const std::wstring _appName;
	private:
		std::vector<Pg::Editor::Manager::IEditorManager*> _editorManagers;
		std::unique_ptr<Pg::Editor::System::FileSystem> _fileSystem;
		std::unique_ptr<Pg::Editor::Event> _editorEvent;
	};
}