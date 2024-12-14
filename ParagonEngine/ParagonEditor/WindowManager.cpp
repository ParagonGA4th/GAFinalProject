#include "WindowManager.h"
#include "DataContainer.h"
#include "Event.h"

#include "IEditorWindow.h"
#include "Layout.h"
#include "ToolBar.h"
#include "Hierarchy.h"
#include "Inspector.h"
#include "DontDestroy.h"
#include "Scene.h"
#include "Filter.h"

#include "../ParagonUI/UIManager.h"

#include "../ParagonData/Scene.h"

#include <memory>
#include <algorithm>
#include <singleton-cpp/singleton.h>

Pg::Editor::Manager::WindowManager::WindowManager()
	:_isDisable(false)
{
	// editor helper
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
	_windows.emplace_back(new Pg::Editor::Window::DontDestroy());
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

	std::unique_ptr<Pg::Editor::Event> _windowAble = std::make_unique<Pg::Editor::Event>();
	_windowAble->AddEvent(Pg::Editor::eEventType::_EDITORDISABLE, { [&](void* data) { WindowAble(data); }});
}

void Pg::Editor::Manager::WindowManager::Update()
{
	_uiManager->Update();
	for_each(_windows.begin(), _windows.end(),
		[&](Pg::Editor::Window::IEditorWindow* ewindow) 
		{
			ewindow->SetDisable(_isDisable);
			if (ewindow->GetShow()) ewindow->Update();
		});
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

void Pg::Editor::Manager::WindowManager::WindowAble(void* disable)
{
	_isDisable = *(static_cast<bool*>(disable));
}

