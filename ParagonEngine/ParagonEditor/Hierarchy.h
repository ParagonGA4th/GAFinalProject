#pragma once
#include "IEditorWindow.h"
#include <string>

namespace Pg::UI { class WidgetContainer; }
namespace Pg::UI::Manager { class UIManager; }

namespace Pg::Editor::Window
{
	class Hierarchy : public IEditorWindow
	{
	public:
		Hierarchy();
		~Hierarchy();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Finalize() override;

		virtual void SetShow(bool show) override;
		virtual bool GetShow() override;

	private:

	private:
		/// inspector value
		std::string _winName;
		bool _isShow;

		/// helper class
		Pg::UI::Manager::UIManager* _uiManager;
		Pg::UI::WidgetContainer* cons;

		/// Data value
	};
}

