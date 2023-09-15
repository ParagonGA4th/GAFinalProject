#pragma once
#include <string>
#include <vector>

/// <summary>
/// 변지상의 Scene 클래스.
/// 여기서는 기본적인 씬 생성 및 삭제. 그리고 씬을 가지고 있는 리스트가 들어간다.
/// 2023.09.14
/// </summary>

namespace Pg::Core
{
	class GameObject;

	class Scene
	{
	public:
		Scene(std::string sceneName);
		virtual ~Scene();

		void Start();
		void Update();

		void AddObject(GameObject* obj);
		void DeleteObject(GameObject* obj);

		GameObject* CreateGameObject(std::string name);

		std::string GetSceneName();
		void SetSceneName(const std::string& sceneName);

	private:

		std::string _sceneName;

		//씬에서는 
		std::vector<GameObject*> _objectList;
	};
}


