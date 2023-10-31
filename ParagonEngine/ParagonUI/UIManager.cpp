#include "UIManager.h"
#include "ImGuiManager.h"
#include "Panel.h"


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

void Pg::UI::Manager::UIManager::Update(void* Texture)
{
	_imguiManager->CreateFrame();
	_imguiManager->ViewTest(Texture);
}

void Pg::UI::Manager::UIManager::LastUpdate()
{
	_imguiManager->Render();
}

void Pg::UI::Manager::UIManager::Finalize()
{
	_imguiManager->Finalize();
}

//void Pg::UI::Manager::UIManager::SetFont()
//{
//
//}

void Pg::UI::Manager::UIManager::UIHandler(MSG message)
{
	_imguiManager->ImguiHandler(message);                                             
}

void Pg::UI::Manager::UIManager::WindowBegin(std::string winName)
{
	_imguiManager->Begin(winName);
}

void Pg::UI::Manager::UIManager::WindowEnd()
{
	_imguiManager->End();
}
