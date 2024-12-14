#include "DataManager.h"
#include "Serializer.h"

#include "../ParagonData/Scene.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/Component.h"
#include "../ParagonData/Collider.h"
#include "../ParagonData/Button.h"
#include "../ParagonData/Handle.h"
#include "../ParagonData/Slider.h"
#include "../ParagonScript/FactoryHelper.h"
#include "../ParagonScript/Script.h"
#include "../ParagonHelper/UUIDGenerator.h"
#include "../ParagonData/DynamicCollider.h"

#include <sstream>
#include <algorithm>
#include <singleton-cpp/singleton.h>
#include <visit_struct/visit_struct_intrusive.hpp>


#include "../ParagonUtil/Log.h"

Pg::Editor::Manager::DataManager::DataManager()
{
	auto& tdataCon = singleton<Pg::Editor::Data::DataContainer>();
	_dataContainer = &tdataCon;

	//FactoryHelper의 Initialize 역시 해당 단계에서 수행된다.
	Pg::DataScript::FactoryHelper::Initialize();
}

Pg::Editor::Manager::DataManager::~DataManager()
{}

void Pg::Editor::Manager::DataManager::DataLoad(std::string path, bool isScene)
{
	if (isScene)
	{
		if (_dataContainer->GetSceneList().empty())
		{
			SceneLoad(path);
			if (_scenes.size() > 0) _dataContainer->SetSceneList(_scenes);
		}
		else
		{
			bool isFind = false;
			for (auto& scene : _scenes)
			{
				std::string fileName = path.substr(path.rfind("\\") + 1);
				fileName = fileName.substr(0, fileName.rfind("."));

				if (fileName == scene->GetSceneName())
				{
					_dataContainer->SetCurrentScene(scene->GetSceneName());
					isFind = true;
					break;
				}
			}

			if (!isFind)
			{
				SceneLoad(path);
				_dataContainer->SetSceneList(_scenes);
				_dataContainer->SetCurrentScene(_scenes.at(0));
			}
		}
	}
	else
	{
		_scenes.clear();

		ProjectLoad(path);
		if (_scenes.size() > 0) _dataContainer->SetSceneList(_scenes, false);
	}
}

std::unordered_map<std::string, std::string> Pg::Editor::Manager::DataManager::DataSave(bool isScene)
{
	_sceneSerializeData.clear();

	// Data를 가져와서 Serialize
	SceneSave(isScene);
	if (!isScene) ProjectSave();

	return _sceneSerializeData;
}

std::unordered_map<std::string, std::string> Pg::Editor::Manager::DataManager::DataCreate(bool isScene)
{
	_sceneSerializeData.clear();

	std::string sceneName = "Sample Scene";
	std::string docToString;
	std::stringstream ss;

	if (!isScene)
	{
		// project 생성
		pugi::xml_document proejctDoc;
		proejctDoc.append_child("proejct");

		pugi::xml_node scenesNode = proejctDoc.child("proejct").append_child("scenes");
		Pg::Serialize::Serializer::SerializeString(&scenesNode, "scene", sceneName);

		proejctDoc.save(ss, "\t");
		docToString = ss.str();
		_sceneSerializeData.insert({ "project", docToString });
	}
	else
	{
		if (!_dataContainer->GetSceneList().empty())
		{
			int newSceneNum = 0;

			for (auto scene : _dataContainer->GetSceneList())
			{
				if (scene->GetSceneName().find("New Scene") != std::string::npos)
				{
					try
					{
						newSceneNum = std::stoi(scene->GetSceneName());
					}
					catch (const std::invalid_argument&)
					{
						newSceneNum++;
						continue;
					}
				}
			}

			if (newSceneNum != 0) sceneName = "New Scene " + std::to_string(newSceneNum);
			else sceneName = "New Scene";
		}
	}

	// scene 생성
	{
		pugi::xml_document doc;

		pugi::xml_node declarationNode = doc.prepend_child(pugi::node_declaration);
		declarationNode.append_attribute("version") = "1.0";
		declarationNode.append_attribute("encoding") = "utf-8";

		doc.append_child("scene");

		doc.save(ss, "\t"); // save 함수를 사용하여 스트림에 XML을 저장
		docToString = ss.str();

		_sceneSerializeData.insert({ sceneName, docToString });
	}

	return _sceneSerializeData;
}


