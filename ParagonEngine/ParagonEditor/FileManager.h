#pragma once
#include <string>

struct IFileDialog;

namespace Pg::Editor::Manager
{
	class FileManager
	{
	public:
		void Initialize(std::string path);

		void FileOpen();
		bool FileSave();

	private:
		std::string GetOpenFilePath();
		std::string GetSaveFilePath();
		void ShowDialog(IFileDialog* fileDialog, std::string& path);

	private:
		std::string _projectPath;
	};
}
