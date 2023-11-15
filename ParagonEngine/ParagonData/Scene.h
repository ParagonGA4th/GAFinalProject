#pragma once
#include <string>
#include <vector>
#include "DirectionalLight.h"

/// <summary>
/// 변지상의 Scene 클래스.
/// 여기서는 기본적인 씬 생성 및 삭제. 그리고 씬을 가지고 있는 리스트가 들어간다.
/// 2023.09.14
/// 
/// 
/// 23.10.11 오수안
/// 씬이 기본적으로 가지고 있는 main Directional Light 메서드를 추가
/// </summary>

namespace Pg::Data
{
	class GameObject;
	class Camera;

	using Pg::Data::GameObject;
	using Pg::Data::Camera;
	using Pg::Data::DirectionalLight;

	class Scene
	{
	public:
		Scene(std::string sceneName);
		virtual ~Scene();

		void Start();
		void Update();
		void FixedUpdate();
		void LateUpdate();

		GameObject* AddObject(std::string obj);
		void DeleteObject(std::string obj);

		std::string GetSceneName();
		void SetSceneName(const std::string& sceneName);

		Camera* GetMainCamera();
		void SetMainCamera(Camera* mainCamera);

		DirectionalLight* GetMainLight();
		void SetMainLight(DirectionalLight* mainLight);

		//오브젝트 리스트 자체를 반환.
		const std::vector<GameObject*>& GetObjectList() const;

	private:
		//씬 이름
		std::string _sceneName;

		//씬 안에 오브젝트가 여러개 존재한다.
		std::vector<GameObject*> _objectList;

		//카메라
		Camera* _mainCamera;
		DirectionalLight* _mainDirLight;
	};
}


