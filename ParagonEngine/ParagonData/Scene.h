#pragma once
#include <string>
#include <vector>
#include "../ParagonGameEngine/Camera.h"


/// <summary>
/// 변지상의 Scene 클래스.
/// 여기서는 기본적인 씬 생성 및 삭제. 그리고 씬을 가지고 있는 리스트가 들어간다.
/// 2023.09.14
/// </summary>

namespace Pg::Engine
{
	class Camera;
}

namespace Pg::Data
{
	class GameObject;

	using namespace Pg::Engine;

	class Scene
	{
	public:
		Scene(std::string sceneName);
		virtual ~Scene();

		void Start();
		void Update();

		GameObject* AddObject(std::string obj);
		void DeleteObject(std::string obj);

		GameObject* CreateGameObject(std::string name);

		std::string GetSceneName();
		void SetSceneName(const std::string& sceneName);

		Camera* GetMainCamera();
		void SetMainCamera(Camera* mainCamera);

		//오브젝트 리스트 자체를 반환.
		std::vector<GameObject*>& GetObjectList();

	private:

		//씬 이름
		std::string _sceneName;

		//씬 안에 오브젝트가 여러개 존재한다.
		std::vector<GameObject*> _objectList;

		//카메라
		Camera* _mainCamera;
	};
}


