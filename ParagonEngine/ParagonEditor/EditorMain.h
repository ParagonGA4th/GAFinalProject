#pragma once
#include "IEditor.h"
#include <windows.h>
#include <memory>

namespace Pg::Editor
{
	namespace Manager
	{
		class EditorManager;
	}

	class EditorMain : public IEditor 
	{
	public:
		 EditorMain();
		~EditorMain();

		virtual void Initialize() override;
		virtual void Run() override;
		virtual void Finalize() override;

	private:
		ATOM WindowRegisterClass(HINSTANCE hInstance);
		BOOL CreateWindows(HINSTANCE hInstance);
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	private:
		HWND _hWnd;

		float _screenWidth;
		float _screenHeight;

		const WCHAR* _appName;

	private:
		// WndProc에 접근 하기 위해 static 사용
		static Pg::Editor::Manager::EditorManager* _editorManager;
	};
}

