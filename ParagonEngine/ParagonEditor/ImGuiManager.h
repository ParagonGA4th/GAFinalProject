#pragma once
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#include <vector>

struct GameObjectData;

class ImGuiManager
{
public:
	ImGuiManager(std::vector<GameObjectData*> data);

	void CreateFrame();

	void ShowDemoInspector();
	void ShowDemoHierarchy();
	void ShowDemoFilter();
	void ShowDemoViewPort(void* hwnd);

	void Render();
	void Finalize();

private:
	void DataSetting(GameObjectData* data);

private:
	bool _active;

	char _name[256];
	char _tag[256];

	float position[3];
	float rotation[3];
	float scale[3];

	int _objectSelectedNumber;		// 현재 Hierarchy창에서 선택된 treenode 

	std::vector<GameObjectData*> _gameObjectDatas;	// Scene 하나
};
 
