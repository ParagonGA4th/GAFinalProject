#include "ImGuiManager.h"
#include <string>
#include <map>
#include <vector>

ImGuiManager::ImGuiManager()
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

	//float fontSize = 18.0f;// *2.0f;
	//io.Fonts->AddFontFromFileTTF("assets/fonts/opensans/OpenSans-Bold.ttf", fontSize);
	//io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/opensans/OpenSans-Regular.ttf", fontSize);

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();

	// 색상 변경 예시
	style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // 텍스트 색상을 빨간색으로 설정
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f); // 텍스트 색상을 빨간색으로 설정
	 
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}
}


void ImGuiManager::CreateFrame()
{
	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

char* nameBuf = new char();
char* TagBuf = new char();
bool active = false;

float position = 0.0f;
float rotation = 0.0f;
float sacle = 0.0f;

void ImGuiManager::ShowDemoInspector()
{
	ImGui::Begin("DemoInspector", NULL, ImGuiWindowFlags_NoCollapse);

	ImGui::Text("Name      ");
	ImGui::SameLine();
	ImGui::InputText("##NameInput", nameBuf, sizeof(nameBuf));
	
	ImGui::Text("Tag       ");
	ImGui::SameLine();
	ImGui::InputText("##TagInput", TagBuf, sizeof(TagBuf));

	
	ImGui::Text("Active    ");
	ImGui::SameLine();
	ImGui::Checkbox("##ActiveInput", &active);


	if (ImGui::CollapsingHeader("Transform"))
	{
		ImGui::Text("Position  ");
		ImGui::SameLine();
		ImGui::InputFloat3("##PositionInput", &position);

		ImGui::Text("Rotation  ");
		ImGui::SameLine();
		ImGui::InputFloat3("##RotationInput", &rotation);

		ImGui::Text("Scale     ");
		ImGui::SameLine();
		ImGui::InputFloat3("##ScaleInput", &sacle);
	}

	ImGui::End();
}

int count = 0;
int itemClicked = -1;

void ImGuiManager::ShowDemoHierarchy()
{
	ImGui::Begin("DemoHierarchy", NULL, ImGuiWindowFlags_NoCollapse);

	if (ImGui::Button("AddObject", ImVec2(100, 30)))
	{
		count++;
	}

	for (int i = 0; i < count; i++)
	{
		if(ImGui::Selectable(std::to_string(i).c_str(), i == itemClicked)) itemClicked = i;
	}

	ImGui::End();
}

void ImGuiManager::ShowDemoFilter()
{
	ImGui::Begin("DemoProjectFilter", NULL, ImGuiWindowFlags_NoCollapse);

	if (ImGui::TreeNode("Basic trees"))
	{
		for (int i = 0; i < 5; i++)
		{
			if (ImGui::Selectable(std::to_string(i).c_str(), i == itemClicked)) itemClicked = i;
		}
		ImGui::TreePop();
	}

	ImGui::End();
}

void ImGuiManager::Render()
{
	ImGui::Render();
	ImGui::UpdatePlatformWindows();
	ImGui::RenderPlatformWindowsDefault();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiManager::Finalize()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}
