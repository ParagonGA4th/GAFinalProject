#pragma once
#include <memory>
#include <string>
#include <unordered_map>

namespace Pg::Editor { class Event; }
namespace Pg::Editor::Manager { class DataManager; }

namespace Pg::Editor::System
{
	using FilePath = std::string;
	class FileSystem
	{
	public:
		FileSystem();
		~FileSystem();

		void Initialize();

		void FileOpen();
		void FileSave();

	private:
		// Get File Data
		void ShowDialog(bool isOpen);
		
		void CreateFolder();
		void CreateParagonFile(std::unordered_map<std::string, std::string> fileData);

		std::string SeparatingFileName();

	private:
		FilePath _rootPath;
		FilePath _assetsPath;
		FilePath _scriptPath;

		std::unique_ptr<Pg::Editor::Manager::DataManager> _dataManager;
		std::unique_ptr<Pg::Editor::Event> _fileSaveEvent;
		std::unique_ptr<Pg::Editor::Event> _fileOpenEvent;
	};
}
