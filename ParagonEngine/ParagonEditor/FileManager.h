#pragma once
#include "../ParagonUtil/pugixml.hpp"
#include <string>
#include <shobjidl.h>

namespace Pg::Editor::Manager
{
	class FileManager
	{
	public:
		void Initialize();

		void FileOpen();
		bool FileSave();

	private:
		// Get File Data
		std::string GetOpenFilePath();
		std::string GetSaveFilePath();
		void ShowDialog(IFileDialog* fileDialog, std::string& path);

		// Serialize, Desrialize

	private:
		std::string _projectPath;
		COMDLG_FILTERSPEC fileTypes[3];
	};
}
