#include "DataManager.h"
#include "Serializer.h"
#include "DataContainer.h"

#include "../ParagonData/Scene.h"
#include "../ParagonData/GameObject.h"

#include <algorithm>
#include <singleton-cpp/singleton.h>

Pg::Editor::Manager::DataManager::DataManager()
{
	auto& tdataCon = singleton<Pg::Editor::Data::DataContainer>();
	_dataContainer = &tdataCon;
}

Pg::Editor::Manager::DataManager::~DataManager() 
{}

void Pg::Editor::Manager::DataManager::DataLoad(std::string path, std::string fileName)
{
	_path = path;
	_fileName = fileName;

	if (_path.find("pgproject") == std::string::npos) SceneLoad();
	else ProjectLoad();

	if(_scenes.size() > 0) _dataContainer->SetScenes(_scenes);
}

void Pg::Editor::Manager::DataManager::DataSave(std::string path, std::string fileName)
{
	// Data를 가져와서 Serialize
	
	SceneSave();

	// xml로 파싱
	// 폴더 생성()
}

void Pg::Editor::Manager::DataManager::ProjectLoad()
{

}

void Pg::Editor::Manager::DataManager::SceneLoad()
{
	pugi::xml_document doc;
	if (doc.load_file(_path.c_str()))
	{
		_scenes.push_back(new Pg::Data::Scene(_fileName));

		pugi::xml_node rootNode = doc.child("scene");
		DataDeserialize(rootNode.first_child(), _scenes.size() - 1);
	}
}

void Pg::Editor::Manager::DataManager::SceneSave()
{
	for (auto& scene : _dataContainer->GetScenes())
	{
		pugi::xml_document doc;
		
		doc.append_child("scene");
		pugi::xml_node node = doc.child("scene").append_child("objects");

		DataSerialize(node, scene);
	}
	// 파일 덮어쓰기
}


void Pg::Editor::Manager::DataManager::DataDeserialize(pugi::xml_node root, int sceneNum)
{
	// <objects>를 순회 하며 pgscene에 있는 object를 가져온다
	for (pugi::xml_node object = root.first_child(); object; object = root.next_sibling())
	{
		// GameObject 생성
		Pg::Data::GameObject* obj = _scenes.at(sceneNum)->AddObject(Pg::Serialize::Serializer::DeserializeString(&object, "name"));

		obj->SetActive(Pg::Serialize::Serializer::DeserializeBoolean(&object, "active"));

		// 컴포넌트를 추가하기 위해 노드 가져오기
		pugi::xml_node comps = object.find_node([](const pugi::xml_node& node) { return std::string(node.name()) == "components"; });

		for (pugi::xml_node component = comps.first_child(); component; component = comps.next_sibling())
		{
			std::string typeName = typeid(Pg::Data::Transform).name();

			if (typeName == Pg::Serialize::Serializer::DeserializeString(&component, "type"))
			{
				pugi::xml_node trans = component.find_node([](const pugi::xml_node& node) { return std::string(node.name()) == "position"; });
				obj->_transform.SetPosition(Pg::Serialize::Serializer::DeserializeVec3(&trans, "x"));

				trans = component.find_node([](const pugi::xml_node& node) { return std::string(node.name()) == "rotation"; });
				obj->_transform.SetRotation(Pg::Serialize::Serializer::DeserializeQuaternion(&trans, "w"));

				trans = component.find_node([](const pugi::xml_node& node) { return std::string(node.name()) == "scale"; });
				obj->_transform.SetScale(Pg::Serialize::Serializer::DeserializeVec3(&trans, "x"));
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

		xmlObject.append_child("name");
		xmlObject.append_child("tag");
		xmlObject.append_child("active");
		xmlObject.append_child("parent");

		xmlObject.append_child("components");
	}
}
