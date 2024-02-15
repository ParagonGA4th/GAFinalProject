#include "WindowManager.h"
#include "DataContainer.h"

#include "IEditorWindow.h"
#include "Layout.h"
#include "ToolBar.h"
#include "Hierarchy.h"
#include "Inspector.h"
#include "Scene.h"
#include "Filter.h"

#include "../ParagonUI/UIManager.h"

#include "../ParagonData/Scene.h"

#include <algorithm>
#include <singleton-cpp/singleton.h>

Pg::Editor::Manager::WindowManager::WindowManager()
{
	auto& tdataCon = singleton<Pg::Editor::Data::DataContainer>();
	_dataContainer = &tdataCon;

	// UI
	auto& tUIManager = singleton<Pg::UI::Manager::UIManager>();
	_uiManager = &tUIManager;

	// Editor window

	_windows.emplace_back(new Pg::Editor::Window::Layout());
	_windows.emplace_back(new Pg::Editor::Window::ToolBar());
	_windows.emplace_back(new Pg::Editor::Window::Hierarchy());
	_windows.emplace_back(new Pg::Editor::Window::Inspector());
	_windows.emplace_back(new Pg::Editor::Window::Scene());
	_windows.emplace_back(new Pg::Editor::Window::Filter());
}

Pg::Editor::Manager::WindowManager::~WindowManager()
{

}

void Pg::Editor::Manager::WindowManager::Initialize(void* hWnd)
{
	_uiManager->Initialize(hWnd, _dataContainer->GetDevice(),_dataContainer->GetDeviceContext());
	for_each(_windows.begin(), _windows.end(), 
		[](Pg::Editor::Window::IEditorWindow* ewindow) { ewindow->Initialize(); });
}

void Pg::Editor::Manager::WindowManager::Update()
{
	_uiManager->Update();
	for_each(_windows.begin(), _windows.end(), 
		[](Pg::Editor::Window::IEditorWindow* ewindow) { if(ewindow->GetShow()) ewindow->Update(); });
	_uiManager->LastUpdate();
}

void Pg::Editor::Manager::WindowManager::Finalize()
{
	_uiManager->Finalize();
}

void Pg::Editor::Manager::WindowManager::WindowHandler(MSG message)
{
	_uiManager->UIHandler(message);
}

