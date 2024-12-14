#include "EditorManager.h"
#include "DataContainer.h"
#include "WindowManager.h"

#include <singleton-cpp/singleton.h>

Pg::Editor::Manager::EditorManager::EditorManager()
{
	auto& tdataCon = singleton<Pg::Editor::Data::DataContainer>();
	_dataContainer = &tdataCon;

	_windowManager = std::make_unique<Pg::Editor::Manager::WindowManager>();
}

Pg::Editor::Manager::EditorManager::~EditorManager()
{
}

void Pg::Editor::Manager::EditorManager::Initialize(void* hWnd)
{
	_windowManager->Initialize(hWnd);
}

void Pg::Editor::Manager::EditorManager::Update()
{
	if (_dataContainer->GetEditorOnOff()) _windowManager->Update();
}

void Pg::Editor::Manager::EditorManager::LateUpdate()
{
}

void Pg::Editor::Manager::EditorManager::Finalize()
{
	_windowManager->Finalize();
}

void Pg::Editor::Manager::EditorManager::ManagerHandler(MSG message)
{
	_windowManager->WindowHandler(message);
}