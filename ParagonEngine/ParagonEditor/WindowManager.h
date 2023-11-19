#pragma once
#include <windows.h>
#include <memory>

namespace Pg::Editor::Data { class DataContainer; }
namespace Pg::UI::Manager { class UIManager; }
namespace Pg::Editor::Window
{
	class Inspector;
	class Hierarchy;
	class Scene;
	class Filter;
}

namespace Pg::Editor::Manager
{
	class WindowManager
	{
	public:
		WindowManager();
		~WindowManager();

		void Initialize(HWND hWnd);
		void Update();
		void LastUpdate();
		void Finalize();
		void WindowHandler(MSG message);

	private:
		Pg::UI::Manager::UIManager* _uiManager;
		Pg::Editor::Data::DataContainer* _dataContainer;

		// Editor Windows
		std::unique_ptr<Pg::Editor::Window::Inspector> _inspector;
		std::unique_ptr<Pg::Editor::Window::Hierarchy> _hierarchy;
		std::unique_ptr<Pg::Editor::Window::Scene> _scene;
		std::unique_ptr<Pg::Editor::Window::Filter> _filter;
	};
}

