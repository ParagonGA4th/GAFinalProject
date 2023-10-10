#include "FileManager.h"

void FileManager::XmlLoad()
{
	pugi::xml_document doc;
	pugi::xml_node rootNode;

	if (doc.load_file("../ParagonEditor/SceneName.xml"))
	{
		rootNode = doc.child("scene");
		XmlDataParsing(rootNode.first_child());
	}
}

void FileManager::JsonLoad()
{
}

std::vector<GameObjectData*> FileManager::GetGameObjectData() const
{
	return _gameObjectDatas;
}

void FileManager::XmlDataParsing(pugi::xml_node node)
{
	int count = 1;

	// node Name = objects
	for (pugi::xml_node child : node.children("object"))
	{
		GameObjectData* objData = new GameObjectData();
		objData->_objectNumber = count;

		// object 안에 들어있는 데이터를 
		XmlObjectDataParsing(child, *objData);

		_gameObjectDatas.push_back(objData);
		++count;
	}
}

void FileManager::XmlObjectDataParsing(pugi::xml_node node, GameObjectData& data)
{
	// Object가 넘어온다
	for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling())
	{
		std::string childName = child.name();

		if (childName.compare("name") == 0) { data._name = child.child_value(); }
		if (childName.compare("tag") == 0) { data._tag = child.child_value(); }
		if (childName.compare("active") == 0) { data._active = child.child_value(); }
		if (childName.compare("parent") == 0) { data._parent = std::stoi(child.child_value()); }
		if (childName.compare("components") == 0) { XmlComponentDataParsing(child.first_child(), data); }
	}
}

void FileManager::XmlComponentDataParsing(pugi::xml_node node, GameObjectData& data)
{
	for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling())
	{
		std::string childName = child.name();

		if (childName.compare("type") == 0) { data._type = child.child_value(); }
		if (childName.compare("data") == 0) 
		{ 
			for (pugi::xml_node tool = child.first_child(); tool; tool = tool.next_sibling())
			{
				std::string childName = tool.name();

				if (childName.compare("position") == 0) 
				{
					data._transform._position._x = std::stof(tool.first_child().child_value());
					data._transform._position._y = std::stof(tool.first_child().next_sibling().child_value());
					data._transform._position._z = std::stof(tool.first_child().next_sibling().next_sibling().child_value());
				}

				if (childName.compare("rotation") == 0) 
				{
					data._transform._rotation._x = std::stof(tool.first_child().child_value());
					data._transform._rotation._y = std::stof(tool.first_child().next_sibling().child_value());
					data._transform._rotation._z = std::stof(tool.first_child().next_sibling().next_sibling().child_value());
				}

				if (childName.compare("scale") == 0) 
				{
					data._transform._scale._x = std::stof(tool.first_child().child_value());
					data._transform._scale._y = std::stof(tool.first_child().next_sibling().child_value());
					data._transform._scale._z = std::stof(tool.first_child().next_sibling().next_sibling().child_value());
				}
			}
		}
	}
}
