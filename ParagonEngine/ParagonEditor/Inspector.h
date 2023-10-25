#pragma once
#include "IEditorWindow.h"
#include <string>

namespace Pg::UI::Manager { class UIManager; }
namespace Pg::UI { class Panel; }

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
		void CreateUI();

	private:
		std::string _winName;
		bool _isShow;

		Pg::UI::Manager::UIManager* _uiManager;
		Pg::UI::Panel* _panel;
	};
}


