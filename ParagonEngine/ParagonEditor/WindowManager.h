#pragma once
#include <windows.h>
#include <vector>

namespace Pg::Editor { class Event; }
namespace Pg::Editor::Data { class DataContainer; }
namespace Pg::Editor::Window { class IEditorWindow; }
namespace Pg::UI::Manager { class UIManager; }

namespace Pg::Editor::Manager
{
	class WindowManager
	{
	public:
		WindowManager();
		~WindowManager();

		void Initialize(void* hWnd);
		void Update();
		void Finalize();
		void WindowHandler(MSG message);

	private:
		void WindowAble(void* disable);

	private:
		Pg::UI::Manager::UIManager* _uiManager;
		Pg::Editor::Data::DataContainer* _dataContainer;
		std::vector<Pg::Editor::Window::IEditorWindow*> _windows;

		bool _isDisable;
	};
}

