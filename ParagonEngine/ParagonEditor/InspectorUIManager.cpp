#include "InspectorUIManager.h"
#include "InspectorDataManager.h"
#include "DataContainer.h"
#include "Event.h"

#include "../ParagonData/GameObject.h"
#include "../ParagonData/ComponentList.h"
#include "../ParagonScript/Script.h"

#include "../ParagonUI/UIManager.h"
#include "../ParagonUI/WidgetContainer.h"
#include "../ParagonUI/Text.h"
#include "../ParagonUI/InputText.h"
#include "../ParagonUI/CheckBox.h"
#include "../ParagonUI/Button.h"
#include "../ParagonUI/Combo.h"
#include "../ParagonUI/DragFloat.h"
#include "../ParagonUI/DragFloat3.h"
#include "../ParagonUI/Selectable.h"
#include "../ParagonUI/Popup.h"
#include "../ParagonUI/InputInt.h"

#include "../ParagonUI/Column.h"
#include "../ParagonUI/Collaps.h"

#include <singleton-cpp/singleton.h>

Pg::Editor::Window::InspectorUIManager::InspectorUIManager()
{
	auto& tdcon = singleton<Pg::Editor::Data::DataContainer>();
	_dataContainer = &tdcon;

	_defaultUI = std::make_unique<Pg::UI::WidgetContainer>();
	_changedUI = std::make_unique<Pg::UI::WidgetContainer>();
	_assetList = std::make_unique<Pg::Editor::Event>();
}

Pg::Editor::Window::InspectorUIManager::~InspectorUIManager()
{

}

void Pg::Editor::Window::InspectorUIManager::Initialize(InspectorDataManager* manager, Pg::UI::Manager::UIManager* uimanager)
{
	_dataManager = manager;
	_uiManager = uimanager;

	_defaultUI->CreateColumnsWidget<Pg::UI::Widget::Text>("Name");
	_defaultUI->CreateColumnsWidget<Pg::UI::Widget::InputText>("Name", &_objName);

	_defaultUI->CreateColumnsWidget<Pg::UI::Widget::Text>("Tag");
	_defaultUI->CreateColumnsWidget<Pg::UI::Widget::InputText>("Tag", &_objTag);

	_defaultUI->CreateColumnsWidget<Pg::UI::Widget::Text>("Active");
	_defaultUI->CreateColumnsWidget<Pg::UI::Widget::CheckBox>("Active", &_isActive);

	_defaultUI->CreateColumnsWidget<Pg::UI::Widget::Text>("DontDestroy");
	_defaultUI->CreateColumnsWidget<Pg::UI::Widget::CheckBox>("DontDestroy", &_isDestroy);

	_defaultUI->CreateWidget<Pg::UI::Layout::Column<2>>("BasicInfo", _defaultUI->GetColumnWidgets());

	_defaultUI->ClearColumnWidget();

	_componentList.emplace_back(typeid(Pg::Data::Camera).name());
	_componentList.emplace_back(typeid(Pg::Data::AudioSource).name());

	_componentList.emplace_back(typeid(Pg::Data::BoxCollider).name());
	_componentList.emplace_back(typeid(Pg::Data::CapsuleCollider).name());
	_componentList.emplace_back(typeid(Pg::Data::SphereCollider).name());

	_componentList.emplace_back(typeid(Pg::Data::StaticBoxCollider).name());
	_componentList.emplace_back(typeid(Pg::Data::PlaneCollider).name());

	_componentList.emplace_back(typeid(Pg::Data::DirectionalLight).name());
	_componentList.emplace_back(typeid(Pg::Data::PointLight).name());
	_componentList.emplace_back(typeid(Pg::Data::SpotLight).name());

	_componentList.emplace_back(typeid(Pg::Data::TextRenderer).name());
	_componentList.emplace_back(typeid(Pg::Data::ImageRenderer).name());

	_componentList.emplace_back(typeid(Pg::Data::StaticMeshRenderer).name());
	_componentList.emplace_back(typeid(Pg::Data::SkinnedMeshRenderer).name());

	_componentList.emplace_back(typeid(Pg::Data::Button).name());
	_componentList.emplace_back(typeid(Pg::Data::Slider).name());

	_componentList.emplace_back(typeid(Pg::DataScript::Script).name()); 

	for (auto& script : Pg::DataScript::Script::get_registry())
	{
		std::string sub = script.first.substr(script.first.rfind("::") + 2);
		_scriptList.emplace_back(sub);
	}

	_defaultUI->CreateColumnsWidget<Pg::UI::Widget::Combo>("##Add Component", _componentList, _componentIndex);
	_defaultUI->CreateColumnsWidget<Pg::UI::Widget::Button>("Add Component", 115.f, 30.f, _isClick);
	_defaultUI->CreateWidget<Pg::UI::Layout::Column<2>>("AddComponent", _defaultUI->GetColumnWidgets(), 0);

	_defaultUI->CreateTempWidget<Pg::UI::Widget::Selectable>(_scriptList, _selectedScript);
	_defaultUI->CreateWidget<Pg::UI::Widget::Popup>("ScriptList", _defaultUI->GetTempWidgets());
}

