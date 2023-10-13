#include "TestScene.h"
#include "../ParagonData/Camera.h"
#include "../ParagonData/ImageRenderer.h"
#include "../ParagonData/Button.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/Transform.h"
#include "../ParagonData/BaseRenderer.h" //Render 연동 보기 위해.
#include "../ParagonData/RendererBase2D.h" //Render 연동 보기 위해.
#include "../ParagonData/RendererBase3D.h" //Render 연동 보기 위해.

// Script<->Component 확인하기 위해.
#include "../ParagonData/MoveForwardBack.h"

Pg::Engine::TestScene::TestScene()
{

}

Pg::Data::GameObject* tObj1;
Pg::Data::GameObject* tObj2;
Pg::Data::GameObject* tObj2_1;
Pg::Data::GameObject* tObj2_2;

void Pg::Engine::TestScene::Initialize()
{
	using namespace Pg::Data;

	tCurrentScene = new Scene("TestCurrentSceneWorkspace");

	//이거 호출하면 MainCamera 반환함!!
	tCurrentScene->GetMainCamera();

	//카메라 하나 더 생성
	tObj1 = tCurrentScene->AddObject("Camera1");
	tObj1->AddComponent<Camera>();
	tObj1->GetComponent<Transform>()->SetPosition({ 10.0f, 0.0f, 5.0f });

	tObj2 = tCurrentScene->AddObject("Cube1");
	tObj2->GetComponent<Transform>()->SetPosition({ 3.0f, 0.0f, 5.0f });
	tObj2->GetComponent<Transform>()->SetLocalRotationEuler(0.5f, 1.0f, 2.0f);
	tObj2->AddComponent<RendererBase3D>();
	tObj2->GetComponent<RendererBase3D>()->SetActive(true);

	tObj2_1 = tCurrentScene->AddObject("Cube2");
	tObj2_1->GetComponent<Transform>()->SetPosition({ 0.0f, 0.0f, 0.0f });
	tObj2_1->AddComponent<RendererBase3D>();
	tObj2_1->GetComponent<RendererBase3D>()->SetActive(true);
	tObj2_1->AddComponent<MoveForwardBack>();
	tObj2_1->GetComponent<MoveForwardBack>()->SetActive(true);

	tObj2_2 = tCurrentScene->AddObject("Cube3");
	tObj2_2->GetComponent<Transform>()->SetPosition({ -3.0f, 3.0f, -3.0f });
	tObj2_2->GetComponent<Transform>()->SetLocalRotationEuler(1.0f, 2.0f, 0.5f);
	tObj2_2->AddComponent<RendererBase3D>();
	tObj2_2->GetComponent<RendererBase3D>()->SetActive(true);

	/// 오수안, 새로 추가한 라이트와 UI 컴포넌트 테스트를 위한 코드 
	GameObject* tObj3 = tCurrentScene->AddObject("BtnTest");
	tObj3->GetComponent<Transform>()->SetPosition({0.f, 0.f, 0.f});
	tObj3->AddComponent<Button>();
	tObj3->GetComponent<ImageRenderer>()->SetImagePath("Test.png");


	GameObject* tLight = tCurrentScene->AddObject("LightTest");
	tLight->GetComponent<Transform>()->SetPosition({ 10.f, 10.f, 10.f });


	////tObj2->AddComponent<BaseRenderer>();
	//tObj2->AddComponent<RendererBase2D>();

	//이렇게 하면 메인 카메라 바뀜!!
	tCurrentScene->SetMainCamera(tObj1->GetComponent<Camera>());

	assert(true);
}

Pg::Data::Scene* Pg::Engine::TestScene::GetCurrentScene()
{
	return tCurrentScene;
}
