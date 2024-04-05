#pragma once
#include "../ParagonData/AssetDefines.h"

#include <string>
#include <memory>
#include <unordered_map>

namespace Pg::UI { class WidgetContainer; }
namespace Pg::Editor { class Event; }
namespace Pg::Editor::Data { class DataContainer; }

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
		/// Helper Class
		std::unique_ptr<Pg::UI::WidgetContainer> _defaultUI;
		std::unique_ptr<Pg::UI::WidgetContainer> _changedUI;
		Pg::Editor::Data::DataContainer* _dataContainer;
		InspectorDataManager* _dataManager;

		/// default UI value
		std::vector<std::string> _componentList;
		int _componentIndex;
		bool _isClick;
		bool _isRefresh;

		/// Change UI Value
		Pg::Data::Enums::eAssetDefine _define;
		int _staticMeshNameIndex;
		int _prevNameIndex;

		int _staticMeshMaterialIndex;
		int _prevMaterialIndex;

		/// object value;
		std::string _objName;
		std::string _objTag;
		bool _isActive;

		/// Component UI Helper Value
		std::unordered_map<std::string, bool*> _componentExistence;

		/// Event
		std::unique_ptr<Pg::Editor::Event> _assetList;
	};
}


