#pragma once
#include "../ParagonMath/PgMath.h"

#include <string>
#include <unordered_map>
#include <memory>

namespace Pg::Data { class GameObject; }
namespace Pg::UI { class WidgetContainer; }
namespace Pg::Editor::Data { class DataContainer; }

namespace Pg::Editor::Window
{
	class InspectorHelper
	{
	public:
		InspectorHelper();
		~InspectorHelper();

		void Initialize();
		void Update();
		void SetData(Pg::Data::GameObject* object);

	private:
		void ComponentUI();

	private:
		std::unique_ptr<Pg::UI::WidgetContainer> _widgetCon;
		Pg::Editor::Data::DataContainer* _dataContainer;

		// Object Info
		Pg::Data::GameObject* _object;
		std::string _objName;
		std::string _objTag;
		bool _isActive;

		// Picking Object Info
		bool _isPicked;
		std::string _pickedObjName;

		// Component UI Helper Value
		std::unordered_map<std::string, bool*> _componentExistence;
	};
}