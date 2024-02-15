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

void Pg::UI::Manager::UIManager::SetWindowSizeForGizmo()
{
	_imguiManager->SetGizmoSize();
}

void Pg::UI::Manager::UIManager::DrawGizmo()
{
	_imguiManager->DrawGizmo();
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