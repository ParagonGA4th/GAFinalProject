#include "ImGuiManager.h"
#include <string>
#include <cstdlib>
#include <filesystem>

namespace fs = std::filesystem;

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


	// 한글 폰트
	const char* koreanFontPath = "..\\font\\NotoSansKR-Regular.ttf";
	io.Fonts->AddFontFromFileTTF(koreanFontPath, 18.0f, NULL, io.Fonts->GetGlyphRangesKorean());


	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();

	// 색상 변경 예시
	style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // 텍스트 색상을 하얀색으로 설정
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f); // 프레임 색상을 회색으로 설정
	
	 
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

	ImGui::Text("한글 테스트");

	const char* projectPath = std::getenv("PROJECT_PATH");

	// 재귀함수 만들기
	if (ImGui::TreeNode("Basic trees"))
	{
		for (const auto& file : fs::directory_iterator(projectPath))
		{
			if (file.path().filename().string().rfind(".") == std::string::npos)
			{
				if (ImGui::TreeNode(file.path().filename().string().c_str()))
				{
					std::string folderPath = file.path().string();

					for (const auto& ffile : fs::directory_iterator(folderPath))
					{
						if (ffile.path().filename().string().rfind(".") == std::string::npos)
						{

						}
						else
						{
							ImGui::Selectable(ffile.path().filename().string().c_str(), false);
						}
					}	
					ImGui::TreePop();
				}
			}
			else
			{
				ImGui::Selectable(file.path().filename().string().c_str(), false);
			}
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
