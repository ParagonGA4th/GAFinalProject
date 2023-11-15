#include "EditorManager.h"
#include "DataContainer.h"

#include <singleton-cpp/singleton.h>

Pg::Editor::Manager::EditorManager::EditorManager()
{
	auto& tdataCon = singleton<Pg::Editor::Data::DataContainer>();
	_dataContainer = &tdataCon;


}

Pg::Editor::Manager::EditorManager::~EditorManager()
{

}

void Pg::Editor::Manager::EditorManager::Initialize(HWND hWnd)
{

}

void Pg::Editor::Manager::EditorManager::Update()
{
	if (!_dataContainer->GetEditorOnOff()) return;


}

void Pg::Editor::Manager::EditorManager::LastUpdate()
{
	if (!_dataContainer->GetEditorOnOff()) return;


}

void Pg::Editor::Manager::EditorManager::Finalize()
{

}


void Pg::Editor::Manager::EditorManager::UIHandler(MSG message)
{

}

