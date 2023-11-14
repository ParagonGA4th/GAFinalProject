#pragma once
#include <vector>

struct ID3D11Device;
struct ID3D11DeviceContext;

namespace Pg::Data { class Scene; }

namespace Pg::Editor::Data
{
	class DataContainer
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

		void SetScenes(std::vector<Pg::Data::Scene*> scenes);
		std::vector<Pg::Data::Scene*> GetScenes();

		void SetCurrentScene(Pg::Data::Scene* scene);
		Pg::Data::Scene* GetCurrentScene();				

	private:
		ID3D11Device* _device;
		ID3D11DeviceContext* _deviceContext;

		std::vector<Pg::Data::Scene*> _scenes;
		Pg::Data::Scene* _currentScene;

		void* _sceneTexture;
		void* _gameTexture;

		bool _onOff;
	};
}