void Pg::Editor::Manager::DataManager::ProjectLoad(std::string path)
{
	pugi::xml_document doc;

	if (doc.load_file(path.c_str()))
	{
		pugi::xml_node rootNode = doc.first_child();
		for (pugi::xml_node scene = rootNode.first_child().first_child(); scene; scene = scene.next_sibling())
		{
			_dataContainer->SetProjectPath(path.substr(0, path.rfind("\\") + 1));

			std::string scenePath = path.substr(0, path.rfind("\\") + 1).append("Asset\\Scene\\");
			scenePath.append(scene.text().as_string()).append(".pgscene");

			SceneLoad(scenePath);
		}
	}
}

void Pg::Editor::Manager::DataManager::SceneLoad(std::string path)
{
	pugi::xml_document doc;
	if (doc.load_file(path.c_str()))
	{
		std::string sceneName = path.substr(path.rfind("\\") + 1);
		sceneName = sceneName.substr(0, sceneName.rfind("."));

		Pg::Data::Scene* newScene = new Pg::Data::Scene(sceneName);
		_scenes.push_back(newScene);

		pugi::xml_node rootNode = doc.child("scene");
		newScene->SetIs3D(Pg::Serialize::Serializer::DeserializeBoolean(&rootNode, "is3D"));
		DataDeserialize(rootNode.child("objects"), _scenes.size() - 1);

		if (!_existsParentObject.empty())
		{
			for (auto& obj : newScene->GetObjectList())
			{
				for (auto& childObj : _existsParentObject)
				{
					if (obj->GetName() == childObj.second)
						obj->_transform.AddChild(childObj.first);
				}
			}

			_existsParentObject.clear();
		}
	}
}

void Pg::Editor::Manager::DataManager::ProjectSave()
{
	pugi::xml_document proejctDoc;
	proejctDoc.append_child("proejct");
	pugi::xml_node scenesNode = proejctDoc.child("proejct").append_child("scenes");

	for (auto& scene : _sceneSerializeData)
	{
		Pg::Serialize::Serializer::SerializeString(&scenesNode, "scene", scene.first);
	}

	std::stringstream ss;
	proejctDoc.save(ss, "\t"); // save 함수를 사용하여 스트림에 XML을 저장

	std::string docToString = ss.str();

	_sceneSerializeData.insert({ "project", docToString });
}

void Pg::Editor::Manager::DataManager::SceneSave(bool isSceneSave)
{
	if (isSceneSave)
	{
		pugi::xml_document doc;

		pugi::xml_node declarationNode = doc.prepend_child(pugi::node_declaration);
		declarationNode.append_attribute("version") = "1.0";
		declarationNode.append_attribute("encoding") = "utf-8";

		doc.append_child("scene");
		pugi::xml_node scene_node = doc.child("scene");
		Pg::Serialize::Serializer::SerializeBoolean(&scene_node, "is3D", _dataContainer->GetCurrentScene()->GetIs3D());

		pugi::xml_node node = doc.child("scene").append_child("objects");

		DataSerialize(node, _dataContainer->GetCurrentScene());

		std::stringstream ss;
		doc.save(ss, "\t"); // save 함수를 사용하여 스트림에 XML을 저장

		std::string docToString = ss.str();

		_sceneSerializeData.insert({ _dataContainer->GetCurrentScene()->GetSceneName(), docToString });

		std::string sceneName = _dataContainer->GetCurrentScene()->GetSceneName().substr(0, _dataContainer->GetCurrentScene()->GetSceneName().rfind("."));
	}
	else
	{
		for (auto& scene : _dataContainer->GetSceneList())
		{
			pugi::xml_document doc;

			pugi::xml_node declarationNode = doc.prepend_child(pugi::node_declaration);
			declarationNode.append_attribute("version") = "1.0";
			declarationNode.append_attribute("encoding") = "utf-8";

			doc.append_child("scene");
			pugi::xml_node scene_node = doc.child("scene");
			Pg::Serialize::Serializer::SerializeBoolean(&scene_node, "is3D", scene->GetIs3D());

			pugi::xml_node node = doc.child("scene").append_child("objects");

			DataSerialize(node, scene);

			std::stringstream ss;
			doc.save(ss, "\t"); // save 함수를 사용하여 스트림에 XML을 저장

			std::string docToString = ss.str();

			_sceneSerializeData.insert({ scene->GetSceneName(), docToString });

			std::string sceneName = scene->GetSceneName().substr(0, scene->GetSceneName().rfind("."));

		}
	}
}


