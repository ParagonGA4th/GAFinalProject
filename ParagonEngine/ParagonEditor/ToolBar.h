#pragma once
#include "IEditorWindow.h"
#include "../ParagonData/EditorMode.h"

#include <memory>

namespace Pg::Editor { class Event; }
namespace Pg::UI { class WidgetContainer; }
namespace Pg::UI::Manager { class UIManager; }

namespace Pg::Editor::Window
{
	class ToolBar : public IEditorWindow
	{
	public:
		ToolBar();
		~ToolBar();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Finalize() override;

		virtual void SetShow(bool show) override;
		virtual bool GetShow() override;

		virtual std::string GetWindowName() override;

		virtual void SetDisable(bool disable) override;

	private:
		/// ToolBar value
		std::string _winName;
		bool _isShow;

		/// helper class
		Pg::UI::Manager::UIManager* _uiManager;
		std::unique_ptr<Pg::UI::WidgetContainer> _widgetCon;
		std::unique_ptr<Pg::Editor::Event> _editorManaged;

		Pg::Data::Enums::eEditorMode _editorModeType;

		/// Data value
		bool _isStartBtnClick;
		bool _isStopBtnClick;

		std::string _gizmoType = "Translate";
		std::string _prevGizmoType;
	};
}

