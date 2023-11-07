#pragma once

struct ID3D11Device;
struct ID3D11DeviceContext;

namespace Pg::Editor::Helper
{
	class EditorHelper
	{
	public:
		void SetDevice(ID3D11Device* device);
		ID3D11Device* GetDevice();		
		
		void SetDeviceContext(ID3D11DeviceContext* deviceContext);
		ID3D11DeviceContext* GetDeviceContext();

		void SetSceneTexture(void* Texture);
		void* GetSceneTexture();

		void SetGameTexture(void* Texture);
		void* GetGameTexture();

		void SetEditorOnOff(bool onoff);
		bool GetEditorOnOff();

	private:
		ID3D11Device* _device;
		ID3D11DeviceContext* _deviceContext;

		void* _sceneTexture;
		void* _gameTexture;

		bool _onOff;
	};
}