void Pg::Editor::Manager::DataManager::DataDeserialize(pugi::xml_node root, int sceneNum)
{
	//ScenesData scenesData;
	// <objects>를 순회 하며 pgscene에 있는 object를 가져온다
	for (pugi::xml_node object = root.first_child(); object; object = object.next_sibling())
	{
		// GameObject 생성
		Pg::Data::GameObject* obj = nullptr;
		obj = _scenes.at(sceneNum)->AddObject("New Object");

		obj->SetName(Pg::Serialize::Serializer::DeserializeString(&object, "name"));
		obj->SetActive(Pg::Serialize::Serializer::DeserializeBoolean(&object, "active"));
		obj->SetTag(Pg::Serialize::Serializer::DeserializeString(&object, "tag"));
		obj->SetDontDestroyOnLoad(Pg::Serialize::Serializer::DeserializeBoolean(&object, "dontdestroy"));

		bool parent = Pg::Serialize::Serializer::DeserializeBoolean(&object, "parent");
		std::string parent_name = Pg::Serialize::Serializer::DeserializeString(&object, "parent_name");

		if (parent_name.empty()) parent = false;
		if (parent) _existsParentObject.insert({ obj, parent_name });

		// 컴포넌트를 추가하기 위해 노드 가져오기
		pugi::xml_node comps = object.find_node([](const pugi::xml_node& node) { return std::string(node.name()) == "components"; });

		// insepector에서 쓰이게 될 오브젝트 데이터 덩어리
		for (pugi::xml_node component = comps.first_child(); component; component = component.next_sibling())
		{
			std::vector<std::tuple<std::string, std::string, void*>> tSerVec;
			std::string typeName = Pg::Serialize::Serializer::DeserializeString(&component, "type");

			if (!typeName.empty())
			{
				if (typeName.find("Transform") != std::string::npos)
				{
					obj->_transform.OnDeserialize(tSerVec);
				}
				else
				{
					//auto component = AddComponentToObject(typeName, obj);
					auto componentData = obj->AddComponent(typeName);
					if (componentData != nullptr)
					{
						componentData->OnDeserialize(tSerVec);

						if (typeName.find("Collider") != std::string::npos)
						{
							auto dyCol = obj->GetComponent<Pg::Data::DynamicCollider>();
							if (dyCol != nullptr)
							{
								pugi::xml_node dyNode = component.find_node([&](const pugi::xml_node& node) { return std::string(node.name()) == "kinematic"; });
								if (dyNode != nullptr)
								{
									dyCol->SetKinematic(Pg::Serialize::Serializer::DeserializeBoolean(&dyNode, ""));
								}
							}

							auto col = obj->GetComponent<Pg::Data::Collider>();
							pugi::xml_node node = component.find_node([&](const pugi::xml_node& node) { return std::string(node.name()) == "layer"; });
							if (node != nullptr)
							{
								col->SetLayer(Pg::Serialize::Serializer::DeserializeUint(&node, ""));
								node = node.next_sibling();
							}
							else
							{
								node = component.find_node([&](const pugi::xml_node& node) { return std::string(node.name()) == "trigger"; });
							}

							col->SetTrigger(Pg::Serialize::Serializer::DeserializeBoolean(&node, ""));

							node = node.next_sibling();
							col->SetPositionOffset(Pg::Serialize::Serializer::DeserializePGFloat3(&node));

							node = node.next_sibling();
							col->SetRotationOffset(Pg::Serialize::Serializer::DeserializePGQuaternion(&node));
						}
					}
					else
					{
						Pg::DataScript::FactoryHelper::AddScript(obj, typeName);
						if (typeName.find("Behaviour") != std::string::npos ||
							typeName.find("Area") != std::string::npos ||
							typeName.find("InGameCamera") != std::string::npos ||
							typeName.find("Attack") != std::string::npos ||
							typeName.find("Mimic") != std::string::npos || 
							typeName.find("ArrowLogic") != std::string::npos ||
							typeName.find("SetInGameSortingLayer") != std::string::npos)
						{
							auto scriptData = obj->GetComponent<Pg::DataScript::Script>();
							if (scriptData != nullptr)
							{
								scriptData->OnDeserialize(tSerVec);
							}
						}
					}
				}

				for (auto& [valName, typeInfo, val] : tSerVec)
				{
					pugi::xml_node node = component.find_node(
						[&](const pugi::xml_node& node)
						{
							valName = valName.substr(valName.rfind("_") + 1);
							return std::string(node.name()) == valName;
						});
					Pg::Serialize::Serializer::Deserialize(typeInfo, &node, val);
				}
			}
		}
	}
}

