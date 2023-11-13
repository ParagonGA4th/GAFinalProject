#include "Filter.h"
#include "../ParagonUI/UIManager.h"
#include "../ParagonUI/WidgetContainer.h"
#include <singleton-cpp/singleton.h>

#include "../ParagonUI/Selectable.h"
#include "../ParagonUI/TreeNode.h"

#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

Pg::Editor::Window::Filter::Filter()
	:_winName("Project"), _isShow(true)
{
	auto& tUIManager = singleton<Pg::UI::Manager::UIManager>();
	_uiManager = &tUIManager;

	cons = new Pg::UI::WidgetContainer();
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

	//TestFilter(_firstPath);
}

void Pg::Editor::Window::Filter::Update()
{
	_uiManager->WindowBegin(_winName);
	cons->Update();
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

void Pg::Editor::Window::Filter::TestFilter(std::string path)
{
	std::vector<std::string> fileNames;
	std::string folderPath;
	std::string fileName;

	bool isAllFile = true;

	//for (const auto& file : fs::directory_iterator(path))
	//{
	//	fileName = file.path().filename().string();

	//	if (fileName.rfind(".") == std::string::npos)
	//	{
	//		isAllFile = false;

	//		folderPath = file.path().string();
	//		TestFilter(folderPath);


	//		if (_firstPath.compare(path) != 0) cons->CreateTreeNodeWidget<Pg::UI::Widget::TreeNode>(fileName.c_str(), cons->GetTreeNodeWidgets());
	//		else cons->CreateWidget<Pg::UI::Widget::TreeNode>(fileName.c_str(), cons->GetTreeNodeWidgets());
	//
	//		cons->ClearTreeNodeWidget();
	//	}
	//	else
	//	{
	//		if(isAllFile) if (fileName.compare(".vs") != 0) fileNames.emplace_back(fileName);
	//	}
	//}

	//if(isAllFile) cons->CreateTreeNodeWidget<Pg::UI::Widget::Selectable>(fileNames);
	//fileNames.clear();
}
