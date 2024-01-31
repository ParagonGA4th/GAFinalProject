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

		std::string _objName;
		std::string _objTag;
		bool _isActive;

		// 저장된 것은 quatrnion이지만, 보여지는 것은 float3
		Pg::Math::PGFLOAT3 _tempFloat3;
		Pg::Math::PGFLOAT4* _tempFloat4;
		Pg::Math::PGQuaternion* _tempQut;

		Pg::Data::GameObject* _object;

		Pg::Editor::Data::DataContainer* _dataContainer;

		std::unordered_map<std::string, bool*> _componentExistence;

		bool _isAddComponent;
	};
}