#pragma once
#include "../ParagonUtil/pugixml.hpp"

#include <vector>
#include <string>
#include <shobjidl.h>

namespace Pg::Data
{
	class GameObject;
	class Scene;
}

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

		void FileLoad(std::string path);

		// Serialize, Desrialize
		void DataSerialize();
		void DataDeserialize(pugi::xml_node node, int sceneNumber);

	private:
		std::string _projectPath;

		// 탐색기 파일 필터 설정을 위한 변수
		COMDLG_FILTERSPEC fileTypes[3];
	
		std::vector<Pg::Data::Scene*> _scenes;
	};
}