void Pg::Editor::Manager::DataManager::DataSerialize(pugi::xml_node node, Pg::Data::Scene* scene)
{
	// scene node 안에 objects
	// objects node 안에 object
	// 각 component에 맞는 serialize 형식 필요

	for (auto& object : scene->GetObjectList())
	{
		if (object->GetName().compare("EditorCamera") == 0) continue;

		pugi::xml_node xmlObject = node.append_child("object");

		Pg::Serialize::Serializer::SerializeString(&xmlObject, "name", object->GetName());
		Pg::Serialize::Serializer::SerializeBoolean(&xmlObject, "active", object->GetActive());
		Pg::Serialize::Serializer::SerializeString(&xmlObject, "tag", object->GetTag());
		Pg::Serialize::Serializer::SerializeBoolean(&xmlObject, "dontdestroy", object->GetDontDestroyOnLoad());

		if (object->_transform.GetParent() != nullptr)
		{
			Pg::Serialize::Serializer::SerializeBoolean(&xmlObject, "parent", true);
			Pg::Serialize::Serializer::SerializeString(&xmlObject, "parent_name", object->_transform.GetParent()->_object->GetName());
		}

		pugi::xml_node objComponents = xmlObject.append_child("components");
		for (auto& component : object->GetComponentList())
		{
			if (component.first.find("MonsterHelper") != std::string::npos) continue;

			// 확인한 component의 type에 따라 serialize 한다 
			pugi::xml_node objComponent = objComponents.append_child("component");

			// 변수명, 타입, 값
			std::vector<std::tuple<std::string, std::string, void*>> tSerVec;

			Pg::Serialize::Serializer::SerializeString(&objComponent, "type", component.first);
			pugi::xml_node componentData = objComponent.append_child("data");

			component.second->OnSerialize(tSerVec);

			if (component.first.find("Collider") != std::string::npos)
			{
				auto dyCol = object->GetComponent<Pg::Data::DynamicCollider>();
				if (dyCol != nullptr)
				{
					if (dyCol->GetKinematic())
						Pg::Serialize::Serializer::SerializeBoolean(&componentData, "kinematic", dyCol->GetKinematic());
				}

				auto col = object->GetComponent<Pg::Data::Collider>();
				//Pg::Serialize::Serializer::SerializeUint(&componentData, "layer", col->GetLayer());
				Pg::Serialize::Serializer::SerializeBoolean(&componentData, "trigger", col->GetTrigger());
				Pg::Serialize::Serializer::SerializePGFloat3(&componentData, "positionOffset", col->GetPositionOffset());
				Pg::Serialize::Serializer::SerializePGQuat(&componentData, "rotationOffset", col->GetRotationOffset());
			}

			for (auto& [valName, typeInfo, val] : tSerVec)
			{
				valName = valName.substr(valName.rfind("_") + 1);
				Pg::Serialize::Serializer::Serialize(typeInfo, &componentData, valName, val);
			}
		}
	}
}

Pg::Data::Component* Pg::Editor::Manager::DataManager::AddComponentToObject(const std::string& typeName, Pg::Data::GameObject* go)
{
	return nullptr;
}
