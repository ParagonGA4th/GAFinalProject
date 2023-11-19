#include "WindowManager.h"
#include "DataContainer.h"

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
	_inspector = std::make_unique<Pg::Editor::Window::Inspector>();
	_hierarchy = std::make_unique<Pg::Editor::Window::Hierarchy>();
	_scene = std::make_unique<Pg::Editor::Window::Scene>();
	_filter = std::make_unique<Pg::Editor::Window::Filter>();
}

Pg::Editor::Manager::WindowManager::~WindowManager()
{

}

void Pg::Editor::Manager::WindowManager::Initialize(HWND hWnd)
{
	_uiManager->Initialize(static_cast<void*>(hWnd), _dataContainer->GetDevice(), _dataContainer->GetDeviceContext());
	_inspector->Initialize();
	_hierarchy->Initialize();
	_scene->Initialize();
	_filter->Initialize();
}

void Pg::Editor::Manager::WindowManager::Update()
{
	_uiManager->Update(_dataContainer->GetSceneTexture());
	//_inspector->Update();
	_hierarchy->Update();
	//_scene->Update();
	//_filter->Update();
}

void Pg::Editor::Manager::WindowManager::LastUpdate()
{
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

