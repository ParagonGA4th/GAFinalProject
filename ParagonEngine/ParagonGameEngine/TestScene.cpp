#include "TestScene.h"
#include "../ParagonData/Camera.h"
#include "../ParagonData/ImageRenderer.h"
#include "../ParagonData/Button.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/Transform.h"

//#include "../ParagonData/BaseRenderer.h" //Render 연동 보기 위해.
//#include "../ParagonData/RendererBase2D.h" //Render 연동 보기 위해.
//#include "../ParagonData/RendererBase3D.h" //Render 연동 보기 위해.
#include "../ParagonData/ImageRenderer.h" //Render 연동 보기 위해.
#include "../ParagonData/TextRenderer.h" //Render 연동 보기 위해.
#include "../ParagonData/StaticMeshRenderer.h" //Render 연동 보기 위해.



// Script<->Component 확인하기 위해.
#include "../ParagonData/MoveForwardBack.h"
#include "EditorCameraScript.h"

Pg::Engine::TestScene::TestScene()
{
	tCurrentScene = new Pg::Data::Scene("TestCurrentSceneWorkspace");
}

Pg::Data::GameObject* tObj1;
Pg::Data::GameObject* tObj2;
Pg::Data::GameObject* tObj2_1;
Pg::Data::GameObject* tObj2_2;

void Pg::Engine::TestScene::Initialize()
{
	using namespace Pg::Data;

	
	/// 오수안, 새로 추가한 스크립트 컴포넌트의 테스트를 위한 코드 

	//카메라 하나 더 생성
	tObj1 = tCurrentScene->AddObject("Camera1");
	tObj1->AddComponent<Camera>();
	tObj1->GetComponent<Transform>()->SetPosition({ 0.f, 3.0f, -10.f });
	tObj1->GetComponent<Transform>()->SetLocalRotationEuler(0.f,0.f,0.f);
	tObj1->AddComponent<EditorCameraScript>();

	tObj2 = tCurrentScene->AddObject("Cube1");
	tObj2->GetComponent<Transform>()->SetPosition({ 3.0f, 0.0f, 5.0f });
	tObj2->GetComponent<Transform>()->SetLocalRotationEuler(0.5f, 1.0f, 2.0f);
	tObj2->AddComponent<StaticMeshRenderer>();
	tObj2->GetComponent<StaticMeshRenderer>()->SetMeshFilePath("../Resources/3DModels/MultiMatMesh/LavaWoodCone.fbx");
	tObj2->GetComponent<StaticMeshRenderer>()->SetActive(true);

	GameObject* tObj2_1 = tCurrentScene->AddObject("Cube3");
	tObj2_1->GetComponent<Transform>()->SetPosition({ 5.0f, 0.0f, 0.0f });
	tObj2_1->AddComponent<StaticMeshRenderer>();
	tObj2_1->GetComponent<StaticMeshRenderer>()->SetMeshFilePath("../Resources/3DModels/MultiMatMesh/RoadLavaCone.fbx");
	tObj2_1->GetComponent<StaticMeshRenderer>()->SetActive(true);
	//tObj2_1->AddComponent<MoveForwardBack>();
	//tObj2_1->GetComponent<MoveForwardBack>()->SetActive(true);

	GameObject* tObj2_2 = tCurrentScene->AddObject("Cube4");
	tObj2_2->GetComponent<Transform>()->SetPosition({ -5.0f, 0.0f, 0.0f });
	tObj2_2->AddComponent<StaticMeshRenderer>()->SetMeshFilePath("../Resources/3DModels/MultiMatMesh/WoodRoadCone.fbx");
	tObj2_2->GetComponent<StaticMeshRenderer>()->SetActive(true);

	/// 오수안, 새로 추가한 라이트와 UI 컴포넌트 테스트를 위한 코드 
	
	GameObject* tLight = tCurrentScene->AddObject("LightTest");
	tLight->GetComponent<Transform>()->SetPosition({ 10.f, 10.f, 10.f });

	GameObject* tObj3 = tCurrentScene->AddObject("BtnTest");
	tObj3->GetComponent<Transform>()->SetPosition({ 200.f, 200.f, 0.f });
	tObj3->GetComponent<Transform>()->SetIs3D(false);
	tObj3->AddComponent<Button>();
	tObj3->GetComponent<ImageRenderer>()->SetImagePath("../Resources/Textures/LPDisk.jpg");

	GameObject* tObj4 = tCurrentScene->AddObject("TextTest");
	tObj4->GetComponent<Transform>()->SetPosition({ 600.0f, 200.f, 0.f });
	tObj4->GetComponent<Transform>()->SetIs3D(false);
	tObj4->AddComponent<TextRenderer>();
	tObj4->GetComponent<TextRenderer>()->SetFont("../Resources/Fonts/NotoSansKR_16.spritefont");
	tObj4->GetComponent<TextRenderer>()->SetString("Writing Test");

	//이렇게 하면 메인 카메라 바뀜!!
	tCurrentScene->SetMainCamera(tObj1->GetComponent<Camera>());

	assert(true);
}

void Pg::Engine::TestScene::Update()
{
	/// 오수안, 새로 추가한 스크립트 컴포넌트의 업데이트 테스트를 위한 코드
}

Pg::Data::Scene* Pg::Engine::TestScene::GetCurrentScene()
{
	return tCurrentScene;
}