void Pg::Editor::Window::InspectorUIManager::Update()
{
	SetData();
	_defaultUI->Update();
	_changedUI->Update();
	ChangedUI();
	UpdateData();
}

void Pg::Editor::Window::InspectorUIManager::ChangedUI()
{
	for (auto& object : _dataManager->_objectData)
	{
		if (object.first != _objName) continue;

		if (_prevObjName.empty() || _prevObjName != _objName)
		{
			_prevObjName = _objName;
			_changedUI->ClearWidget();

			for (auto& component : object.second)
			{
				/// component Name
				std::string comName;
				if (component.first.find("::") == std::string::npos) comName = component.first.substr(component.first.rfind("class") + 6);
				else comName = component.first.substr(component.first.rfind("::") + 2);

				if (_componentExistence.find(component.first) == _componentExistence.end()) _componentExistence.insert({ component.first , new bool(true) });
				else *_componentExistence.at(component.first) = true;

				_changedUI->ClearColumnWidget();
				_changedUI->ClearCollapsWidget();

				ColliderUI(comName);

				/// component Data
				for (auto& [valName, typeInfo, val] : component.second)
				{
					valName = valName.substr(valName.rfind("_") + 1);
					_changedUI->CreateColumnsWidget<Pg::UI::Widget::Text>(valName);

					if (typeInfo == typeid(bool).name())
						_changedUI->CreateColumnsWidget<Pg::UI::Widget::CheckBox>(valName, static_cast<bool*>(val));				
					
					if (typeInfo == typeid(int).name() || typeInfo == typeid(unsigned int).name())
						_changedUI->CreateColumnsWidget<Pg::UI::Widget::InputInt>(valName, static_cast<int*>(val));

					if (typeInfo == typeid(std::string).name())
					{
						if (!RendererUI(comName, valName, val))
							_changedUI->CreateColumnsWidget<Pg::UI::Widget::InputText>(valName, static_cast<std::string*>(val));
					}

					if (typeInfo == typeid(float).name())
						_changedUI->CreateColumnsWidget<Pg::UI::Widget::DragFloat>(valName, static_cast<float*>(val));

					if (typeInfo == typeid(Pg::Math::PGFLOAT3).name())
						_changedUI->CreateColumnsWidget<Pg::UI::Widget::DragFloat3>(valName, static_cast<Pg::Math::PGFLOAT3*>(val));

					if (typeInfo == typeid(Pg::Math::PGFLOAT4).name())
						_changedUI->CreateColumnsWidget<Pg::UI::Widget::DragFloat3>(valName, static_cast<Pg::Math::PGFLOAT4*>(val));

					if (typeInfo == typeid(Pg::Math::PGQuaternion).name())
						_changedUI->CreateColumnsWidget<Pg::UI::Widget::DragFloat3>(valName, static_cast<Pg::Math::PGQuaternion*>(val));
				}

				_changedUI->CreateCollapsWidget<Pg::UI::Layout::Column<2>>(comName, _changedUI->GetColumnWidgets());

				if (comName.find("StaticMeshRenderer") != std::string::npos)
					_changedUI->CreateCollapsWidget<Pg::UI::Widget::Button>("Refresh", 115.f, 30.f, _isRefresh);

				_changedUI->CreateWidget<Pg::UI::Layout::Collaps>
					(comName, _changedUI->GetCollapsWidgets(), _componentExistence.at(component.first));
			}
		}
	}
}

