#pragma once
#include "IEditorWindow.h"
#include <string>
#include <memory>

namespace Pg::Editor { class Event; }
namespace Pg::UI::Manager { class UIManager; }

namespace Pg::Editor::Window
{
	class Layout : public IEditorWindow
	{
	public:
		Layout();
		~Layout();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Finalize() override;

		virtual void SetShow(bool show) override;
		virtual bool GetShow() override;

	private:
		/// Layout value
		std::string _winName;
		bool _isShow;

		/// helper class
		Pg::UI::Manager::UIManager* _uiManager;
	};
}

