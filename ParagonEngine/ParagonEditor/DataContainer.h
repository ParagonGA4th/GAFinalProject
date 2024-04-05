#pragma once
#include <vector>
#include <unordered_map>
#include <string>

struct ID3D11Device;
struct ID3D11DeviceContext;

namespace Pg::Data { class Scene; class GameObject; }
namespace Pg::Editor::Data
{
	class DataContainer
	{
	public:
		void SetGraphicsData(ID3D11Device* d, ID3D11DeviceContext* dc);
		ID3D11Device* GetDevice() const;
		ID3D11DeviceContext* GetDeviceContext() const;

		void SetProjectPath(std::string path);
		std::string GetProjectPath();

		void SetSceneTexture(void* Texture);
		void* GetSceneTexture() const;

		void SetEditorOnOff(bool onoff);
		bool GetEditorOnOff() const;

		void SetSceneList(std::vector<Pg::Data::Scene*> scenes);
		std::vector<Pg::Data::Scene*> GetSceneList();

		// sceneNumber == vector<Scene> index
		void SetCurrentScene(int sceneNumber);
		void SetCurrentScene(std::string sceneName);
		void SetCurrentScene(Pg::Data::Scene* scene);
		Pg::Data::Scene* GetCurrentScene();	

		void SetPickObject(Pg::Data::GameObject* object);
		Pg::Data::GameObject* GetPickObject();

		void SetAssetList(std::vector<std::string> list);
		std::vector<std::string> GetAssetList();
		int GetAssetIndex(std::string assetName);

	private:
		ID3D11Device* _device;
		ID3D11DeviceContext* _deviceContext;

		std::vector<Pg::Data::Scene*> _scenes;
		Pg::Data::Scene* _currentScene;
		Pg::Data::GameObject* _pickObject;

		std::string _projectPath;
		void* _sceneTexture;
		bool _onOff;

		std::vector<std::string> _assetList;
	};
}