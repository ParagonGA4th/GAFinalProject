#pragma once
#include <memory>
#include <string>

namespace Pg::Editor::Manager
{
	using FilePath = std::string;
	class DataManager;
	class FileManager
	{
	public:
		FileManager();
		~FileManager();

		void Initialize();

		void FileOpen();
		bool FileSave();

	private:
		// Get File Data
		void ShowDialog(bool isOpen);
		
		void CreateFolder();
		void CreatePFile();

		std::string SeparatingFileName();

	private:
		FilePath _rootPath;
		FilePath _assetsPath;
		FilePath _scriptPath;
		std::unique_ptr<Pg::Editor::Manager::DataManager> _dataManager;
	};
}
