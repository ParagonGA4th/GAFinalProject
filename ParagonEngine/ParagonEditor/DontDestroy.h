#pragma once
#include "IEditorWindow.h"
#include <vector>
#include <memory>

namespace Pg::UI { class WidgetContainer; }
namespace Pg::UI::Manager { class UIManager; }
namespace Pg::Editor::Data { class DataContainer; }

namespace Pg::Editor::Window
{
	class DontDestroy : public IEditorWindow
	{
	public:
		DontDestroy();
		~DontDestroy();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Finalize() override;

		virtual void SetShow(bool show) override;
		virtual bool GetShow() override;

		virtual std::string GetWindowName() override;

		virtual void SetDisable(bool disable) override;

	private:
		void DataSet();

	private:
		/// DontDestroy Value
		std::string _winName;
		bool _isShow;
		bool _isDisable;

		/// helper class
		Pg::Editor::Data::DataContainer* _dataContainer;
		Pg::UI::Manager::UIManager* _uiManager;
		std::unique_ptr<Pg::UI::WidgetContainer> _widgetCon;

		/// Data Value
		std::vector<std::string> _dontDestroyObjList;
		std::string _currentSceneName;
		std::string _selectedName;
	};
}