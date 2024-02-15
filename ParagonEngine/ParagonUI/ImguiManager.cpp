#include "ImGuiManager.h"

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#include "Gizmo.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Pg::UI::Manager::ImGuiManager::ImGuiManager()
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;		// Enable Gamepad Controls 
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

	//io.DisplaySize = ImVec2(1920, 1080);

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();

	// 색상 변경 예시
	//style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // 텍스트 색상을 하얀색으로 설정
	//style.Colors[ImGuiCol_FrameBg] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f); // 프레임 색상을 진회색으로 설정

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	_imGizmo = std::make_unique<Pg::UI::Helper::Gizmo>();
}

Pg::UI::Manager::ImGuiManager::~ImGuiManager()
{

}

void Pg::UI::Manager::ImGuiManager::Initialize(void* hWnd, ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(device, deviceContext);
}

void Pg::UI::Manager::ImGuiManager::CreateFrame()
{
	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	_imGizmo->CreateFrame();
}

void Pg::UI::Manager::ImGuiManager::Render()
{
	ImGui::Render();
	//_deviceContext->OMSetRenderTargets(1, &_renderTarget, nullptr);
	//const float clear_color_with_alpha[4] = { 0,0,0,0 };
	//_deviceContext->ClearRenderTargetView(_renderTarget, clear_color_with_alpha);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
}

void Pg::UI::Manager::ImGuiManager::Finalize()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void Pg::UI::Manager::ImGuiManager::ImguiHandler(MSG message)
{
	ImGui_ImplWin32_WndProcHandler(message.hwnd, message.message, message.wParam, message.lParam);
}

void Pg::UI::Manager::ImGuiManager::Begin(std::string panelName, bool isTool)
{
	if(!isTool) ImGui::Begin(panelName.c_str(), NULL, ImGuiWindowFlags_NoCollapse);
	else ImGui::Begin(panelName.c_str(), &isTool, ImGuiWindowFlags_NoCollapse);
}

void Pg::UI::Manager::ImGuiManager::DockSpaceBegin(std::string dockName)
{
	ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus;

	ImGui::Begin("DockSpace Demo", nullptr, window_flags);
	ImGui::PopStyleVar(3);
	
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}
}

void Pg::UI::Manager::ImGuiManager::End(bool isDockspace)
{
	ImGui::End();
}

void Pg::UI::Manager::ImGuiManager::SetGizmoCamera(Pg::Data::Camera* camera)
{
	_imGizmo->SetCamera(camera);
}

void Pg::UI::Manager::ImGuiManager::SetGizmoTransform(Pg::Data::Transform* trans)
{
	_imGizmo->SetTransform(trans);
}

void Pg::UI::Manager::ImGuiManager::SetGizmoSize()
{
	ImVec2 pos = ImGui::GetWindowPos();
	ImVec2 size = ImGui::GetWindowSize();
	_imGizmo->SetWindowSize(pos.x, pos.y, size.x, size.y);
}

void Pg::UI::Manager::ImGuiManager::DrawGizmo()
{
	_imGizmo->DrawGizmo();
}
