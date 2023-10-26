#pragma once
#include "IEditorWindow.h"
#include <string>

#include "../ParagonUI/WidgetContainer.h"

namespace Pg::UI::Manager { class UIManager; }
namespace Pg::Editor::Window
{
	class Inspector : public IEditorWindow
	{
	public:
		Inspector();
		~Inspector();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Finalize() override;

		virtual void SetShow(bool show) override;
		virtual bool GetShow() override;	

	private:

	private:
		std::string _winName;
		bool _isShow;

		Pg::UI::Manager::UIManager* _uiManager;
		Pg::UI::WidgetContainer cons;

		std::string _objName;
		std::string _objTag;
	};
}


