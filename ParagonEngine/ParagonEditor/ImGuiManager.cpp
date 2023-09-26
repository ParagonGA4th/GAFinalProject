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
	//const char* koreanFontPath = "..\\font\\NotoSansKR-Regular.ttf";
	//io.Fonts->AddFontFromFileTTF(koreanFontPath, 18.0f, NULL, io.Fonts->GetGlyphRangesKorean());
	//bool fontBuilt=io.Fonts->Build();
	//assert(fontBuilt);


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


void ImGuiManager::ShowDemoInspector()
{
	ImGui::Begin("DemoInspector", NULL, ImGuiWindowFlags_NoCollapse);

	static char nameBuf[255] = "";
	ImGui::Text("Name");
	ImGui::SameLine();
	bool re = ImGui::InputText("##NameInput", nameBuf, IM_ARRAYSIZE(nameBuf), ImGuiInputTextFlags_EnterReturnsTrue);

	if (re)
	{
		re = false;
	}

	//ImGui::Text("Tag");
	//ImGui::SameLine();
	//ImGui::InputText("##TagInput", TagBuf, 255);

	
	ImGui::Text("Active");
	ImGui::SameLine();
	ImGui::Checkbox("##ActiveInput", &active);


	if (ImGui::CollapsingHeader("Transform"), true)
	{
		ImGui::Text("Position");
		ImGui::SameLine();
		ImGui::InputFloat3("##PositionInput", position);

		ImGui::Text("Rotation");
		ImGui::SameLine();
		ImGui::InputFloat3("##RotationInput", rotation);

		ImGui::Text("Scale");
		ImGui::SameLine();
		ImGui::InputFloat3("##ScaleInput", sacle);
	}

	ImGui::End();
}

int itemClicked = -1;

void ImGuiManager::ShowDemoHierarchy()
{
	ImGui::Begin("DemoHierarchy", NULL, ImGuiWindowFlags_NoCollapse);

	// Scene = Tree Nodes
	// Normal GameObject = Selectable
	// Group GameObject = Tree Nodes

	// 오브젝트를 가져와서 그룹으로 만들 수 있어야 한다. -> Drag And Drop

	/*for (int i = 0; i < count; i++)
	{
		if(ImGui::Selectable(std::to_string(i).c_str(), i == itemClicked)) itemClicked = i;
	}*/

	ImGui::End();
}

void ImGuiManager::ShowDemoFilter()
{
	ImGui::Begin("DemoProjectFilter", NULL, ImGuiWindowFlags_NoCollapse);

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


void ImGuiManager::ShowDemoViewPort()
{
	ImGui::Begin("DemoScene", NULL, ImGuiWindowFlags_NoCollapse);
	
	
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
