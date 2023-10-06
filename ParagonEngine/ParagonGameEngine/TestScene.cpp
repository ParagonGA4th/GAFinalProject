#include "TestScene.h"
#include "Camera.h"
#include "../ParagonCore/GameObject.h"
#include "../ParagonCore/Transform.h"
#include "../ParagonCore/BaseRenderer.h" //Render 연동 보기 위해.

Pg::Engine::TestScene::TestScene()
{

}

void Pg::Engine::TestScene::Initialize()
{
	using namespace Pg::Core;

	tCurrentScene = new Scene("TestCurrentScene");

	//이거 호출하면 MainCamera 반환함!!
	tCurrentScene->GetMainCamera();

	//카메라 하나 더 생성
	GameObject* tObj1 = new GameObject("Camera1");
	tObj1->AddComponent<Camera>();
	tObj1->GetComponent<Transform>()->SetPosition({ 10.0f, 0.0f, 5.0f });

	GameObject* tObj2 = new GameObject("Cube2");
	tObj2->GetComponent<Transform>()->SetPosition({ -10.0f, 0.0f, 5.0f });
	tObj2->AddComponent<BaseRenderer>();

	tCurrentScene->AddObject("Camera1");
	tCurrentScene->AddObject("Cube2");

	//이렇게 하면 메인 카메라 바뀜!!
	tCurrentScene->SetMainCamera(tObj1->GetComponent<Camera>());
}

Pg::Core::Scene* Pg::Engine::TestScene::GetCurrentScene()
{
	return tCurrentScene;
}
