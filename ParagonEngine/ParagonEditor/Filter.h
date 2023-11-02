#pragma once
#include "IEditorWindow.h"
#include <string>

namespace Pg::UI { class WidgetContainer; }
namespace Pg::UI::Manager { class UIManager; }

namespace Pg::Editor::Window
{
	class Filter : public IEditorWindow
	{
	public:
		Filter();
		~Filter();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Finalize() override;
		
		virtual void SetShow(bool show) override;
		virtual bool GetShow() override;

	private:
		void TestFilter(std::string path);

	private:
		/// filter Value
		std::string _winName;
		bool _isShow;

		/// helper class
		Pg::UI::Manager::UIManager* _uiManager;
		Pg::UI::WidgetContainer* cons;

		/// Data Value
		std::string _firstPath;
	};
}


