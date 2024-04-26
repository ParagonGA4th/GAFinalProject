#include "Filter.h"
#include "DataContainer.h"
#include "Event.h"

#include "../ParagonUI/UIManager.h"
#include "../ParagonUI/WidgetContainer.h"
#include "../ParagonUI/Selectable.h"
#include "../ParagonUI/TreeNode.h"

#include <filesystem>
#include <singleton-cpp/singleton.h>

namespace fs = std::filesystem;

Pg::Editor::Window::Filter::Filter()
	:_winName("Project"), _isShow(true)
{
	auto& tdataCon = singleton<Pg::Editor::Data::DataContainer>();
	_dataContainer = &tdataCon;

	auto& tUIManager = singleton<Pg::UI::Manager::UIManager>();
	_uiManager = &tUIManager;

	_widgetCon = std::make_unique<Pg::UI::WidgetContainer>();
}

Pg::Editor::Window::Filter::~Filter()
{
}

void Pg::Editor::Window::Filter::Initialize()
{
	// 프로젝트의 디렉터리(경로)를 가져온다
	// for문을 돌며 폴더와 파일을 구분한다(. -> 확장자 존재시 필수이기 때문에 구분자로 쓰임)
	// 파일일 경우 각 파일 명에 따라 list 생성 -> selectable 클래스가 string list를 받기 때문
	// 폴더일 경우 그동안 생성되었던 파일명 list를 selectable에 넘겨준 뒤, treenode 생성
}

void Pg::Editor::Window::Filter::Update()
{
	_uiManager->WindowBegin(_winName);
	
	_widgetCon->ClearWidget();
	_fileNames.clear();
	
	DataSet(_dataContainer->GetProjectPath());	
	_widgetCon->Update();
	FileSelected();

	_uiManager->WindowEnd();
}

void Pg::Editor::Window::Filter::Finalize()
{

}

void Pg::Editor::Window::Filter::SetShow(bool show)
{
	_isShow = show;
}

bool Pg::Editor::Window::Filter::GetShow()
{
	return _isShow;
}

void Pg::Editor::Window::Filter::DataSet(std::string path)
{
	if (path.empty()) return;

	std::string rootFolderName = path.substr(0, path.rfind("\\"));
	rootFolderName = rootFolderName.substr(rootFolderName.rfind("\\") + 1);
	
	static bool isEnter = false;
	static int depth = 0;

	// 1차 폴더 및 파일 구분
	auto firstFiles = SeparateFiles(path, rootFolderName);

	for (auto& f : firstFiles)
	{
		if (rootFolderName != f.first)
		{
			std::string folderName = f.first.substr(f.first.rfind("\\") + 1);

			auto secondFiles = SeparateFiles(f.first, folderName);

			for (auto& fn : secondFiles)
			{
				if (fn.first == folderName)
				{
					_fileNames.insert({ folderName, secondFiles[folderName] });

					auto& selectable = _widgetCon->CreateTreeNodeWidget<Pg::UI::Widget::Selectable>(_fileNames[folderName], _selectObject);
					_isDoubleClicked = selectable.GetSelectedObjectDoubleClicked();
					
					if (depth > 0 && !isEnter) isEnter = true;
				}
				else if (fn.first != folderName && !isEnter && depth < 1)
				{
					depth++;
					DataSet(f.first);
				}
			}

			if (isEnter)
			{
				isEnter = false;
				_widgetCon->CreateTreeNodeWidget<Pg::UI::Widget::TreeNode>(folderName, _widgetCon->GetTreeNodeWidgets());
				_widgetCon->ClearTreeNodeWidget(depth - 1);
				_widgetCon->SetTempWidgets(_widgetCon->GetTreeNodeWidgets());
				_widgetCon->ClearTreeNodeWidget();
			}
			else
			{
				if (_widgetCon->GetTempWidgets().size() > 0)
				{
					_widgetCon->CreateWidget<Pg::UI::Widget::TreeNode>(folderName, _widgetCon->GetTempWidgets());
					_widgetCon->ClearTempWidgets();
				}
				else
				{
					_widgetCon->CreateWidget<Pg::UI::Widget::TreeNode>(folderName, _widgetCon->GetTreeNodeWidgets());
					depth--;
				}
				_widgetCon->ClearTreeNodeWidget();
			}
		}
	}
}

Pg::Editor::Window::FilterData Pg::Editor::Window::Filter::SeparateFiles(std::string path, std::string folderName)
{
	FilterData files;
	std::vector<std::string> fileNames;

	for (const auto& file : fs::directory_iterator(path))
	{
		// fileName에서 '.'(확장자)가 없다는 것은 폴더라는 것
		if (file.path().string().find(".") == std::string::npos) files.insert({ file.path().string(), fileNames});
		else
		{
			if (folderName != file.path().filename().string())
			{
				files[folderName].emplace_back(file.path().filename().string());
			}
		}
	}

	return files;
}

void Pg::Editor::Window::Filter::FileSelected()
{
	if (_isDoubleClicked != nullptr && *_isDoubleClicked)
	{
		std::string filePath = _dataContainer->GetProjectPath() + "Asset\\Scene\\" + _selectObject;

		std::unique_ptr<Pg::Editor::Event> sceneLoad = std::make_unique<Pg::Editor::Event>();
		sceneLoad->Invoke(eEventType::_LOADSCENE, static_cast<void*>(&filePath));
	}
}

std::string Pg::Editor::Window::Filter::GetWindowName()
{
	return _winName;
}

void Pg::Editor::Window::Filter::SetDisable(bool disable)
{

}