#pragma once
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

class ImGuiManager
{
public:
	ImGuiManager();

	void CreateFrame();
	void Render();
	void Finalize();
};

