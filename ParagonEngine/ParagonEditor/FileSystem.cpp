#include "FileSystem.h"
#include "DataManager.h"
#include "Event.h"
#include "../ParagonData/Scene.h"

#include <shobjidl.h>
#include <fstream>
#include <filesystem>
#include <cassert>
#include <windows.h>
#include <singleton-cpp/singleton.h>

namespace fs = std::filesystem;

Pg::Editor::System::FileSystem::FileSystem()
{
	_dataManager = std::make_unique<Pg::Editor::Manager::DataManager>();
	_fileEvent = std::make_unique<Pg::Editor::Event>();

	auto& tdataCon = singleton<Pg::Editor::Data::DataContainer>();
	_dataContainer = &tdataCon;
}

Pg::Editor::System::FileSystem::~FileSystem()
{
}

void Pg::Editor::System::FileSystem::Initialize()
{
	_rootPathWithFileName = fs::current_path().string();
	_rootPathWithFileName = _rootPathWithFileName.substr(0, _rootPathWithFileName.rfind("\\"));
	_rootPathWithFileName += "\\StolenYouth\\StolenYouth.pgproject";

	_dataManager->DataLoad(_rootPathWithFileName);
#if defined(DEBUG) | defined(_DEBUG)
	CreateFolder();
#endif

	_fileEvent->AddEvent(Pg::Editor::eEventType::_NEWSCENE, [&]() { NewScene(); });
	_fileEvent->AddEvent(Pg::Editor::eEventType::_OPENSCENE, [&]() { OpenScene(); });
	_fileEvent->AddEvent(Pg::Editor::eEventType::_LOADSCENE, [&](void* data) { LoadScene(data); });
	_fileEvent->AddEvent(Pg::Editor::eEventType::_SAVESCENE, [&]() { SaveScene(); });
	_fileEvent->AddEvent(Pg::Editor::eEventType::_NEWPROJECT, [&]() { NewProject(); });
	_fileEvent->AddEvent(Pg::Editor::eEventType::_OPENPROJECT, [&]() { OpenProject(); });
	_fileEvent->AddEvent(Pg::Editor::eEventType::_SAVEPROJECT, [&]() { SaveProject(); });
}

void Pg::Editor::System::FileSystem::NewScene()
{
	if(_scenePath.empty()) CreateFolderPath();
	CreateParagonFile(_dataManager->DataCreate(true));
	_dataManager->DataLoad(_scenePathWithFileName, true);
}

void Pg::Editor::System::FileSystem::OpenScene()
{
	_isScene = true;
	ShowDialog(true);

	if (_isCancel)
	{
		_isCancel = false;
		return;
	}

	_dataManager->DataLoad(_rootPathWithFileName, _isScene);

	if (_rootPathWithFileName.find("Asset") != std::string::npos)
	{
		_rootPathWithFileName = _rootPathWithFileName.substr(0, _rootPathWithFileName.rfind("Asset"));
		if (_scenePath.empty()) CreateFolderPath();
	}
}

void Pg::Editor::System::FileSystem::LoadScene(void* path)
{
	if (path == nullptr) return;
	
	std::string filePath = *(static_cast<std::string*>(path));
	_dataManager->DataLoad(filePath, true);
}


void Pg::Editor::System::FileSystem::SaveScene()
{
	_isScene = true;
	CreateParagonFile(_dataManager->DataSave(_isScene));
}

void Pg::Editor::System::FileSystem::NewProject()
{
	_isScene = false;
	ShowDialog(false);

	if (_isCancel)
	{
		_isCancel = false;
		return;
	}

	CreateFolder();
	CreateParagonFile(_dataManager->DataCreate());
	_dataManager->DataLoad(_rootPathWithFileName);
}


void Pg::Editor::System::FileSystem::OpenProject()
{
	_isScene = false;
	ShowDialog(true);
	_dataManager->DataLoad(_rootPathWithFileName);
	CreateFolderPath();
	_fileEvent->Invoke(Pg::Editor::eEventType::ONPROJECTLOAD);
}

void Pg::Editor::System::FileSystem::SaveProject()
{
	_isScene = false;
	if(_rootPathWithFileName.empty()) ShowDialog(false);

	if (_isCancel)
	{
		_isCancel = false;
		return;
	}

	if (!_rootPathWithFileName.empty()) CreateFolder();
	CreateParagonFile(_dataManager->DataSave());
}

