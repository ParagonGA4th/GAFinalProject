#pragma once
#include "DataContainer.h"

#include "../ParagonHelper/pugixml.hpp"

#include <string>
#include <vector>
#include <unordered_map>

namespace Pg::Serialize { class Serializer; }
namespace Pg::Editor::Data { class DataContainer; }
namespace Pg::Data
{
	class Scene;
	class GameObject;
	class Component;
}
namespace Pg::Editor::Manager
{
	class DataManager
	{
	public:
		DataManager();
		~DataManager();

		void DataLoad(std::string path, bool isScene = false);
		std::unordered_map<std::string, std::string> DataSave(bool isScene = false);
		std::unordered_map<std::string, std::string> DataCreate(bool isScene = false);

	private:
		void ProjectLoad(std::string path);
		void SceneLoad(std::string path);

		void ProjectSave();
		void SceneSave(bool isSceneSave);

		void DataDeserialize(pugi::xml_node root, int sceneNum);
		void DataSerialize(pugi::xml_node node, Pg::Data::Scene* scene);

	private:
		Pg::Data::Component* AddComponentToObject(const std::string& typeName, Pg::Data::GameObject* go);

	private:
		std::vector<Pg::Data::Scene*> _scenes;
		std::vector<Pg::Data::GameObject*> _changeObjects;

		// file을 쓰는 것은 FileSystem에서 담당하기 때문에
		// first - SceneName, second - SerializeData
		std::unordered_map<std::string, std::string> _sceneSerializeData; 
		std::unordered_map<Pg::Data::GameObject*, std::string> _existsParentObject;

		Pg::Editor::Data::DataContainer* _dataContainer;
	};
}


