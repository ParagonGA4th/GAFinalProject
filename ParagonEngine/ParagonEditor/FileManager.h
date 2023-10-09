#pragma once
#include "DataStruct.h"
#include "../ParagonUtil/pugixml.hpp"

#include <vector>

class FileManager
{
public:
	FileManager() {};
	void XmlLoad();
	void JsonLoad();

	std::vector<GameObjectData*> GetGameObjectData() const;

private:
	void XmlDataParsing(pugi::xml_node node);
	void XmlObjectDataParsing(pugi::xml_node node, GameObjectData& data);
	void XmlComponentDataParsing(pugi::xml_node node, GameObjectData& data);

private:
	std::vector<GameObjectData*> _gameObjectDatas;
};

