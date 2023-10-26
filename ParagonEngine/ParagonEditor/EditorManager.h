#pragma once
#include <windows.h>
#include <string>
#include <memory>

namespace Pg::Editor::Helper { class EditorHelper; }
namespace Pg::Editor::Window { class Inspector; }
namespace Pg::UI::Manager { class UIManager; }

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
		std::unique_ptr<Pg::Editor::Window::Inspector> _inspector;

		bool _editorOnOff;

		std::string str;
	};
}


