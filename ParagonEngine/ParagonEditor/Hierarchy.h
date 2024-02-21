#pragma once
#include "IEditorWindow.h"
#include <string>
#include <vector>
#include <memory>

namespace Pg::Editor { class Event; }
namespace Pg::Editor::Data { class DataContainer; }
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
		void DataSet();

	private:
		/// hierarchy value
		std::string _winName;
		bool _isShow;

		/// helper class
		Pg::Editor::Data::DataContainer* _dataContainer;
		Pg::UI::Manager::UIManager* _uiManager;
		std::unique_ptr<Pg::UI::WidgetContainer> _widgetCon;

		std::unique_ptr<Pg::Editor::Event> _changeObjectData;

		/// Data value
		std::vector<std::string> _objNames;
		int* _selectedNumber;
		int _prevObjListSize;

		std::string _prevObjName;
		std::string _pickedObjName;
		bool _isPicked;
	};
}

