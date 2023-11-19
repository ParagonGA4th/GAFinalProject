#include "DataContainer.h"

void Pg::Editor::Data::DataContainer::SetDevice(ID3D11Device* device)
{
	_device = device;
}

ID3D11Device* Pg::Editor::Data::DataContainer::GetDevice()
{
	return _device;
}

void Pg::Editor::Data::DataContainer::SetDeviceContext(ID3D11DeviceContext* deviceContext)
{
	_deviceContext = deviceContext;
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

void Pg::Editor::Data::DataContainer::SetScenes(std::vector<Pg::Data::Scene*> scenes)
{
	_scenes = scenes;

	// юс╫ц
	SetCurrentScene(_scenes.at(0));
}

std::vector<Pg::Data::Scene*> Pg::Editor::Data::DataContainer::GetScenes()
{
	return _scenes;
}

void Pg::Editor::Data::DataContainer::SetCurrentScene(Pg::Data::Scene* scene)
{
	_currentScene = scene;
}

Pg::Data::Scene* Pg::Editor::Data::DataContainer::GetCurrentScene()
{
	return _currentScene;
}

