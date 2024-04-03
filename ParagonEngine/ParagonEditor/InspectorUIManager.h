#pragma once
#include <string>
#include <unordered_map>
#include <memory>

namespace Pg::UI { class WidgetContainer; }
namespace Pg::Editor::Window
{
	class InspectorUIManager
	{
	public:
		InspectorUIManager();
		~InspectorUIManager();

		void Initialize();
		void Update();
		//void SetData(Pg::Data::GameObject* object);

	private:
		//void ComponentUI();

	private:
		std::unique_ptr<Pg::UI::WidgetContainer> _defaultUI;
		std::unique_ptr<Pg::UI::WidgetContainer> _changedUI;
		//Pg::Editor::Data::DataContainer* _dataContainer;

		/// default UI value
		std::vector<std::string> _componentList;
		int* _componentIndex;
		bool* _isClick;

		// Object Info
		//Pg::Data::GameObject* _object;
		//std::string _objName;
		//std::string _objTag;
		//bool _isActive;

		// Picking Object Info
		//bool _isPicked;
		//std::string _pickedObjName;

		// Component UI Helper Value
		//std::unordered_map<std::string, bool*> _componentExistence;

	};
}


