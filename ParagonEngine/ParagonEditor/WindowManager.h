#pragma once
#include <windows.h>
#include <vector>

namespace Pg::Editor::Data { class DataContainer; }
namespace Pg::UI::Manager { class UIManager; }
namespace Pg::Editor::Window { class IEditorWindow; }

namespace Pg::Editor::Manager
{
	class WindowManager
	{
	public:
		WindowManager();
		~WindowManager();

		void Initialize(HWND hWnd);
		void Update();
		void Finalize();
		void WindowHandler(MSG message);

	private:
		Pg::UI::Manager::UIManager* _uiManager;
		Pg::Editor::Data::DataContainer* _dataContainer;
		std::vector<Pg::Editor::Window::IEditorWindow*> _windows;
	};
}

