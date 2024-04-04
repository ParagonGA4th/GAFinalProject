#pragma once
#include <string>
#include <memory>
#include <unordered_map>

namespace Pg::UI { class WidgetContainer; }
namespace Pg::Editor::Window
{
	class InspectorDataManager;
	class InspectorUIManager
	{
		friend class InspectorDataManager;
	public:
		InspectorUIManager();
		~InspectorUIManager();

		void Initialize(InspectorDataManager* manager);
		void Update();

	private:
		void ChangedUI();
		void SetData();
		void UpdateData();

	private:
		std::unique_ptr<Pg::UI::WidgetContainer> _defaultUI;
		std::unique_ptr<Pg::UI::WidgetContainer> _changedUI;

		/// default UI value
		std::vector<std::string> _componentList;
		int* _componentIndex;
		bool* _isClick;
		bool* _isRefresh;

		// object value;
		std::string _objName;
		std::string _objTag;
		bool _isActive;

		InspectorDataManager* _dataManager;

		// Component UI Helper Value
		std::unordered_map<std::string, bool*> _componentExistence;

	};
}


