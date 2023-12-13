#pragma once
#include <windows.h>
#include <memory>

namespace Pg::Editor::Data { class DataContainer; }
namespace Pg::Editor::Manager
{
	class WindowManager;
	class EditorManager
	{
	public:
		EditorManager();
		~EditorManager();
		
		void Initialize(HWND hWnd);
		void Update();
		void Finalize();

		void WindowHandler(MSG message);

	private:
		Pg::Editor::Data::DataContainer* _dataContainer;
		std::unique_ptr<Pg::Editor::Manager::WindowManager> _windowManager;
	};
}


