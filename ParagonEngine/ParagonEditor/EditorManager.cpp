#include "EditorManager.h"
#include "EditorHelper.h"
#include "../ParagonUI/UIManager.h"

#include <singleton-cpp/singleton.h>

#ifdef _DEBUG
#pragma comment(lib,"..\\Builds\\x64\\Debug\\ParagonUI.lib")
#else
#pragma comment(lib,"..\\Builds\\x64\\Release\\ParagonUI.lib")
#endif // _DEBUG

Pg::Editor::Manager::EditorManager::EditorManager()
	:_isUIInitailized(false)
{
	auto& tEditorHelper = singleton<Pg::Editor::Helper::EditorHelper>();
	_edHepler = &tEditorHelper;

	// UI
	_uiManager = std::make_unique<Pg::UI::Manager::UIManager>();

	// Editor event
	// Editor window
}

Pg::Editor::Manager::EditorManager::~EditorManager()
{

}

void Pg::Editor::Manager::EditorManager::Initialize(HWND hWnd)
{
	_uiManager->Initialize(static_cast<void*>(hWnd), _edHepler->GetDevice(), _edHepler->GetDeviceContext());
}

void Pg::Editor::Manager::EditorManager::Update()
{
	_uiManager->Update();
}

void Pg::Editor::Manager::EditorManager::LastUpdate()
{
	_uiManager->LastUpdate();
}

void Pg::Editor::Manager::EditorManager::Finalize()
{
	_uiManager->Finalize();
}


void Pg::Editor::Manager::EditorManager::UIHandler(MSG message)
{
	_uiManager->UIHandler(message);
}

