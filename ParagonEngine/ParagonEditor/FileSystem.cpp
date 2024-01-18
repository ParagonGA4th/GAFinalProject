#include "FileSystem.h"
#include "DataManager.h"
#include "Event.h"

#include <shobjidl.h>
#include <fstream>
#include <filesystem>
#include <cassert>
#include <windows.h>

namespace fs = std::filesystem;

Pg::Editor::System::FileSystem::FileSystem()
{
	_dataManager = std::make_unique<Pg::Editor::Manager::DataManager>();
	_fileSaveEvent = std::make_unique<Pg::Editor::Event>();
	_fileOpenEvent = std::make_unique<Pg::Editor::Event>();
}

Pg::Editor::System::FileSystem::~FileSystem()
{
}

void Pg::Editor::System::FileSystem::Initialize()
{
	// project가 처음 open 될 때는 기존 폴더(Builds//x64//Relase//)에 있는 sample load.

	_fileSaveEvent->AddEvent(Pg::Editor::eEventType::FileSave, [&]() { FileSave(); });
	_fileOpenEvent->AddEvent(Pg::Editor::eEventType::FileOpen, [&]() { FileOpen(); });
}

void Pg::Editor::System::FileSystem::FileOpen()
{
	ShowDialog(true);
	_dataManager->DataLoad(_rootPath);
}

void Pg::Editor::System::FileSystem::FileSave()
{
	ShowDialog(false);
	CreateFolder();
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

	fileTypes[0] = { L"Pragon Project", L"*.pgproject" };

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

	if (filePath != NULL)
	{
		std::wstring wString;
		wString.append(&filePath[0]);

		_rootPath.clear();
		_rootPath.append(wString.begin(), wString.end());
	}

	itemDialog->Release();
	CoUninitialize();
}

void Pg::Editor::System::FileSystem::CreateFolder()
{
	fs::path rootPath = _rootPath.substr(0, _rootPath.rfind("."));

	_assetsPath = rootPath.string() + "\\Assets";
	fs::path subFolder_1 = _assetsPath;

	_scriptPath = rootPath.string() + "\\Scripts";
	fs::path subFolder_2 = _scriptPath;

	fs::create_directory(rootPath);
	fs::create_directory(subFolder_1);
	fs::create_directory(subFolder_2);
}

void Pg::Editor::System::FileSystem::CreateParagonFile(std::unordered_map<std::string, std::string> fileData)
{
	try
	{
		for (auto& data : fileData)
		{
			fs::path filePath = _assetsPath + "\\" + data.first + ".pgscene";

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