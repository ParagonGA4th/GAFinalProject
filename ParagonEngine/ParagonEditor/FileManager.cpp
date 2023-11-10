#include "FileManager.h"

#include "../ParagonData/Serializer.h"
#include "../ParagonData/Scene.h"
#include "../ParagonData/GameObject.h"

#include <cassert>

using Pg::Data::Serialize::Serializer;

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
	std::string openFileFullPath = GetOpenFilePath();

	if (!openFileFullPath.empty()) FileLoad(openFileFullPath);	
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

	// Common Item Dialog 인터페이스 생성
	IFileOpenDialog* pFileOpen;
	hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_PPV_ARGS(&pFileOpen));
	if (FAILED(hr)) { CoUninitialize(); assert(SUCCEEDED(hr)); }

	// Common Item Dialog 특성 설정
	pFileOpen->SetTitle(L"Paragon Project"); // 제목 설정 (선택 사항)
	//pFileOpen->SetOptions(FOS_PICKFOLDERS); // 폴더 선택 대화 상자로 사용 (선택 사항)

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

	// Common Item Dialog 인터페이스 생성
	IFileSaveDialog* pFileSave;
	hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL, IID_PPV_ARGS(&pFileSave));
	if (FAILED(hr)) { CoUninitialize(); assert(SUCCEEDED(hr)); }

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

void Pg::Editor::Manager::FileManager::FileLoad(std::string path)
{

	pugi::xml_document doc;
	if (doc.load_file(path.c_str()))
	{
		// project를 불러올 때와 scene을 불러올 때 경우가 달라야 함
		path = path.substr(path.rfind("\\") + 1);
		path = path.substr(0, path.find(".", 0));
		_scenes.push_back(new Pg::Data::Scene(path));

		pugi::xml_node rootNode = doc.child("scene");
		DataDeserialize(rootNode.first_child(), _scenes.size()- 1);
	}
}

void Pg::Editor::Manager::FileManager::DataDeserialize(pugi::xml_node node, int sceneNumber)
{
	// <objects>를 순회 하며 pgscene에 있는 object를 가져온다
	for (pugi::xml_node object = node.first_child(); object; object = node.next_sibling())
	{
		// GameObject 생성
		Pg::Data::GameObject* obj = _scenes.at(sceneNumber)->AddObject(Serializer::DeserializeString(&object, "name"));
		obj->SetActive(Serializer::DeserializeBoolean(&object, "active"));

		// 컴포넌트를 추가하기 위해 노드 가져오기
		pugi::xml_node comps = object.find_node([](const pugi::xml_node& node) { return std::string(node.name()) == "components"; });

		for (pugi::xml_node component = comps.first_child(); component; component = comps.next_sibling())
		{

		}
	}
}
