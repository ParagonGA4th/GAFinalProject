#pragma once
#include "../ParagonUtil/pugixml.hpp"
#include <string>
#include <vector>
#include <unordered_map>

namespace Pg::Data
{
	class Scene;
	class GameObject;
}
namespace Pg::Serialize { class Serializer; }
namespace Pg::Editor::Data { class DataContainer; }
namespace Pg::Editor::Manager
{
	class DataManager
	{
	public:
		DataManager();
		~DataManager();

		void DataLoad(std::string path, std::string fileName);
		std::unordered_map<std::string, std::string> DataSave();

	private:
		void ProjectLoad();
		void SceneLoad();

		//void ProjectSave();
		void SceneSave();

		void DataDeserialize(pugi::xml_node root, int sceneNum);
		void DataSerialize(pugi::xml_node node, Pg::Data::Scene* scene);

	private:
		std::string _path;
		std::string _fileName;

		std::vector<Pg::Data::Scene*> _scenes;
		std::vector<Pg::Data::GameObject*> _changeObjects;
		std::unordered_map<std::string, std::string> _sceneSerializeData;

		Pg::Editor::Data::DataContainer* _dataContainer;
	};
}


