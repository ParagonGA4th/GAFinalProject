#pragma once
#include <windows.h>
#include <string>
#include <memory>

namespace Pg::UI::Manager { class UIManager; }
namespace Pg::Editor::Helper { class EditorHelper; }
namespace Pg::Editor::Window
{
	class Inspector; 
	class Hierarchy; 
	class Scene;
	class Filter;
}

namespace Pg::Editor::Manager
{
	class EditorManager
	{
	public:
		EditorManager();
		~EditorManager();
		
		void Initialize(HWND hWnd);
		void Update();
		void LastUpdate();		
		void Finalize();

		void UIHandler(MSG message);

	private:
		Pg::UI::Manager::UIManager* _uiManager;
		Pg::Editor::Helper::EditorHelper* _edHepler;

		// Editor Windows
		std::unique_ptr<Pg::Editor::Window::Inspector> _inspector;
		std::unique_ptr<Pg::Editor::Window::Hierarchy> _hierarchy;
		std::unique_ptr<Pg::Editor::Window::Scene> _scene;
		std::unique_ptr<Pg::Editor::Window::Filter> _filter;

		bool _editorOnOff;

		std::string str;
	};
}