void Pg::Editor::Window::InspectorUIManager::SetData()
{
	_objName = _dataManager->_object->GetName();
	_objTag = _dataManager->_object->GetTag();
	_isActive = _dataManager->_object->GetActive();
	_isDestroy = _dataManager->_object->GetDontDestroyOnLoad();
}

void Pg::Editor::Window::InspectorUIManager::UpdateData()
{
	if (_isClick)
	{
		if (_componentList.at(_componentIndex) == typeid(Pg::DataScript::Script).name())
		{
			_uiManager->OpenPopup("ScriptList");
		}
		else
		{
			_dataManager->AddComponent(_componentList.at(_componentIndex));
			_componentExistence.insert({ _componentList.at(_componentIndex), new bool(true) });
		}
	}

	if (!_selectedScript.empty())
	{
		for (auto& iter : Pg::DataScript::Script::get_registry())
		{
			std::string iterSub = iter.first.substr(iter.first.rfind("::") + 2);
			if (iterSub.compare(_selectedScript) == 0)
			{
				_dataManager->_object->AddComponent(iter.first, iter.second(_dataManager->_object));
				_componentExistence.insert({ _componentList.at(_componentIndex), new bool(true) });
				_selectedScript = "";
			}
		}
	}

	for (auto& [name, val] : _componentExistence)
	{
		if (*val == false)
		{
			_dataManager->RemoveComponent(name);

			if (name.find("StaticMeshRenderer") != std::string::npos)
			{
				_dataManager->AddModifiedObject();
				_dataManager->ModifiedObject(true);
			}

			auto iter = _componentExistence.find(name);
			delete iter->second;
			_componentExistence.erase(iter);
			break;
		}
	}

	_dataManager->_object->SetName(_objName);
	_dataManager->_object->SetTag(_objTag);
	_dataManager->_object->SetActive(_isActive);
	_dataManager->_object->SetDontDestroyOnLoad(_isDestroy);

	if (_isRefresh)
		_dataManager->ModifiedObject(_isRefresh);
}

void Pg::Editor::Window::InspectorUIManager::ColliderUI(std::string comName)
{
	if (comName.find("Collider") != std::string::npos)
	{
		auto col = _dataManager->_object->GetComponent<Pg::Data::Collider>();

		if (_prevLayer != col->GetLayer()) _layer = col->GetLayer();
		if (_prevTrigger != col->GetTrigger()) _trigger = col->GetTrigger();
		if (_prevPosOffset != col->GetPositionOffset()) _posOffset = col->GetPositionOffset();
		if (_prevRotOffset != col->GetRotationOffset()) _rotOffset = col->GetRotationOffset();

		std::vector<std::string> layerType;
		layerType.emplace_back("LAYER_DEFAULT");
		layerType.emplace_back("LAYER_PLAYER");
		layerType.emplace_back("LAYER_MONSTER");
		layerType.emplace_back("LAYER_PROJECTILES");
		layerType.emplace_back("LAYER_MAP");
		layerType.emplace_back("LAYER_BACKGROUND");
		layerType.emplace_back("LAYER_MOVABLE_OBJECTS");
		layerType.emplace_back("LAYER_BOSS");

		_changedUI->CreateColumnsWidget<Pg::UI::Widget::Text>("layer");
		_changedUI->CreateColumnsWidget<Pg::UI::Widget::Combo>("##layer", layerType, _layer);
		
		_changedUI->CreateColumnsWidget<Pg::UI::Widget::Text>("trigger");
		_changedUI->CreateColumnsWidget<Pg::UI::Widget::CheckBox>("trigger", &_trigger);

		_changedUI->CreateColumnsWidget<Pg::UI::Widget::Text>("position Offset");
		_changedUI->CreateColumnsWidget<Pg::UI::Widget::DragFloat3>("position Offset", &_posOffset);
		_changedUI->CreateColumnsWidget<Pg::UI::Widget::Text>("rotation Offset");
		_changedUI->CreateColumnsWidget<Pg::UI::Widget::DragFloat3>("rotation Offset", &_rotOffset);

		if (_prevLayer != _layer) _prevLayer = _layer;
		if (_prevTrigger != _trigger) _prevTrigger = _trigger;
		if (_prevPosOffset != _posOffset) _prevPosOffset = _posOffset;
		if (_prevRotOffset != _rotOffset) _prevRotOffset = _rotOffset;
		
		col->SetLayer(_layer);
		col->SetTrigger(_trigger);
		col->SetPositionOffset(_posOffset);
		col->SetRotationOffset(_rotOffset);		
	}
}

