#include "EditorHelper.h"

void Pg::Editor::Helper::EditorHelper::SetDevice(ID3D11Device* device)
{
	_device = device;
}

ID3D11Device* Pg::Editor::Helper::EditorHelper::GetDevice()
{
	return _device;
}

void Pg::Editor::Helper::EditorHelper::SetDeviceContext(ID3D11DeviceContext* deviceContext)
{
	_deviceContext = deviceContext;
}

ID3D11DeviceContext* Pg::Editor::Helper::EditorHelper::GetDeviceContext()
{
	return _deviceContext;
}

void Pg::Editor::Helper::EditorHelper::SetSceneTexture(void* Texture)
{
	_sceneTexture = Texture;
}

void* Pg::Editor::Helper::EditorHelper::GetSceneTexture()
{
	return _sceneTexture;
}

void Pg::Editor::Helper::EditorHelper::SetGameTexture(void* Texture)
{
	_gameTexture = Texture;
}

void* Pg::Editor::Helper::EditorHelper::GetGameTexture()
{
	return _gameTexture;
}

void Pg::Editor::Helper::EditorHelper::SetEditorOnOff(bool onoff)
{
	_onOff = onoff;
}

bool Pg::Editor::Helper::EditorHelper::GetEditorOnOff()
{
	return _onOff;
}

void Pg::Editor::Helper::EditorHelper::SetCurrentScene(Pg::Data::Scene* scene)
{
	_currentScene = scene;
}

Pg::Data::Scene* Pg::Editor::Helper::EditorHelper::GetCurrentScene()
{
	return _currentScene;
}
