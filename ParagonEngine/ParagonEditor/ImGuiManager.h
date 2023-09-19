#pragma once
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

class ImGuiManager
{
public:
	ImGuiManager();

	void CreateFrame();

	void ShowDemoInspector();
	void ShowDemoHierarchy();
	void ShowDemoFilter();

	void Render();
	void Finalize();
};

