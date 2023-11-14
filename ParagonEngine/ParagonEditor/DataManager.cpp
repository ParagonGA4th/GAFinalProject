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

void Pg::Editor::Manager::DataManager::SetFilePath(std::string path)
{
	_path = path;
}


void Pg::Editor::Manager::DataManager::DataLoad()
{
	if (_path.find("pgproject") == std::string::npos) SceneLoad();
	else ProjectLoad();

	_dataContainer->SetScenes(_scenes);
}

void Pg::Editor::Manager::DataManager::DataSave()
{

}

void Pg::Editor::Manager::DataManager::ProjectLoad()
{

}

void Pg::Editor::Manager::DataManager::SceneLoad()
{
	pugi::xml_document doc;
	if (doc.load_file(_path.c_str()))
	{
		_fileName = _path.substr(_path.rfind("\\") + 1);
		_fileName = _fileName.substr(0, _fileName.find(".", 0));

		_scenes.push_back(new Pg::Data::Scene(_fileName));

		pugi::xml_node rootNode = doc.child("scene");
		DataDeserialize(rootNode.first_child(), _scenes.size() - 1);
	}
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
				TransformDeserialize(component, obj);
			}
		}
	}
}

void Pg::Editor::Manager::DataManager::TransformDeserialize(pugi::xml_node component, Pg::Data::GameObject* obj)
{
	pugi::xml_node trans = component.find_node([](const pugi::xml_node& node) { return std::string(node.name()) == "position"; });
	obj->_transform.SetPosition
	(
		Pg::Serialize::Serializer::DeserializeFloat(&trans, "x"),
		Pg::Serialize::Serializer::DeserializeFloat(&trans, "y"),
		Pg::Serialize::Serializer::DeserializeFloat(&trans, "z")
	);

	trans = component.find_node([](const pugi::xml_node& node) { return std::string(node.name()) == "rotation"; });
	obj->_transform.SetRotation
	(
		Pg::Serialize::Serializer::DeserializeFloat(&trans, "W"),
		Pg::Serialize::Serializer::DeserializeFloat(&trans, "x"),
		Pg::Serialize::Serializer::DeserializeFloat(&trans, "y"),
		Pg::Serialize::Serializer::DeserializeFloat(&trans, "z")
	);

	trans = component.find_node([](const pugi::xml_node& node) { return std::string(node.name()) == "scale"; });
	obj->_transform.SetScale
	(
		Pg::Serialize::Serializer::DeserializeFloat(&trans, "x"),
		Pg::Serialize::Serializer::DeserializeFloat(&trans, "y"),
		Pg::Serialize::Serializer::DeserializeFloat(&trans, "z")
	);
}