#pragma once
#include <string>
#include <memory>
#include <unordered_map>

namespace Pg::Data { class GameObject; }
namespace Pg::Editor { class Event; }
namespace Pg::Editor::Data { class DataContainer; }

namespace Pg::Editor::Window
{
	// object Name, component Name
	using ObjData = std::unordered_map<std::string, std::unordered_map<std::string, std::vector<std::tuple<std::string, std::string, void*>>>>;

	class InspectorUIManager;
	class InspectorDataManager
	{
		friend class InspectorUIManager;
	public:
		InspectorDataManager();
		~InspectorDataManager();

		void Initialize(InspectorUIManager* manager);
		void Update();

		void SetFlag(int flag);
		void SetObject(Pg::Data::GameObject* object);

	private:
		void GetCurrentSceneObjects(bool isRefresh = false);
		void AddComponent(std::string componentName);
		void RemoveComponent(std::string componentName);
		void ModifiedObject(bool isModified);
		void AddModifiedObject();
		void RefreshData(std::string componentName, bool isRemove);

	private:
		/// Helper Class
		InspectorUIManager* _uiManager;
		Pg::Editor::Data::DataContainer* _dataContainer;

		// Object Managing Value
		std::vector<Pg::Data::GameObject*> _modifiedObjList;
		Pg::Data::GameObject* _object;
		ObjData _objectData;

		int _flag;
		std::string _prevSceneName;

		// Event
		std::unique_ptr<Pg::Editor::Event> _modifiedObject;
	};
}