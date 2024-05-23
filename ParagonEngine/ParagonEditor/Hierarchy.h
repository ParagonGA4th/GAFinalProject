#pragma once
#include "IEditorWindow.h"
#include <map>
#include <utility>
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

		virtual std::string GetWindowName() override;
	
		virtual void SetDisable(bool disable) override;
	private:
		void DataSet();
		void GetCurrentSceneObjectList();
		void GetSelectedObject();
		void IsObjectChanged(void* isChanged);

	private:
		/// hierarchy value
		std::string _winName;
		bool _isShow;
		bool _isDisable;

		std::string _prevSceneName;
		std::string _prevObjName;
		int _prevObjListSize;
		bool _isRefresh;
		bool _isSetData;
		int _count = 0;

		/// helper class
		Pg::Editor::Data::DataContainer* _dataContainer;
		Pg::UI::Manager::UIManager* _uiManager;
		std::unique_ptr<Pg::UI::WidgetContainer> _widgetCon;
		std::unique_ptr<Pg::Editor::Event> _changeObjectData;

		/// Data value
		// key - parent, value - child
		std::map<int, std::pair<std::string, std::vector<std::string>>> _objNameList;
		std::string* _currentObjName;
		bool* _isNewObject;
		bool* _isDeleteObject;
		bool* _isObjectChange;
	};
}

