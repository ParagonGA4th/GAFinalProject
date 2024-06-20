#include "DataContainer.h"

#include "../ParagonData/Scene.h"
#include <algorithm>

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

void Pg::Editor::Data::DataContainer::SetSceneList(std::vector<Pg::Data::Scene*> scenes, bool isSceneLoad)
{
	if (_scenes.empty() && scenes.size() == 1 || !isSceneLoad)
	{
		_scenes = scenes;
		SetCurrentScene(0);
	}
	else
	{
		for (auto& scene : scenes)
		{
			auto it = std::find_if(_scenes.begin(), _scenes.end(),
				[&](Pg::Data::Scene* ts)
				{
					return (ts->GetSceneName() == scene->GetSceneName());
				});

			if (it == _scenes.end())
			{
				_scenes.emplace_back(scene);
			}
		}
	}
}

std::vector<Pg::Data::Scene*> Pg::Editor::Data::DataContainer::GetSceneList()
{
	return _scenes;
}

void Pg::Editor::Data::DataContainer::SetCurrentScene(int sceneNumber)
{
	_isSceneChanged = true;
	_currentScene = _scenes.at(sceneNumber);
}

void Pg::Editor::Data::DataContainer::SetCurrentScene(std::string sceneName)
{
	for (auto& scene : _scenes)
	{
		if (scene->GetSceneName() == sceneName)
		{
			_isSceneChanged = true;
			_currentScene = scene;
		}
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

bool Pg::Editor::Data::DataContainer::GetIsSceneChange()
{
	return _isSceneChanged;
}

void Pg::Editor::Data::DataContainer::SetIsSceneChange(bool isChange)
{
	_isSceneChanged = isChange;
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