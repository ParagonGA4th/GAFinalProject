#pragma once
#include "../ParagonUtil/pugixml.hpp"
#include <string>
#include <vector>

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

		void SetFilePath(std::string path);
		void DataLoad();
		void DataSave();

	private:
		void ProjectLoad();
		void SceneLoad();

		//void ProjectSave();
		//void SceneSave();

		void DataDeserialize(pugi::xml_node root, int sceneNum);
		//void DataSerialize();

		void TransformDeserialize(pugi::xml_node component, Pg::Data::GameObject* obj);

	private:
		std::string _path;
		std::string _fileName;

		std::vector<Pg::Data::Scene*> _scenes;
		std::vector<Pg::Data::GameObject*> _changeObjects;

		Pg::Editor::Data::DataContainer* _dataContainer;
	};
}


