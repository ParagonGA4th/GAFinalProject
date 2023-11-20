#pragma once
#include <shobjidl.h>
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
		void CreateFile();

	private:
		FilePath _path;
		std::unique_ptr<Pg::Editor::Manager::DataManager> _dataManager;

		// 탐색기 파일 필터 설정을 위한 변수
		COMDLG_FILTERSPEC fileTypes[3];	
	};
}
