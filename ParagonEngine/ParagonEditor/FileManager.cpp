#include "FileManager.h"
#include <cassert>
#include <shobjidl.h>
#include <fstream>

void Pg::Editor::Manager::FileManager::Initialize(std::string path)
{
	_projectPath = path;
}

void Pg::Editor::Manager::FileManager::FileOpen()
{
	std::string openFileFullPath = GetOpenFilePath();

	// 파일 스트림을 열기
	std::ifstream fileStream(openFileFullPath);
	std::string fileContext;

	if (fileStream.is_open()) 
	{
		std::string line;
		// 파일로부터 한 줄씩 읽어오기
		while (std::getline(fileStream, line)) 
		{
			fileContext.append(line);
		}

		// 파일 스트림 닫기
		fileStream.close();
	}

	while (fileContext.find("\t") != std::string::npos)
	{
		fileContext.erase(fileContext.find("\t"), 1);
	}
}

bool Pg::Editor::Manager::FileManager::FileSave()
{
	std::string saveFileFullPath = GetSaveFilePath();

	return true;
}

std::string Pg::Editor::Manager::FileManager::GetOpenFilePath()
{
	// COM 라이브러리 초기화
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	assert(SUCCEEDED(hr));

	IFileOpenDialog* pFileOpen;

	// Common Item Dialog 인터페이스 생성
	hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_PPV_ARGS(&pFileOpen));
	if (FAILED(hr)) { CoUninitialize(); }

	// Common Item Dialog 특성 설정
	// pFileOpen->SetTitle(L"파일 선택 대화 상자"); // 제목 설정 (선택 사항)
	//pFileOpen->SetOptions(FOS_PICKFOLDERS); // 폴더 선택 대화 상자로 사용 (선택 사항)

	// 파일 필터 설정: .ppt 확장자 필터
	COMDLG_FILTERSPEC fileTypes[] = {
		{ L"Pragon Project", L"*.pgproject" },
		{ L"모든 파일", L"*.*" },
	};

	pFileOpen->SetFileTypes(ARRAYSIZE(fileTypes), fileTypes);
	pFileOpen->SetFileTypeIndex(1); // 기본 확장자 선택 (1부터 시작)

	// 파일 선택 대화 상자 표시
	std::string filePath;
	ShowDialog(pFileOpen, filePath);

	pFileOpen->Release();
	CoUninitialize();

	return filePath;
}

std::string Pg::Editor::Manager::FileManager::GetSaveFilePath()
{
	// COM 라이브러리 초기화
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	assert(SUCCEEDED(hr));

	IFileSaveDialog* pFileSave;

	// Common Item Dialog 인터페이스 생성
	hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL, IID_PPV_ARGS(&pFileSave));
	if (FAILED(hr)) { CoUninitialize(); assert(SUCCEEDED(hr)); }

	// 파일 필터 설정: .ppt 확장자 필터
	COMDLG_FILTERSPEC fileTypes[] = {
		{ L"Pragon Project", L"*.pgproject" },
	};
	pFileSave->SetFileTypes(ARRAYSIZE(fileTypes), fileTypes);
	pFileSave->SetFileTypeIndex(1); // 기본 확장자 선택 (1부터 시작)

	// 초기 파일 이름 및 경로 설정
	pFileSave->SetFileName(L"NewProject.pgproject");

	// 파일 선택 대화 상자 표시
	std::string filePath;
	ShowDialog(pFileSave, filePath);

	pFileSave->Release();
	CoUninitialize();

	return filePath;
}

void Pg::Editor::Manager::FileManager::ShowDialog(IFileDialog* fileDialog, std::string& path)
{
	// 파일 저장 대화 상자 표시
	HRESULT hr = fileDialog->Show(NULL);
	IShellItem* pItem;
	LPWSTR filePath = NULL;

	if (SUCCEEDED(hr))
	{
		hr = fileDialog->GetResult(&pItem);
		if (SUCCEEDED(hr))
		{
			hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &filePath);
			if (SUCCEEDED(hr))
			{
				CoTaskMemFree(filePath);
			}
			pItem->Release();
		}
	}

	if (filePath != NULL)
	{
		std::wstring wString;
		wString.append(&filePath[0]);

		path.append(wString.begin(), wString.end());
	}
}


//void Pg::Editor::Manager::FileManager::XmlLoad()
//{
//	pugi::xml_document doc;
//	pugi::xml_node rootNode;
//
//	if (doc.load_file("../ParagonEditor/SceneName.xml"))
//	{
//		rootNode = doc.child("scene");
//		XmlDataParsing(rootNode.first_child());
//	}
//}
//
//void Pg::Editor::Manager::FileManager::JsonLoad()
//{
//}
//
//std::vector<GameObjectData*> Pg::Editor::Manager::FileManager::GetGameObjectData() const
//{
//	return _gameObjectDatas;
//}
//
//void Pg::Editor::Manager::FileManager::XmlDataParsing(pugi::xml_node node)
//{
//	int count = 1;
//
//	// node Name = objects
//	for (pugi::xml_node child : node.children("object"))
//	{
//		GameObjectData* objData = new GameObjectData();
//		objData->_objectNumber = count;
//
//		// object 안에 들어있는 데이터를 
//		XmlObjectDataParsing(child, *objData);
//
//		_gameObjectDatas.push_back(objData);
//		++count;
//	}
//}
//
//void Pg::Editor::Manager::FileManager::XmlObjectDataParsing(pugi::xml_node node, GameObjectData& data)
//{
//	// Object가 넘어온다
//	for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling())
//	{
//		std::string childName = child.name();
//
//		if (childName.compare("name") == 0) { data._name = child.child_value(); }
//		if (childName.compare("tag") == 0) { data._tag = child.child_value(); }
//		if (childName.compare("active") == 0) { data._active = child.child_value(); }
//		if (childName.compare("parent") == 0) { data._parent = std::stoi(child.child_value()); }
//		if (childName.compare("components") == 0) { XmlComponentDataParsing(child.first_child(), data); }
//	}
//}
//
//void Pg::Editor::Manager::FileManager::XmlComponentDataParsing(pugi::xml_node node, GameObjectData& data)
//{
//	for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling())
//	{
//		std::string childName = child.name();
//
//		if (childName.compare("type") == 0) { data._type = child.child_value(); }
//		if (childName.compare("data") == 0) 
//		{ 
//			for (pugi::xml_node tool = child.first_child(); tool; tool = tool.next_sibling())
//			{
//				std::string childName = tool.name();
//
//				if (childName.compare("position") == 0) 
//				{
//					data._transform._position._x = std::stof(tool.first_child().child_value());
//					data._transform._position._y = std::stof(tool.first_child().next_sibling().child_value());
//					data._transform._position._z = std::stof(tool.first_child().next_sibling().next_sibling().child_value());
//				}
//
//				if (childName.compare("rotation") == 0) 
//				{
//					data._transform._rotation._x = std::stof(tool.first_child().child_value());
//					data._transform._rotation._y = std::stof(tool.first_child().next_sibling().child_value());
//					data._transform._rotation._z = std::stof(tool.first_child().next_sibling().next_sibling().child_value());
//				}
//
//				if (childName.compare("scale") == 0) 
//				{
//					data._transform._scale._x = std::stof(tool.first_child().child_value());
//					data._transform._scale._y = std::stof(tool.first_child().next_sibling().child_value());
//					data._transform._scale._z = std::stof(tool.first_child().next_sibling().next_sibling().child_value());
//				}
//			}
//		}
//	}
//}
