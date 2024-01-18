#pragma once
#include <vector>
#include <unordered_map>
#include <string>

struct ID3D11Device;
struct ID3D11DeviceContext;

using ComponentData = std::unordered_map<std::string, std::vector<std::tuple<std::string, std::string, void*>>>;
using ObjectData = std::unordered_map<std::string, ComponentData>;
using ScenesData = std::unordered_map<std::string, ObjectData>;

namespace Pg::Data { class Scene; }
namespace Pg::Editor::Data
{
	class DataContainer
	{
	public:
		void SetGraphicsData(ID3D11Device* d, ID3D11DeviceContext* dc);
		ID3D11Device* GetDevice() const;
		ID3D11DeviceContext* GetDeviceContext() const;

		void SetSceneTexture(void* Texture);
		void* GetSceneTexture() const;

		void SetGameTexture(void* Texture);
		void* GetGameTexture() const;

		void SetEditorOnOff(bool onoff);
		bool GetEditorOnOff() const;

		void SetSceneList(std::vector<Pg::Data::Scene*> scenes);
		std::vector<Pg::Data::Scene*> GetSceneList();

		void SetScenesData(ScenesData* sceneData);
		ScenesData* GetScenesData() const;

		// sceneNumber == vector<Scene> index
		void SetCurrentScene(int sceneNumber);
		void SetCurrentScene(std::string sceneName);
		void SetCurrentScene(Pg::Data::Scene* scene);
		Pg::Data::Scene* GetCurrentScene();	

	private:
		ID3D11Device* _device;
		ID3D11DeviceContext* _deviceContext;

		std::vector<Pg::Data::Scene*> _scenes;
		Pg::Data::Scene* _currentScene;

		ScenesData* _scenesData;

		void* _sceneTexture;
		void* _gameTexture;

		bool _onOff;
	};
}