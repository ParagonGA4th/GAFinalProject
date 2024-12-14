#include "InspectorDataManager.h"
#include "InspectorUIManager.h"
#include "DataContainer.h"
#include "Event.h"

#include "../ParagonData/Scene.h"
#include "../ParagonData/GameObject.h"

#include <visit_struct/visit_struct_intrusive.hpp>
#include <singleton-cpp/singleton.h>


Pg::Editor::Window::InspectorDataManager::InspectorDataManager()
{
	auto& tdcon = singleton<Pg::Editor::Data::DataContainer>();
	_dataContainer = &tdcon;

	_modifiedObject = std::make_unique<Pg::Editor::Event>();
}

Pg::Editor::Window::InspectorDataManager::~InspectorDataManager()
{
}

void Pg::Editor::Window::InspectorDataManager::Initialize(InspectorUIManager* manager)
{
	_uiManager = manager;
}

void Pg::Editor::Window::InspectorDataManager::SetFlag(int flag)
{
	_flag = flag;
}

void Pg::Editor::Window::InspectorDataManager::SetObject(Pg::Data::GameObject* object)
{
	_object = object;
	GetCurrentSceneObjects(true);

	//if (_dataContainer->GetPickObject() != nullptr)
	//	_pickedObjName = _dataContainer->GetPickObject()->GetName();
}

void Pg::Editor::Window::InspectorDataManager::Update()
{
	GetCurrentSceneObjects();
}


void Pg::Editor::Window::InspectorDataManager::GetCurrentSceneObjects(bool isRefresh)
{
	auto scene = _dataContainer->GetCurrentScene();
	
	if (_prevSceneName.empty() || _prevSceneName != scene->GetSceneName() || isRefresh)
	{
		_objectData.clear();
		_prevSceneName = scene->GetSceneName();

		for (auto& obj : scene->GetObjectList())
		{
			_objectData.insert({ obj->GetName(), {} });

			// 오브젝트를 가져오자마자 바로 Deserialize 해서 컴포넌트 데이터를 전부 빼낸다
			for (auto& component : obj->GetComponentList())
			{
				std::vector<std::tuple<std::string, std::string, void*>> tTempVec;

				component.second->OnDeserialize(tTempVec);
				_objectData[obj->GetName()].insert({ component.first, tTempVec });
			}
		}
	}
}

void Pg::Editor::Window::InspectorDataManager::AddComponent(std::string componentName)
{
	_object->AddComponent(componentName);
	AddModifiedObject();
	RefreshData(componentName, false);
}

void Pg::Editor::Window::InspectorDataManager::RemoveComponent(std::string componentName)
{
	_object->RemoveComponent(componentName);
	RefreshData(componentName, true);
}

void Pg::Editor::Window::InspectorDataManager::ModifiedObject(bool isModified)
{
	if (/*_flag == -1 || */isModified)
	{
		if (_modifiedObjList.empty()) return;

		_modifiedObject->Invoke(Pg::Editor::eEventType::_MODIFIEDOBJECT, static_cast<void*>(&_modifiedObjList));
		_modifiedObjList.clear();

		{		
			//std::vector<Pg::Data::GameObject*> tobjList(_modifiedObjList.size());
		//std::copy(_modifiedObjList.begin(), _modifiedObjList.end(), tobjList.begin());

		//tobjList.erase(std::remove_if(tobjList.begin(), tobjList.end(),
		//	[&](Pg::Data::GameObject* obj)
		//	{
		//		bool result = false;
		//		for (auto datas : _objectData)
		//		{
		//			if (obj->GetName() == datas.first)
		//			{
		//				for (auto data : datas.second)
		//				{
		//					std::string valueName = "";
		//					if (data.first.find("StaticMeshRenderer") != std::string::npos) valueName = "meshName";
		//					else continue;

		//					for (auto& [valName, typeInfo, val] : data.second)
		//					{
		//						if (valName == valueName)
		//						{
		//							if (typeInfo == typeid(std::string).name())
		//							{
		//								std::string temp = *(static_cast<std::string*>(val));
		//								result = temp.empty();
		//								break;
		//							}
		//						}
		//					}
		//				}
		//				break;
		//			}
		//		}
		//		return result;
		//	}), tobjList.end());

		//if (!tobjList.empty())
		//{
		//	// process로 넘기기 
		//	if (tobjList.size() != _modifiedObjList.size())
		//	{
		//		_modifiedObjList.clear();
		//		_modifiedObjList.resize(tobjList.size());
		//		std::copy(tobjList.begin(), tobjList.end(), _modifiedObjList.begin());
		//	}
		//}

		}
	}
}

void Pg::Editor::Window::InspectorDataManager::AddModifiedObject()
{
	if (!_modifiedObjList.empty())
	{
		auto tchObj = std::find_if(_modifiedObjList.begin(), _modifiedObjList.end(),
			[&](Pg::Data::GameObject* obj)
			{
				return obj->GetName() == _object->GetName();
			});

		if (tchObj != _modifiedObjList.end()) return;
	}

	_modifiedObjList.emplace_back(_object);
}

void Pg::Editor::Window::InspectorDataManager::RefreshData(std::string componentName, bool isRemove)
{
	if (isRemove)
	{
		_objectData[_object->GetName()].at(componentName).clear();
		_objectData[_object->GetName()].erase(componentName);
	}
	else
	{
		_objectData[_object->GetName()].insert({ componentName, {} });
	}

	for (auto component : _object->GetComponentList())
	{
		std::vector<std::tuple<std::string, std::string, void*>> tTempVec;

		component.second->OnDeserialize(tTempVec);

		_objectData[_object->GetName()].at(component.first).clear();
		_objectData[_object->GetName()].at(component.first) = tTempVec;
	}
}
