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
	void ShowDemoViewPort();

	void Render();
	void Finalize();

private:
	bool _active;

	float position[3];
	float rotation[3];
	float sacle[3];

	bool _objectSelected;			// 선택한 treenode의 정보를 한번만 전달하기 위한 변수 
	int _objectSelectedNumber;		// 현재 Hierarchy창에서 선택된 treenode 

	std::vector<GameObjectData*> _gameObjectDatas;	// Scene 하나
	GameObjectData* _gameObjectData;
};
 
