#pragma once
#include <string>
#include <vector>
#include "DirectionalLight.h"
#include "../ParagonData/GraphicsDebugData.h"

/// <summary>
/// 변지상의 Scene 클래스.
/// 여기서는 기본적인 씬 생성 및 삭제. 그리고 씬을 가지고 있는 리스트가 들어간다.
/// 2023.09.14
/// 
/// 
/// 23.10.11 오수안
/// 씬이 기본적으로 가지고 있는 main Directional Light 메서드를 추가
/// </summary>

namespace Pg::Engine
{
	class SceneSystem;
}
namespace Pg::Data
{
	class GameObject;
	class Camera;

	using Pg::Data::GameObject;
	using Pg::Data::Camera;
	using Pg::Data::DirectionalLight;

	class Scene
	{
		friend class Pg::Engine::SceneSystem;

	public:
		Scene(std::string sceneName);
		virtual ~Scene();

		//Internal_NNN : 게임 로직이랑 분리된 엔진 내부 구성 유지하기 위해. (GameLoop w/ Editor Only)
		//에디터를 여전히 돌리고 있음에도 PlayMode가 아닐 경우,
		//요구되는 정보가 안 들어갈 수도 있다. 이때 활용하기 위해. 게임 개발 로직이랑 분리되어야 한다.
		void Internal_EngineAwake();
		void Awake();
		void Start();

		void Internal_EngineUpdate(); //Internal의 Update 버전. Ex. Camera 등에 의해 활용.
		void Update();
		void FixedUpdate();
		void LateUpdate();

	
		//Editor / TestScene이 발동되기 위해 필요한 (오브젝트 "에디터 시간" 생성) 함수들.
		GameObject* AddObject(const std::string& obj);
		void DeleteObject(const std::string& obj);

		//런타임에 오브젝트를 추가 / 삭제하기 위해서 쓰이는 함수들. (오브젝트 "런 타임" 생성) 함수들.
		void AddObjectRuntime(const std::string& obj);
		void DeleteObjectRuntime(const std::string& obj);

		std::string GetSceneName();
		void SetSceneName(const std::string& sceneName);

		Camera* GetMainCamera();
		void SetMainCamera(Camera* mainCamera);

		DirectionalLight* GetMainLight();
		void SetMainLight(DirectionalLight* mainLight);

		//오브젝트 리스트 자체를 반환.
		const std::vector<GameObject*>& GetObjectList() const;

		//오브젝트의 이름을 가지고 단일 오브젝트를 찾는다.
		Pg::Data::GameObject* FindObjectWithName(const std::string& name);

		//태그를 갖고 오브젝트들을 찾아서 반환한다.
		std::vector<Pg::Data::GameObject*> FindObjectsWithTag(const std::string& tag);

	private:
		//씬 이름
		std::string _sceneName;

		//씬 안에 오브젝트가 여러개 존재한다.
		std::vector<GameObject*> _objectList;

		//카메라
		Camera* _mainCamera;
		DirectionalLight* _mainDirLight;

	public:
		GraphicsDebugData _graphicsDebugData;

	private:
		//SceneSystem에서 Add/Object 꼬이지 않게 하기 위해서.
		std::vector<std::string> _addObjectPlanList;
		std::vector<std::string> _deleteObjectPlanList;

		void HandleAddDeleteInScene();
	};
}


