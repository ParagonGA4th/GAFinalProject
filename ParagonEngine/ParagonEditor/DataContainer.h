#pragma once
#include <vector>
#include <string>

struct ID3D11Device;
struct ID3D11DeviceContext;

namespace Pg::Data { class Scene; }
namespace Pg::Editor::Data
{
	class DataContainer
	{
	public:
		void SetGraphicsData(ID3D11Device* d, ID3D11DeviceContext* dc);
		ID3D11Device* GetDevice();
		ID3D11DeviceContext* GetDeviceContext();

		void SetSceneTexture(void* Texture);
		void* GetSceneTexture();

		void SetGameTexture(void* Texture);
		void* GetGameTexture();

		void SetEditorOnOff(bool onoff);
		bool GetEditorOnOff();

		void SetSceneList(std::vector<Pg::Data::Scene*> scenes);
		std::vector<Pg::Data::Scene*> GetSceneList();

		void SetCurrentScene(int sceneNumber);
		void SetCurrentScene(std::string sceneName);
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