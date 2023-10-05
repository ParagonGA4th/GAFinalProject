#include "ImGuiManager.h"
#include "DataStruct.h"

#include <cstdlib>
#include <filesystem>

namespace fs = std::filesystem;

ImGuiManager::ImGuiManager(std::vector<GameObjectData*> data)
	:_gameObjectDatas(data), _gameObjectData(), _objectSelectedNumber(1), _objectSelected(true)
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

	_gameObjectData = _gameObjectDatas.front();
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
	//ImGui::ShowDemoWindow(&_active);
	
	ImGui::Begin("DemoInspector", NULL, ImGuiWindowFlags_NoCollapse);
	if (ImGui::IsWindowHovered(ImGuiHoveredFlags_None) && ImGui::IsMouseClicked(0)) {
		// 창을 클릭한 경우의 동작
		// 여기에 원하는 코드를 추가하세요.

		ImGui::SetWindowFocus("DemoInspector");
	}
	
	static char nameBuf[256];
	static char tagBuf[256];
	strcpy(nameBuf, _gameObjectData->_name.c_str());
	strcpy(tagBuf, _gameObjectData->_tag.c_str());

	if (ImGui::BeginTable("table1", 2))
	{
		for (int row = 0; row < 3; row++)
		{
			ImGui::TableNextRow();
			for (int column = 0; column < 2; column++)
			{
				ImGui::TableSetColumnIndex(column);
				if(row == 0 && column == 0)	ImGui::Text("Name");
				else if(row == 0 && column == 1)
				{
					//if (ImGui::GetIO().KeysDown[ImGuiKey_A]) {}

					if (ImGui::InputText("##Name", nameBuf, IM_ARRAYSIZE(nameBuf), ImGuiInputTextFlags_EnterReturnsTrue))
					{
						_gameObjectData->_name = nameBuf;
					}
				}

				if(row == 1 && column == 0)	ImGui::Text("Tag");
				else if(row == 1 && column == 1)
				{
					//if (ImGui::GetIO().KeysDown[ImGuiKey_A]) {}

					if (ImGui::InputText("##Tag", tagBuf, IM_ARRAYSIZE(tagBuf), ImGuiInputTextFlags_EnterReturnsTrue))
					{
						_gameObjectData->_tag = tagBuf;
					}
				}

				if(row == 2 && column == 0)	ImGui::Text("Active");
				else if (row == 2 && column == 1) ImGui::Checkbox("##Active", &_active);
			}
		}
		ImGui::EndTable();
	}

	if (ImGui::CollapsingHeader("Transform"), true)
	{
		ImGui::Text("Position");
		ImGui::SameLine();
		ImGui::InputFloat3("##Position", position);

		ImGui::Text("Rotation");
		ImGui::SameLine();
		ImGui::InputFloat3("##Rotation", rotation);

		ImGui::Text("Scale");
		ImGui::SameLine();
		ImGui::InputFloat3("##Scale", sacle);
	}

	ImGui::End();
}

void ImGuiManager::ShowDemoHierarchy()
{
	ImGui::Begin("DemoHierarchy", NULL, ImGuiWindowFlags_NoCollapse);

	// Scene = Tree Nodes
	// Normal GameObject = Selectable
	// Group GameObject = Tree Nodes

	// 오브젝트를 가져와서 그룹으로 만들 수 있어야 한다. -> Drag And Drop

	//for (auto& i : _gameObjectDatas)
	//{
	//	if (ImGui::Selectable(i->_name.c_str(), i->_objectNumber == _objectClicked))
	//	{
	//		_gameObjectData = i;
	//		_objectClicked = i->_objectNumber;
	//	}
	//}

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
	
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->GetWorkCenter());
	ImGui::SetNextWindowViewport(viewport->ID);
	//ImGui::SetWindowViewport()
	
	ImGui::End();
}

void ImGuiManager::Render()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
}

void ImGuiManager::Finalize()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}
