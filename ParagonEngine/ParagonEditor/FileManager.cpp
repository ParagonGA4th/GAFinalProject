#include "FileManager.h"
#include "DataManager.h"

#include <cassert>

Pg::Editor::Manager::FileManager::FileManager()
{
	_dataManager = std::make_unique<Pg::Editor::Manager::DataManager>();
}

Pg::Editor::Manager::FileManager::~FileManager()
{
}

void Pg::Editor::Manager::FileManager::Initialize()
{
	// 파일 필터 설정
	fileTypes[0] = { L"Pragon Project", L"*.pgproject" };
	fileTypes[1] = { L"Pragon Scene", L"*.pgscene" };
	fileTypes[2] = { L"모든 파일", L"*.*" };

	// project가 처음 open 될 때는 기존 폴더(Builds//x64//Relase//)에 있는 sample load.
}

void Pg::Editor::Manager::FileManager::FileOpen()
{
	ShowDialog(true);
	_dataManager->SetFilePath(_path);
	_dataManager->DataLoad();
}

bool Pg::Editor::Manager::FileManager::FileSave()
{
	ShowDialog(false);
	_dataManager->SetFilePath(_path);
	_dataManager->DataSave();
	return true;
}

void Pg::Editor::Manager::FileManager::ShowDialog(bool isOpen)
{
	// COM 라이브러리 초기화
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	assert(SUCCEEDED(hr));

	// Common Item Dialog 인터페이스 생성
	IFileOpenDialog* itemDialog;
	if (isOpen) hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_PPV_ARGS(&itemDialog));
	else hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL, IID_PPV_ARGS(&itemDialog));

	if (FAILED(hr)) { CoUninitialize(); assert(SUCCEEDED(hr)); }

	if (isOpen)	itemDialog->SetTitle(L"Paragon Project"); // 제목 설정 (선택 사항)
	else itemDialog->SetFileName(L"NewProject.pgproject");

	itemDialog->SetFileTypes(ARRAYSIZE(fileTypes), fileTypes);
	itemDialog->SetFileTypeIndex(1); // 기본 확장자 선택 (1부터 시작)

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

		_path.append(wString.begin(), wString.end());
	}

	itemDialog->Release();
	CoUninitialize();
}

