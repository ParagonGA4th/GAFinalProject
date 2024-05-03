#include "ImGuiManager.h"

#include "imgui.h"
#include "imgui_internal.h"
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

	_imGizmo = std::make_unique<Pg::UI::Helper::Gizmo>();

	//PretendardFont 사용.
	_pretendardFont = (void*)io.Fonts->AddFontFromFileTTF("../Resources/Fonts/TTF/Pretendard-Medium.ttf", 14.0f, NULL,
		io.Fonts->GetGlyphRangesDefault());
	IM_ASSERT(_pretendardFont != NULL);

	//스타일 바꾸기 (IMGUI)
	ChangeStyle();
}

Pg::UI::Manager::ImGuiManager::~ImGuiManager()
{

}

void Pg::UI::Manager::ImGuiManager::Initialize(void* hWnd, ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(device, deviceContext);

	_hwnd = (HWND)hWnd;
}

void Pg::UI::Manager::ImGuiManager::CreateFrame()
{
	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	_imGizmo->CreateFrame();

	//Pretendard Font Push
	//ImFont* tPreFont = (ImFont*)_pretendardFont;
	//ImGui::PushFont(tPreFont);
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
	if (!isTool) ImGui::Begin(panelName.c_str(), NULL, ImGuiWindowFlags_NoCollapse);
	else ImGui::Begin(panelName.c_str(), &isTool, ImGuiWindowFlags_NoCollapse);
}

void Pg::UI::Manager::ImGuiManager::DockSpaceBegin(std::string dockName)
{
	ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_NoCloseButton;
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

void Pg::UI::Manager::ImGuiManager::BeginDisable(bool disable)
{
	if (!disable) return;
	ImGui::BeginDisabled(disable);
}

void Pg::UI::Manager::ImGuiManager::EndDisable()
{
	ImGui::EndDisabled();
}

void Pg::UI::Manager::ImGuiManager::SetGizmoCamera(Pg::Data::Camera* camera)
{
	_imGizmo->SetCamera(camera);
}

void Pg::UI::Manager::ImGuiManager::SetGizmoTransform(Pg::Data::Transform* trans)
{
	_imGizmo->SetTransform(trans);
}

void Pg::UI::Manager::ImGuiManager::DrawGizmo(std::string type)
{
	_imGizmo->DrawGizmo(type);
}

void Pg::UI::Manager::ImGuiManager::AlignForWidth(int widgetCount, float widgetWidth)
{
	ImGuiStyle& style = ImGui::GetStyle();

	float width = (style.ItemSpacing.x * (widgetCount - 1)) + widgetWidth;
	float avail = ImGui::GetContentRegionAvail().x;
	float off = (avail - width) * 0.5f;

	if (off > 0.0f)
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);
}

void Pg::UI::Manager::ImGuiManager::ChangeStyle()
{
	// From : Trippasch
	// https://github.com/ocornut/imgui/issues/707#issuecomment-1372640066

	auto& colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.1f, 0.13f, 1.0f };
	colors[ImGuiCol_MenuBarBg] = ImVec4{ 0.16f, 0.16f, 0.21f, 1.0f };

	// Border
	colors[ImGuiCol_Border] = ImVec4{ 0.44f, 0.37f, 0.61f, 0.29f };
	colors[ImGuiCol_BorderShadow] = ImVec4{ 0.0f, 0.0f, 0.0f, 0.24f };

	// Text
	colors[ImGuiCol_Text] = ImVec4{ 1.0f, 1.0f, 1.0f, 1.0f };
	colors[ImGuiCol_TextDisabled] = ImVec4{ 0.5f, 0.5f, 0.5f, 1.0f };

	// Headers
	colors[ImGuiCol_Header] = ImVec4{ 0.13f, 0.13f, 0.17, 1.0f };
	colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.19f, 0.2f, 0.25f, 1.0f };
	colors[ImGuiCol_HeaderActive] = ImVec4{ 0.16f, 0.16f, 0.21f, 1.0f };

	// Buttons
	colors[ImGuiCol_Button] = ImVec4{ 0.13f, 0.13f, 0.17, 1.0f };
	colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.19f, 0.2f, 0.25f, 1.0f };
	colors[ImGuiCol_ButtonActive] = ImVec4{ 0.16f, 0.16f, 0.21f, 1.0f };
	colors[ImGuiCol_CheckMark] = ImVec4{ 0.74f, 0.58f, 0.98f, 1.0f };

	// Popups
	colors[ImGuiCol_PopupBg] = ImVec4{ 0.1f, 0.1f, 0.13f, 0.92f };

	// Slider
	colors[ImGuiCol_SliderGrab] = ImVec4{ 0.44f, 0.37f, 0.61f, 0.54f };
	colors[ImGuiCol_SliderGrabActive] = ImVec4{ 0.74f, 0.58f, 0.98f, 0.54f };

	// Frame BG
	colors[ImGuiCol_FrameBg] = ImVec4{ 0.13f, 0.13, 0.17, 1.0f };
	colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.19f, 0.2f, 0.25f, 1.0f };
	colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.16f, 0.16f, 0.21f, 1.0f };

	// Tabs
	colors[ImGuiCol_Tab] = ImVec4{ 0.16f, 0.16f, 0.21f, 1.0f };
	colors[ImGuiCol_TabHovered] = ImVec4{ 0.24, 0.24f, 0.32f, 1.0f };
	colors[ImGuiCol_TabActive] = ImVec4{ 0.2f, 0.22f, 0.27f, 1.0f };
	colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.16f, 0.16f, 0.21f, 1.0f };
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.16f, 0.16f, 0.21f, 1.0f };

	// Title
	colors[ImGuiCol_TitleBg] = ImVec4{ 0.16f, 0.16f, 0.21f, 1.0f };
	colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.16f, 0.16f, 0.21f, 1.0f };
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.16f, 0.16f, 0.21f, 1.0f };

	// Scrollbar
	colors[ImGuiCol_ScrollbarBg] = ImVec4{ 0.1f, 0.1f, 0.13f, 1.0f };
	colors[ImGuiCol_ScrollbarGrab] = ImVec4{ 0.16f, 0.16f, 0.21f, 1.0f };
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4{ 0.19f, 0.2f, 0.25f, 1.0f };
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4{ 0.24f, 0.24f, 0.32f, 1.0f };

	// Seperator
	colors[ImGuiCol_Separator] = ImVec4{ 0.44f, 0.37f, 0.61f, 1.0f };
	colors[ImGuiCol_SeparatorHovered] = ImVec4{ 0.74f, 0.58f, 0.98f, 1.0f };
	colors[ImGuiCol_SeparatorActive] = ImVec4{ 0.84f, 0.58f, 1.0f, 1.0f };

	// Resize Grip
	colors[ImGuiCol_ResizeGrip] = ImVec4{ 0.44f, 0.37f, 0.61f, 0.29f };
	colors[ImGuiCol_ResizeGripHovered] = ImVec4{ 0.74f, 0.58f, 0.98f, 0.29f };
	colors[ImGuiCol_ResizeGripActive] = ImVec4{ 0.84f, 0.58f, 1.0f, 0.29f };

	// Docking
	colors[ImGuiCol_DockingPreview] = ImVec4{ 0.44f, 0.37f, 0.61f, 1.0f };

	auto& style = ImGui::GetStyle();
	style.TabRounding = 4;
	style.ScrollbarRounding = 9;
	style.WindowRounding = 7;
	style.GrabRounding = 3;
	style.FrameRounding = 3;
	style.PopupRounding = 4;
	style.ChildRounding = 4;
}

