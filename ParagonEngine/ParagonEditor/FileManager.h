#pragma once
#include "DataStruct.h"
#include "pugixml.hpp"

#include <vector>

class FileManager
{
public:
	FileManager() {};
	void XmlLoad();

private:
	void XmlDataParsing(pugi::xml_node node, GameObjectData& data);

private:
	std::vector<GameObjectData> _gameObjectDatas;
	bool isDataPushing;
};