bool Pg::Editor::Window::InspectorUIManager::RendererUI(std::string comName, std::string valName, void* val)
{
	bool isSpecial = false;

	if (comName.find("StaticMeshRenderer") != std::string::npos) isSpecial = true;
	else if (comName.find("SkinnedMeshRenderer") != std::string::npos) isSpecial = true;

	if (!isSpecial) return isSpecial;

	std::string* value = static_cast<std::string*>(val);

	if (valName == "meshName") _define = Pg::Data::Enums::eAssetDefine::_3DMODEL;
	else if (valName == "materialName") _define = Pg::Data::Enums::eAssetDefine::_RENDERMATERIAL;
	else if (valName == "initAnimName") _define = Pg::Data::Enums::eAssetDefine::_ANIMATION;

	_assetList->Invoke(eEventType::_ASSETLIST, static_cast<void*>(&_define));

	if (valName == "meshName")
	{
		if (*value != _dataContainer->GetAssetList().at(_prevNameIndex)) _meshNameIndex = _dataContainer->GetAssetIndex(*value);
		_changedUI->CreateColumnsWidget<Pg::UI::Widget::Combo>("##" + valName, _dataContainer->GetAssetList(), _meshNameIndex);

		if (_meshNameIndex != _prevNameIndex)
		{
			_dataManager->AddModifiedObject();
			_prevNameIndex = _meshNameIndex;
		}

		*value = _dataContainer->GetAssetList().at(_meshNameIndex);
	}
	else if (valName == "materialName")
	{
		if (*value != _dataContainer->GetAssetList().at(_prevMaterialIndex)) _meshMaterialIndex = _dataContainer->GetAssetIndex(*value);
		_changedUI->CreateColumnsWidget<Pg::UI::Widget::Combo>("##" + valName, _dataContainer->GetAssetList(), _meshMaterialIndex);

		if (_meshMaterialIndex != _prevMaterialIndex)
		{
			_dataManager->AddModifiedObject();
			_prevMaterialIndex = _meshMaterialIndex;
		}
		*value = _dataContainer->GetAssetList().at(_meshMaterialIndex);
	}
	else if (valName == "initAnimName")
	{
		if (*value != _dataContainer->GetAssetList().at(_animIndex)) _animIndex = _dataContainer->GetAssetIndex(*value);
		_changedUI->CreateColumnsWidget<Pg::UI::Widget::Combo>("##" + valName, _dataContainer->GetAssetList(), _animIndex);

		if (_animIndex != _prevAnimIndex)
		{
			_dataManager->AddModifiedObject();
			_prevAnimIndex = _animIndex;
		}

		*value = _dataContainer->GetAssetList().at(_animIndex);
	}

	return isSpecial;
}