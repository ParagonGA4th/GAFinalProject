#include "WindowManager.h"
#include "DataContainer.h"


#include "IEditorWindow.h"
#include "Inspector.h"
#include "Hierarchy.h"
#include "Scene.h"
#include "Filter.h"


#include "../ParagonUI/UIManager.h"


#include <singleton-cpp/singleton.h>

Pg::Editor::Manager::WindowManager::WindowManager()
{
	auto& tdataCon = singleton<Pg::Editor::Data::DataContainer>();
	_dataContainer = &tdataCon;

	// UI
	auto& tUIManager = singleton<Pg::UI::Manager::UIManager>();
	_uiManager = &tUIManager;

	// Editor window

	_windows.emplace_back(new Pg::Editor::Window::Inspector());
	_windows.emplace_back(new Pg::Editor::Window::Hierarchy());
	_windows.emplace_back(new Pg::Editor::Window::Scene());
	_windows.emplace_back(new Pg::Editor::Window::Filter());
}

Pg::Editor::Manager::WindowManager::~WindowManager()
{

}

void Pg::Editor::Manager::WindowManager::Initialize(HWND hWnd)
{
	_uiManager->Initialize(static_cast<void*>(hWnd), _dataContainer->GetDevice(), _dataContainer->GetDeviceContext());
	for (auto& window : _windows)
	{
		window->Initialize();
	}
}

void Pg::Editor::Manager::WindowManager::Update()
{
	_uiManager->Update();
	for (auto& window : _windows)
	{
		if (window->GetShow()) window->Update();
	}
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

