#pragma once
#include <memory>
#include <string>
#include <unordered_map>

namespace Pg::Editor { class Event; }
namespace Pg::Editor::Manager { class DataManager; }
namespace Pg::Editor::Data { class DataContainer; }

namespace Pg::Editor::System
{
	using FilePath = std::string;
	class FileSystem
	{
	public:
		FileSystem();
		~FileSystem();

		void Initialize();

	private:
		void NewScene();
		void OpenScene();
		void LoadScene(void* path);
		void SaveScene();

		void NewProject();
		void OpenProject();
		void SaveProject();

		// Get File Data
		void ShowDialog(bool isOpen);
		
		void CreateFolderPath();
		void CreateFolder();
		void CreateParagonFile(std::unordered_map<std::string, std::string> fileData);

		//std::string SeparatingFileName();

	private:
		FilePath _rootPath;
		FilePath _scenePath;
		FilePath _assetsPath;
		FilePath _scriptPath;
		std::string _rootPathWithFileName;
		std::string _scenePathWithFileName;

		std::unique_ptr<Pg::Editor::Manager::DataManager> _dataManager;
		std::unique_ptr<Pg::Editor::Event> _fileEvent;
		Pg::Editor::Data::DataContainer* _dataContainer;

		bool _isCancel = false;
		bool _isScene;
	};
}
