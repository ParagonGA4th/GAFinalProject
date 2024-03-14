#pragma once
#include "IEditorWindow.h"
#include <memory>

namespace Pg::Editor::Data { class DataContainer; }
namespace Pg::UI { class WidgetContainer; }
namespace Pg::UI::Manager { class UIManager; }

namespace Pg::Editor::Window
{
	class Game : public IEditorWindow
	{
	public:
		Game();
		~Game();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Finalize() override;


		virtual void SetShow(bool show) override;
		virtual bool GetShow() override;

		virtual std::string GetWindowName() override;

		virtual void SetDisable(bool disable) override;

	private:
		/// Game Value
		std::string _winName;
		bool _isShow;
		bool _isDisable;

		/// helper Class
		Pg::Editor::Data::DataContainer* _dataContainer;
		Pg::UI::Manager::UIManager* _uiManager;
		std::unique_ptr<Pg::UI::WidgetContainer> _widgetCon;

		/// Data Value
	};
}

