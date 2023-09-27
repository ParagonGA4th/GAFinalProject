#include "FileManager.h"

void FileManager::XmlLoad()
{
	pugi::xml_document doc;
	pugi::xml_node rootNode;

	if (doc.load_file("SceneName.xml"))
	{
		rootNode = doc.child("scene");
		GameObjectData obj;
		XmlDataParsing(rootNode.first_child(), obj);
	}
}

void FileManager::XmlDataParsing(pugi::xml_node node, GameObjectData& data)
{
	std::string str;
	std::string nodeName = node.name();

	if (nodeName.compare("objects") == 0) XmlDataParsing(node.first_child(), data);

	for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling()) 
	{
		str = child.name();

		if ( str.compare("components") == 0
			|| str.compare("component") == 0 || str.compare("data") == 0) {	XmlDataParsing(child, data);}

		if (str.compare("name") == 0) { data._name = child.child_value(); }
		if (str.compare("tag") == 0) { data._tag = child.child_value(); }
		if (str.compare("active") == 0) { data._active = child.child_value(); }
		if (str.compare("parent") == 0) { data._parent = std::stoi(child.child_value()); }
		if (str.compare("type") == 0) { data._type = child.child_value(); }

		if (str.compare("position") == 0)
		{
			data._transform._position._x = std::stof(child.first_child().child_value());
			data._transform._position._y = std::stof(child.first_child().next_sibling().child_value());
			data._transform._position._z = std::stof(child.first_child().next_sibling().next_sibling().child_value());
		}		
		
		if (str.compare("rotation") == 0)
		{
			data._transform._rotation._x = std::stof(child.first_child().child_value());
			data._transform._rotation._y = std::stof(child.first_child().next_sibling().child_value());
			data._transform._rotation._z = std::stof(child.first_child().next_sibling().next_sibling().child_value());
		}		
		
		if (str.compare("scale") == 0)
		{
			data._transform._scale._x = std::stof(child.first_child().child_value());
			data._transform._scale._y = std::stof(child.first_child().next_sibling().child_value());
			data._transform._scale._z = std::stof(child.first_child().next_sibling().next_sibling().child_value());
		}
	}

	if (!isDataPushing)
	{
		_gameObjectDatas.push_back(data);
		isDataPushing = true;
		return;
	}
}