void Pg::Editor::System::FileSystem::ShowDialog(bool isOpen)
{
	// COM 라이브러리 초기화
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	assert(SUCCEEDED(hr));

	// Common Item Dialog 인터페이스 생성
	IFileDialog* itemDialog;
	if (isOpen) hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_PPV_ARGS(&itemDialog));
	else hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL, IID_PPV_ARGS(&itemDialog));

	if (FAILED(hr)) { CoUninitialize(); assert(SUCCEEDED(hr)); }

	if (isOpen)	itemDialog->SetTitle(L"Paragon Project"); // 제목 설정 (선택 사항)
	else itemDialog->SetFileName(L"NewProject.pgproject");

	// 파일 필터 설정: .ppt 확장자 필터
	COMDLG_FILTERSPEC fileTypes[1];

	if (_isScene) fileTypes[0] = { L"Pragon Scene", L"*.pgscene" };
	else fileTypes[0] = { L"Pragon Project", L"*.pgproject" };

	itemDialog->SetFileTypes(ARRAYSIZE(fileTypes), fileTypes);
	itemDialog->SetFileTypeIndex(isOpen ? 1 : 0); // 기본 확장자 선택 (1부터 시작)

	// 파일 저장 대화 상자 표시
	hr = itemDialog->Show(NULL);
	IShellItem* pItem;
	LPWSTR filePath = NULL;

	if (SUCCEEDED(hr))
	{
		hr = itemDialog->GetResult(&pItem);
		if (SUCCEEDED(hr))
		{
			hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &filePath);
			if (SUCCEEDED(hr)) CoTaskMemFree(filePath);
			pItem->Release();
		}
	}
	else _isCancel = true;

	if (filePath != NULL)
	{
		std::wstring wString;
		wString.append(&filePath[0]);

		_rootPathWithFileName.clear();
		_rootPathWithFileName.append(wString.begin(), wString.end());
	}

	itemDialog->Release();
	CoUninitialize();
}

void Pg::Editor::System::FileSystem::CreateFolderPath()
{
	_rootPath = _rootPathWithFileName.substr(0, _rootPathWithFileName.rfind("\\"));
	_assetsPath = _rootPath + "\\Asset";
	_scriptPath = _rootPath + "\\Script";
	_scenePath = _assetsPath + "\\Scene";
}

void Pg::Editor::System::FileSystem::CreateFolder()
{
	if (!_rootPath.empty()) return;
	CreateFolderPath();

	fs::path rootPath = _rootPath;
	fs::path subFolder_1 = _assetsPath;
	fs::path subFolder_2 = _scriptPath;
	fs::path subFolder_3 = _scenePath;

	try
	{
		fs::create_directory(rootPath);
		fs::create_directory(subFolder_1);
		fs::create_directory(subFolder_2);
		fs::create_directory(subFolder_3);
	}
	catch (const std::exception& e)
	{
		return;
	}
}

void Pg::Editor::System::FileSystem::CreateParagonFile(std::unordered_map<std::string, std::string> fileData)
{
	try
	{
		for (auto& data : fileData)
		{
			fs::path filePath;
			if (data.first == "project")
			{
				std::string rootPath = _rootPath.substr(0, _rootPath.rfind(".")) + "\\";
				filePath = rootPath.append(_rootPath.substr(_rootPath.rfind("\\") + 1));

				if (filePath.string().find(".pgproject") == std::string::npos)
					filePath = filePath.string() + ".pgproject";
			}
			else
			{
				filePath = _scenePath + "\\" + data.first;

				if (filePath.string().find(".pgscene") == std::string::npos)
					filePath = filePath.string() + ".pgscene";

				_scenePathWithFileName = filePath.string();
			}

			// 파일 생성
			std::ofstream file(filePath, std::ios::out | std::ios::trunc);

			if (file.is_open())
			{
				file << data.second; // 파일에 내용 쓰기
				file.close(); // 파일 닫기
			}
		}
	}
	catch (const std::exception& e)
	{
		// 파일 생성 실패 시 예외 처리
	}
}

//std::string Pg::Editor::System::FileSystem::SeparatingFileName()
//{
//	std::string fileName;
//
//	fileName = _rootPath.substr(_rootPath.rfind("\\") + 1);
//	fileName = fileName.substr(0, fileName.find(".", 0));
//
//	return fileName;
//}