float Pg::UI::Manager::ImGuiManager::GetMousePosX()
{
	POINT cursorPos;
	GetCursorPos(&cursorPos);
	ScreenToClient(_hwnd, &cursorPos);
	return cursorPos.x;
}

float Pg::UI::Manager::ImGuiManager::GetMousePosY()
{
	POINT cursorPos;
	GetCursorPos(&cursorPos);
	ScreenToClient(_hwnd, &cursorPos);
	return cursorPos.y;
}

float Pg::UI::Manager::ImGuiManager::GetWindowPosX()
{
	POINT cursorPos = { ImGui::GetWindowPos().x, ImGui::GetWindowPos().y };
	ScreenToClient(_hwnd, &cursorPos);
	return cursorPos.x;
}

float Pg::UI::Manager::ImGuiManager::GetWindowPosY()
{
	POINT cursorPos = { ImGui::GetWindowPos().x, ImGui::GetWindowPos().y };
	ScreenToClient(_hwnd, &cursorPos);
	return cursorPos.y;
}


float Pg::UI::Manager::ImGuiManager::GetWindowWidth()
{
	if (ImGui::GetCurrentContext()->HoveredWindow == NULL)
	{
		if (!_contextInit) return _isNotWorking;
		else return _windowSizeX;
	}

	if (std::strcmp(ImGui::GetCurrentContext()->HoveredWindow->Name, "Scene") == 0)
	{
		_windowSizeX = ImGui::GetCurrentContext()->HoveredWindow->Size.x;
		_contextInit = true;
	}
}

float Pg::UI::Manager::ImGuiManager::GetWindowHeight()
{
	if (ImGui::GetCurrentContext()->HoveredWindow == NULL)
	{
		if (!_contextInit) return _isNotWorking;
		else return _windowSizeY;
	}

	if (std::strcmp(ImGui::GetCurrentContext()->HoveredWindow->Name, "Scene") == 0)
	{
		_windowSizeY = ImGui::GetCurrentContext()->HoveredWindow->Size.y;
		_contextInit = true;
	}
}
\
float Pg::UI::Manager::ImGuiManager::GetWindowTitleBarHeight()
{
	if (ImGui::GetCurrentContext()->HoveredWindow == NULL)
	{
		if (!_contextInit) return _isNotWorking;
		else return _titleBarHeight;
	}

	if (std::strcmp(ImGui::GetCurrentContext()->HoveredWindow->Name, "Scene") == 0)
	{
		_titleBarHeight = ImGui::GetCurrentContext()->HoveredWindow->TitleBarHeight();
		_contextInit = true;
	}
}

void Pg::UI::Manager::ImGuiManager::OpenPopup(std::string popupName)
{
	ImGui::OpenPopup(popupName.c_str());
}