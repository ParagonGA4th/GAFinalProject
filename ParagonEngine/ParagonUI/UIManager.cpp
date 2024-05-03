#include "UIManager.h"
#include "ImGuiManager.h"

Pg::UI::Manager::UIManager::UIManager()
{
	_imguiManager = std::make_unique<Pg::UI::Manager::ImGuiManager>();
}

Pg::UI::Manager::UIManager::~UIManager()
{
}

void Pg::UI::Manager::UIManager::Initialize(void* hWnd, ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	_imguiManager->Initialize(hWnd, device, deviceContext);
}

void Pg::UI::Manager::UIManager::Update()
{
	_imguiManager->CreateFrame();
}

void Pg::UI::Manager::UIManager::LastUpdate()
{
	_imguiManager->Render();
}

void Pg::UI::Manager::UIManager::Finalize()
{
	_imguiManager->Finalize();
}

void Pg::UI::Manager::UIManager::UIHandler(MSG message)
{
	_imguiManager->ImguiHandler(message);                                             
}

void Pg::UI::Manager::UIManager::SetCameraForGizmo(Pg::Data::Camera* camera)
{
	_imguiManager->SetGizmoCamera(camera);
}

void Pg::UI::Manager::UIManager::SetTransformForGizmo(Pg::Data::Transform* trans)
{
	_imguiManager->SetGizmoTransform(trans);
}

void Pg::UI::Manager::UIManager::SetGizmoType(void* gizmoType)
{
	if (gizmoType == nullptr) _gizmoType = "Translate";
	else if (gizmoType != nullptr) _gizmoType = *(static_cast<std::string*>(gizmoType));
}

void Pg::UI::Manager::UIManager::DrawGizmo()
{
	_imguiManager->DrawGizmo(_gizmoType);
}

void Pg::UI::Manager::UIManager::WindowBegin(std::string winName, bool isTool)
{
	_imguiManager->Begin(winName, isTool);
}

void Pg::UI::Manager::UIManager::WindowEnd(bool isDockspace)
{
	_imguiManager->End(isDockspace);
}

void Pg::UI::Manager::UIManager::WindowDockSpaceBegin(std::string winName)
{
	_imguiManager->DockSpaceBegin(winName);
}

void Pg::UI::Manager::UIManager::SetAlignCenter(int widgetCount, float widgetWidth)
{
	_imguiManager->AlignForWidth(widgetCount, widgetWidth);
}

void Pg::UI::Manager::UIManager::BeginDisable(bool disable)
{
	_imguiManager->BeginDisable(disable);
}

void Pg::UI::Manager::UIManager::EndDisable()
{
	_imguiManager->EndDisable();
}

float Pg::UI::Manager::UIManager::GetMousePosX()
{
	return _imguiManager->GetMousePosX();
}

float Pg::UI::Manager::UIManager::GetMousePosY()
{
	return _imguiManager->GetMousePosY();
}

float Pg::UI::Manager::UIManager::GetWindowWidth()
{
	return _imguiManager->GetWindowWidth();
}

float Pg::UI::Manager::UIManager::GetWindowHeight()
{
	return _imguiManager->GetWindowHeight();
}

float Pg::UI::Manager::UIManager::GetWindowTitleBarHeight()
{
	return _imguiManager->GetWindowTitleBarHeight();
}


void Pg::UI::Manager::UIManager::OpenPopup(std::string popupName)
{
	_imguiManager->OpenPopup(popupName);
}

float Pg::UI::Manager::UIManager::GetWindowPosX()
{
	return _imguiManager->GetWindowPosX();
}

float Pg::UI::Manager::UIManager::GetWindowPosY()
{
	return _imguiManager->GetWindowPosY();
}