#pragma once
#include "DataStruct.h"
#include "../ParagonUtil/pugixml.hpp"

#include <vector>
#include <string>

namespace Pg::Editor::Manager
{
	class FileManager
	{
	public:
		FileManager();
		~FileManager();

		//void XmlLoad();
		//void JsonLoad();

		//std::vector<GameObjectData*> GetGameObjectData() const;

	private:
		//void XmlDataParsing(pugi::xml_node node);
		//void XmlObjectDataParsing(pugi::xml_node node, GameObjectData& data);
		//void XmlComponentDataParsing(pugi::xml_node node, GameObjectData& data);

	private:
		//std::vector<GameObjectData*> _gameObjectDatas;
		std::string _projectPath;
	};

}
