#include "DataContainer.h"

#include "../ParagonData/Scene.h"

void Pg::Editor::Data::DataContainer::SetGraphicsData(ID3D11Device* d, ID3D11DeviceContext* dc)
{
	_device = d;
	_deviceContext = dc;
}

ID3D11Device* Pg::Editor::Data::DataContainer::GetDevice()
{
	return _device;
}

ID3D11DeviceContext* Pg::Editor::Data::DataContainer::GetDeviceContext()
{
	return _deviceContext;
}

void Pg::Editor::Data::DataContainer::SetSceneTexture(void* Texture)
{
	_sceneTexture = Texture;
}

void* Pg::Editor::Data::DataContainer::GetSceneTexture()
{
	return _sceneTexture;
}

void Pg::Editor::Data::DataContainer::SetGameTexture(void* Texture)
{
	_gameTexture = Texture;
}

void* Pg::Editor::Data::DataContainer::GetGameTexture()
{
	return _gameTexture;
}

void Pg::Editor::Data::DataContainer::SetEditorOnOff(bool onoff)
{
	_onOff = onoff;
}

bool Pg::Editor::Data::DataContainer::GetEditorOnOff()
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



