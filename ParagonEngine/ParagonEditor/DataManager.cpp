#include "DataManager.h"
#include "Serializer.h"
#include "DataContainer.h"

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

void Pg::Editor::Manager::DataManager::DataLoad(std::string path, std::string fileName)
{
	_path = path;
	_fileName = fileName;

	if (_path.find("pgproject") == std::string::npos) SceneLoad();
	else ProjectLoad();

	if (_scenes.size() > 0) _dataContainer->SetSceneList(_scenes);
}

std::unordered_map<std::string, std::string> Pg::Editor::Manager::DataManager::DataSave()
{
	// Data를 가져와서 Serialize
	SceneSave();
	return _sceneSerializeData;
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
	for (auto& scene : _dataContainer->GetSceneList())
	{
		pugi::xml_document doc;

		pugi::xml_node declarationNode = doc.prepend_child(pugi::node_declaration);
		declarationNode.append_attribute("version") = "1.0";
		declarationNode.append_attribute("encoding") = "utf-8";

		doc.append_child("scene");
		pugi::xml_node node = doc.child("scene").append_child("objects");

		DataSerialize(node, scene);

		std::stringstream ss;
		doc.save(ss, "\t"); // save 함수를 사용하여 스트림에 XML을 저장

		std::string docToString = ss.str();

		_sceneSerializeData.insert({ scene->GetSceneName(), docToString });
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
		obj->SetTag(Pg::Serialize::Serializer::DeserializeString(&object, "tag"));

		// 컴포넌트를 추가하기 위해 노드 가져오기
		pugi::xml_node comps = object.find_node([](const pugi::xml_node& node) { return std::string(node.name()) == "components"; });

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