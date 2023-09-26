#include "FileManager.h"
#include "pugixml.hpp"

#include <string>

void FileManager::Test()
{
	pugi::xml_document doc;
	pugi::xml_node rootNode;
	pugi::xml_node firstChildNode;
	std::string str;

	if (doc.load_file("SceneName.xml"))
	{
		rootNode = doc.child("scene");
		firstChildNode = rootNode.first_child();

		for (pugi::xml_node child = firstChildNode.first_child(); child; child = child.next_sibling()) {
			str = child.name();
		}
	}
}
