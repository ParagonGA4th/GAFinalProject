#include "DataContainer.h"

#include "../ParagonData/Scene.h"

void Pg::Editor::Data::DataContainer::SetGraphicsData(ID3D11Device* d, ID3D11DeviceContext* dc)
{
	_device = d;
	_deviceContext = dc;
}

void Pg::Editor::Data::DataContainer::SetProjectPath(std::string path)
{
	_projectPath = path;
}

std::string Pg::Editor::Data::DataContainer::GetProjectPath()
{
	return _projectPath;
}

ID3D11Device* Pg::Editor::Data::DataContainer::GetDevice() const
{
	return _device;
}

ID3D11DeviceContext* Pg::Editor::Data::DataContainer::GetDeviceContext() const
{
	return _deviceContext;
}

void Pg::Editor::Data::DataContainer::SetSceneTexture(void* Texture)
{
	_sceneTexture = Texture;
}

void* Pg::Editor::Data::DataContainer::GetSceneTexture() const
{
	return _sceneTexture;
}

void Pg::Editor::Data::DataContainer::SetEditorOnOff(bool onoff)
{
	_onOff = onoff;
}

bool Pg::Editor::Data::DataContainer::GetEditorOnOff() const
{
	return _onOff;
}

void Pg::Editor::Data::DataContainer::SetSceneList(std::vector<Pg::Data::Scene*> scenes)
{
	_scenes = scenes;

	// юс╫ц
	SetCurrentScene(0);
}

std::vector<Pg::Data::Scene*> Pg::Editor::Data::DataContainer::GetSceneList()
{
	return _scenes;
}

void Pg::Editor::Data::DataContainer::SetCurrentScene(int sceneNumber)
{
	_currentScene = _scenes.at(sceneNumber);
}

void Pg::Editor::Data::DataContainer::SetCurrentScene(std::string sceneName)
{
	for (auto& scene : _scenes)
	{
		if (scene->GetSceneName() == sceneName) _currentScene = scene;
	}
}

void Pg::Editor::Data::DataContainer::SetCurrentScene(Pg::Data::Scene* scene)
{
	_currentScene = scene;
}

Pg::Data::Scene* Pg::Editor::Data::DataContainer::GetCurrentScene()
{
	return _currentScene;
}

void Pg::Editor::Data::DataContainer::SetPickObject(Pg::Data::GameObject* object)
{
	_pickObject = object;
}

Pg::Data::GameObject* Pg::Editor::Data::DataContainer::GetPickObject()
{
	return _pickObject;
}

void Pg::Editor::Data::DataContainer::SetAssetList(std::vector<std::string> list)
{
	_assetList = list;
}

std::vector<std::string> Pg::Editor::Data::DataContainer::GetAssetList()
{
	return _assetList;
}

int Pg::Editor::Data::DataContainer::GetAssetIndex(std::string assetName)
{
	int count = 0;

	for (auto asset : _assetList)
	{
		if (asset != assetName)
		{
			count++; 
			continue;
		}
		else
		{
			return count;
		}
	}
	if (count == _assetList.size())	return 0;
}

void Pg::Editor::Data::DataContainer::SetUUID(std::string sceneName, std::unordered_map<std::string, std::string> uuid)
{
	_uuid.insert({ sceneName , uuid});
}

std::string Pg::Editor::Data::DataContainer::GetUUID(std::string sceneName, std::string objName)
{
	for (auto& obj : _uuid.at(sceneName))
	{
		if (obj.first == objName) return obj.second;
	}
}

std::string Pg::Editor::Data::DataContainer::GetObjNameWithUUID(std::string sceneName, std::string uuid)
{
	for (auto& obj : _uuid.at(sceneName))
	{
		if (obj.second == uuid) return obj.first;
	}
}

void Pg::Editor::Data::DataContainer::SetMonitorWidth(float width)
{
	_monitorWidth = width;
}

void Pg::Editor::Data::DataContainer::SetMonitorHeight(float height)
{
	_monitorHeight = height;
}

float Pg::Editor::Data::DataContainer::GetMonitorWidth()
{
	return _monitorWidth;
}

float Pg::Editor::Data::DataContainer::GetMonitorHeight()
{
	return _monitorHeight;
}
