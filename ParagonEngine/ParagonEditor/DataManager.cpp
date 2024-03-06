#include "DataManager.h"
#include "Serializer.h"

#include "../ParagonData/Scene.h"
#include "../ParagonData/GameObject.h"

#include <singleton-cpp/singleton.h>
#include <visit_struct/visit_struct_intrusive.hpp>
#include <sstream>


Pg::Editor::Manager::DataManager::DataManager()
{
	auto& tdataCon = singleton<Pg::Editor::Data::DataContainer>();
	_dataContainer = &tdataCon;
}

Pg::Editor::Manager::DataManager::~DataManager()
{}

void Pg::Editor::Manager::DataManager::DataLoad(bool isScene, std::string path)
{
	if (isScene)
	{
		if (_dataContainer->GetSceneList().size() > 0)
		{
			for (auto& scene : _dataContainer->GetSceneList())
			{
				if (path.find(scene->GetSceneName()) != std::string::npos)
				{
					_dataContainer->SetCurrentScene(scene->GetSceneName());
				}
				else
				{
					SceneLoad(path);
					_dataContainer->SetSceneList(_scenes);
					_dataContainer->SetCurrentScene(_scenes.size() - 1);
				}
			}
		}
		else
		{
			SceneLoad(path);
			if (_scenes.size() > 0) _dataContainer->SetSceneList(_scenes);
		}
	}
	else
	{
		_path = path;
		
		_scenes.clear();

		ProjectLoad();
		if (_scenes.size() > 0) _dataContainer->SetSceneList(_scenes);
	}
}

std::unordered_map<std::string, std::string> Pg::Editor::Manager::DataManager::DataSave(bool isScene)
{
	_sceneSerializeData.clear();

	// Data를 가져와서 Serialize
	SceneSave();
	if(!isScene) ProjectSave();

	return _sceneSerializeData;
}

std::unordered_map<std::string, std::string> Pg::Editor::Manager::DataManager::DataCreate()
{
	_sceneSerializeData.clear();

	std::string sceneName = "Sample Scene";
	std::string docToString;
	std::stringstream ss;
	
	// project 생성
	{
		pugi::xml_document proejctDoc;
		proejctDoc.append_child("proejct");

		pugi::xml_node scenesNode = proejctDoc.child("proejct").append_child("scenes");
		Pg::Serialize::Serializer::SerializeString(&scenesNode, "scene", sceneName);

		proejctDoc.save(ss, "\t");
		docToString = ss.str();
		_sceneSerializeData.insert({ "project", docToString });
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


void Pg::Editor::Manager::DataManager::ProjectLoad()
{
	pugi::xml_document doc;

	if (doc.load_file(_path.c_str()))
	{
		pugi::xml_node rootNode = doc.first_child();
		for (pugi::xml_node scene = rootNode.first_child().first_child(); scene; scene = scene.next_sibling())
		{
			_dataContainer->SetProjectPath(_path.substr(0, _path.rfind("\\") + 1));

			std::string scenePath = _path.substr(0, _path.rfind("\\") + 1).append("Asset\\Scene\\");
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
		newScene->GetObjectList().at(0)->AddComponent("class Pg::Data::Camera");
		_scenes.push_back(newScene);

		pugi::xml_node rootNode = doc.child("scene");
		DataDeserialize(rootNode.first_child(), _scenes.size() - 1);
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

void Pg::Editor::Manager::DataManager::SceneSave()
{
	//for (auto& scene : _dataContainer->GetSceneList())
	//{

	pugi::xml_document doc;

	pugi::xml_node declarationNode = doc.prepend_child(pugi::node_declaration);
	declarationNode.append_attribute("version") = "1.0";
	declarationNode.append_attribute("encoding") = "utf-8";

	doc.append_child("scene");
	pugi::xml_node node = doc.child("scene").append_child("objects");

	DataSerialize(node, _dataContainer->GetCurrentScene());

	std::stringstream ss;
	doc.save(ss, "\t"); // save 함수를 사용하여 스트림에 XML을 저장

	std::string docToString = ss.str();

	_sceneSerializeData.insert({ _dataContainer->GetCurrentScene()->GetSceneName(), docToString });

	std::string sceneName = _dataContainer->GetCurrentScene()->GetSceneName().substr(0, _dataContainer->GetCurrentScene()->GetSceneName().rfind("."));

	//}
}


void Pg::Editor::Manager::DataManager::DataDeserialize(pugi::xml_node root, int sceneNum)
{
	//ScenesData scenesData;
	// <objects>를 순회 하며 pgscene에 있는 object를 가져온다
	for (pugi::xml_node object = root.first_child(); object; object = object.next_sibling())
	{
		// GameObject 생성
		Pg::Data::GameObject* obj = _scenes.at(sceneNum)->AddObject(Pg::Serialize::Serializer::DeserializeString(&object, "name"));

		obj->SetActive(Pg::Serialize::Serializer::DeserializeBoolean(&object, "active"));
		obj->SetTag(Pg::Serialize::Serializer::DeserializeString(&object, "tag"));

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
					auto component = obj->AddComponent(typeName);
					component->OnDeserialize(tSerVec);
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
		pugi::xml_node xmlObject = node.append_child("object");

		Pg::Serialize::Serializer::SerializeString(&xmlObject, "name", object->GetName());
		Pg::Serialize::Serializer::SerializeBoolean(&xmlObject, "active", object->GetActive());
		Pg::Serialize::Serializer::SerializeString(&xmlObject, "tag", object->GetTag());

		//xmlObject.append_child("parent");

		pugi::xml_node objComponents = xmlObject.append_child("components");

		// 확인한 component의 type에 따라 serialize 한다
		pugi::xml_node objComponent = objComponents.append_child("component");

		for (auto& component : object->GetComponentList())
		{
			// 변수명, 타입, 값
			std::vector<std::tuple<std::string, std::string, void*>> tSerVec;

			Pg::Serialize::Serializer::SerializeString(&objComponent, "type", component.first);
			pugi::xml_node componentData = objComponent.append_child("data");

			component.second->OnSerialize(tSerVec);

			for (auto& [valName, typeInfo, val] : tSerVec)
			{
				valName = valName.substr(valName.rfind("_") + 1);
				Pg::Serialize::Serializer::Serialize(typeInfo, &componentData, valName, val);
			}
		}
	}
}