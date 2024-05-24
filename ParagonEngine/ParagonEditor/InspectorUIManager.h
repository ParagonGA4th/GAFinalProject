#pragma once
#include "../ParagonData/AssetDefines.h"
#include "../ParagonMath/PgMath.h"

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

		void Initialize(InspectorDataManager* manager, Pg::UI::Manager::UIManager* uimanager);
		void Update();

	private:
		void ChangedUI();
		void ColliderUI(std::string comName);
		bool RendererUI(std::string comName, std::string valName, void* val);
		void SetData();
		void UpdateData();

	private:
		/// Helper Class
		std::unique_ptr<Pg::UI::WidgetContainer> _defaultUI;
		std::unique_ptr<Pg::UI::WidgetContainer> _changedUI;
		Pg::Editor::Data::DataContainer* _dataContainer;
		InspectorDataManager* _dataManager;

		Pg::UI::Manager::UIManager* _uiManager;

		/// default UI value
		int _componentIndex;
		std::vector<std::string> _componentList;

		std::string _selectedScript;
		std::vector<std::string> _scriptList;
		
		bool _isClick;
		bool _isRefresh;

		/// Change UI Value
		Pg::Data::Enums::eAssetDefine _define;
		int _meshNameIndex;
		int _prevNameIndex;

		int _meshMaterialIndex;
		int _prevMaterialIndex;

		int _animIndex;
		int _prevAnimIndex;

		int _layer;
		int _prevLayer;
		bool _trigger;
		bool _prevTrigger;
		Pg::Math::PGFLOAT3 _posOffset;
		Pg::Math::PGFLOAT3 _prevPosOffset;
		Pg::Math::PGQuaternion _rotOffset;
		Pg::Math::PGQuaternion _prevRotOffset;

		/// object value;
		std::string _prevObjName;
		std::string _objName;
		std::string _objTag;
		bool _isActive;
		bool _isDestroy;

		/// Component UI Helper Value
		std::unordered_map<std::string, bool*> _componentExistence;

		/// Event
		std::unique_ptr<Pg::Editor::Event> _assetList;
	};
}


