#include "EditorManager.h"
#include "EditorHelper.h"

#include "Inspector.h"

#include "../ParagonUI/UIManager.h"

#include <singleton-cpp/singleton.h>

Pg::Editor::Manager::EditorManager::EditorManager()
	:_editorOnOff(false)
{
	auto& tEditorHelper = singleton<Pg::Editor::Helper::EditorHelper>();
	_edHepler = &tEditorHelper;

	// UI
	auto& tUIManager = singleton<Pg::UI::Manager::UIManager>();
	_uiManager = &tUIManager;

	// Editor event
	// Editor window
	_inspector = std::make_unique<Pg::Editor::Window::Inspector>();

}

Pg::Editor::Manager::EditorManager::~EditorManager()
{

}

void Pg::Editor::Manager::EditorManager::Initialize(HWND hWnd)
{
	_uiManager->Initialize(static_cast<void*>(hWnd), _edHepler->GetDevice(), _edHepler->GetDeviceContext());
	_inspector->Initialize();
}

void Pg::Editor::Manager::EditorManager::Update()
{
	if (_edHepler->GetEditorOnOff()) _editorOnOff = !_editorOnOff;

	if (_editorOnOff)
	{
		_uiManager->Update(_edHepler->GetTexture());
		_inspector->Update();
	}
}

void Pg::Editor::Manager::EditorManager::LastUpdate()
{
	if (_editorOnOff) _uiManager->LastUpdate();
}

void Pg::Editor::Manager::EditorManager::Finalize()
{
	_uiManager->Finalize();
}


void Pg::Editor::Manager::EditorManager::UIHandler(MSG message)
{
	_uiManager->UIHandler(message);
}

