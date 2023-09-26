#pragma once
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#include <string>
#include <vector>

class ImGuiManager
{
public:
	ImGuiManager();

	void CreateFrame();

	void ShowDemoInspector();
	void ShowDemoHierarchy();
	void ShowDemoFilter();
	void ShowDemoViewPort();

	void Render();
	void Finalize();
private:
	char nameBuf[255] = "";
	char nameOldBuf[255] = "";
	bool active;

	float position[3];
	float rotation[3];
	float sacle[3];
};
 
