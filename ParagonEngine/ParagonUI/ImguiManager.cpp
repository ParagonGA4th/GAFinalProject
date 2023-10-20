#include "ImGuiManager.h"

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"


/// 임시
#include <filesystem>

namespace fs = std::filesystem;
/// 

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

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();

	// 색상 변경 예시
	style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // 텍스트 색상을 하얀색으로 설정
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f); // 프레임 색상을 진회색으로 설정

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}
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
}

void Pg::UI::Manager::ImGuiManager::Render()
{
	ImGui::Render();
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

void Pg::UI::Manager::ImGuiManager::ViewTest(void* Texture)
{
	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

	DemoInspector();
	//DemoHierarchy();
	//DemoFilter();
	//DemoViewPort(Texture);
}

void Pg::UI::Manager::ImGuiManager::DemoInspector()
{
	ImGui::Begin("DemoInspector", NULL, ImGuiWindowFlags_NoCollapse);

	static char name[256];
	static char tag[256];
	static bool active;

	static float position[3];
	static float rotation[3];
	static float scale[3];

	if (ImGui::BeginTable("ObjectInfo", 2))
	{
		for (int row = 0; row < 3; row++)
		{
			ImGui::TableNextRow();
			for (int column = 0; column < 2; column++)
			{
				ImGui::TableSetColumnIndex(column);
				if (row == 0 && column == 0)	ImGui::Text("Name");
				else if (row == 0 && column == 1)
				{
					if (ImGui::InputText("##Name", name, IM_ARRAYSIZE(name), ImGuiInputTextFlags_EnterReturnsTrue))
					{
						//_gameObjectDatas[_objectSelectedNumber - 1]->_name = _name;
					}
				}

				if (row == 1 && column == 0)	ImGui::Text("Tag");
				else if (row == 1 && column == 1)
				{
					if (ImGui::InputText("##Tag", tag, IM_ARRAYSIZE(tag), ImGuiInputTextFlags_EnterReturnsTrue))
					{
						//_gameObjectDatas[_objectSelectedNumber - 1]->_tag = _tag;
					}
				}

				if (row == 2 && column == 0)	ImGui::Text("Active");
				else if (row == 2 && column == 1) ImGui::Checkbox("##Active", &active);
			}
		}
		ImGui::EndTable();
	}

	if (ImGui::CollapsingHeader("Transform"), true)
	{
		if (ImGui::BeginTable("ObjectTransform", 2))
		{
			for (int row = 0; row < 3; row++)
			{
				ImGui::TableNextRow();
				for (int column = 0; column < 2; column++)
				{
					ImGui::TableSetColumnIndex(column);
					if (row == 0 && column == 0)	ImGui::Text("Position");
					else if (row == 0 && column == 1)
					{
						ImGui::InputFloat3("##Position", position);
					}

					if (row == 1 && column == 0)	ImGui::Text("Rotation");
					else if (row == 1 && column == 1)
					{
						ImGui::InputFloat3("##Rotation", rotation);
					}

					if (row == 2 && column == 0)	ImGui::Text("Scale");
					else if (row == 2 && column == 1)
					{
						ImGui::InputFloat3("##Scale", scale);
					}
				}
			}
			ImGui::EndTable();
		}
	}

	ImGui::End();
}

void Pg::UI::Manager::ImGuiManager::DemoHierarchy()
{
	ImGui::Begin("DemoHierarchy", NULL, ImGuiWindowFlags_NoCollapse);

	static int objSelectNum = 0;

	// Scene = Tree Nodes
	// Normal GameObject = Selectable
	// Group GameObject = Tree Nodes

	// 오브젝트를 가져와서 그룹으로 만들 수 있어야 한다. -> Drag And Drop

	//for (auto& i : _gameObjectDatas)
	//{
	//	if (ImGui::Selectable(i->_name.c_str(), i->_objectNumber == _objectSelectedNumber))
	//	{
	//		DataSetting(i);
	//		_objectSelectedNumber = i->_objectNumber;
	//	}
	//}
	for (int i = 0; i < 1; i++)
	{
		if (ImGui::Selectable("TestObject", i == objSelectNum))
		{
			objSelectNum= i;
		}
	}
	ImGui::End();
}

void Pg::UI::Manager::ImGuiManager::DemoFilter()
{
	ImGui::Begin("DemoProjectFilter", NULL, ImGuiWindowFlags_NoCollapse);

	//const char* projectPath = std::getenv("PROJECT_PATH");

	//// 재귀함수 만들기
	//if (ImGui::TreeNode("Filter"))
	//{
	//	for (const auto& file : fs::directory_iterator(projectPath))
	//	{
	//		if (file.path().filename().string().rfind(".") == std::string::npos)
	//		{
	//			if (ImGui::TreeNode(file.path().filename().string().c_str()))
	//			{
	//				std::string folderPath = file.path().string();

	//				for (const auto& ffile : fs::directory_iterator(folderPath))
	//				{
	//					if (ffile.path().filename().string().rfind(".") == std::string::npos)
	//					{

	//					}
	//					else
	//					{
	//						ImGui::Selectable(ffile.path().filename().string().c_str(), false);
	//					}
	//				}
	//				ImGui::TreePop();
	//			}
	//		}
	//		else
	//		{
	//			ImGui::Selectable(file.path().filename().string().c_str(), false);
	//		}
	//	}
	//	ImGui::TreePop();
	//}

	ImGui::End();
}

void Pg::UI::Manager::ImGuiManager::DemoViewPort(void* Texture)
{
	ImGui::Begin("DemoScene", NULL, ImGuiWindowFlags_NoCollapse);
	ImGui::Image(Texture, ImVec2(1080.f, 920.f));
	ImGui::End();
}

