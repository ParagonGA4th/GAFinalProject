#include "TestScene.h"
#include "../ParagonData/Camera.h"
#include "../ParagonData/ImageRenderer.h"
#include "../ParagonData/Button.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/Transform.h"
#include "../ParagonData/BoxCollider.h"
#include "../ParagonData/SphereCollider.h"
#include "../ParagonData/CapsuleCollider.h"

//#include "../ParagonData/BaseRenderer.h" //Render 연동 보기 위해.
//#include "../ParagonData/RendererBase2D.h" //Render 연동 보기 위해.
//#include "../ParagonData/RendererBase3D.h" //Render 연동 보기 위해.
#include "../ParagonData/ImageRenderer.h" //Render 연동 보기 위해.
#include "../ParagonData/TextRenderer.h" //Render 연동 보기 위해.
#include "../ParagonData/StaticMeshRenderer.h" //Render 연동 보기 위해.



// Script<->Component 확인하기 위해.
#include "../ParagonData/MoveForwardBack.h"
#include "PrintRuntimeData.h"
#include "EditorCameraScript.h"
#include "MovingTest.h"

#include "../ParagonData/SpotLight.h"
#include "../ParagonData/PointLight.h"

Pg::Engine::TestScene::TestScene()
{
	tCurrentScene = new Pg::Data::Scene("TestCurrentSceneWorkspace");
}

void Pg::Engine::TestScene::Initialize()
{
	using namespace Pg::Data;

	
	///새로 추가한 스크립트 컴포넌트의 테스트를 위한 코드 

	//카메라 하나 더 생성
	Pg::Data::GameObject* tObj1 = tCurrentScene->AddObject("Camera1");
	tObj1->AddComponent<Camera>();
	tObj1->GetComponent<Transform>()->SetPosition({ 0.f, 3.0f, -10.f });
	tObj1->GetComponent<Transform>()->SetLocalRotationEuler(0.f,0.f,0.f);
	tObj1->AddComponent<EditorCameraScript>();

	Pg::Data::GameObject* tObj2 = tCurrentScene->AddObject("Cube1");
	tObj2->GetComponent<Transform>()->SetPosition({ 0.0f, 10.0f, 0.0f });
	//tObj2->GetComponent<Transform>()->SetLocalRotationEuler(0.5f, 1.0f, 2.0f);
	tObj2->AddComponent<BoxCollider>();
	tObj2->AddComponent<StaticMeshRenderer>();
	tObj2->GetComponent<StaticMeshRenderer>()->SetMeshFilePath("../Resources/3DModels/BasicMesh/Cube/Cube.fbx");
	tObj2->GetComponent<StaticMeshRenderer>()->SetActive(true);
	//tObj2->GetComponent<MoveForwardBack>()->SetActive(true);

	GameObject* tObj2_1 = tCurrentScene->AddObject("Sphere1");
	tObj2_1->GetComponent<Transform>()->SetPosition({ 3.0f, 10.0f, 0.0f });
	tObj2_1->AddComponent<SphereCollider>();
	//tObj2_1->GetComponent<SphereCollider>()->SetRadius(2.0f);
	tObj2_1->AddComponent<StaticMeshRenderer>();
	tObj2_1->GetComponent<StaticMeshRenderer>()->SetMeshFilePath("../Resources/3DModels/BasicMesh/Sphere/Sphere.fbx");
	tObj2_1->GetComponent<StaticMeshRenderer>()->SetActive(true);


	GameObject* tObj2_2 = tCurrentScene->AddObject("Capsule1");
	tObj2_2->GetComponent<Transform>()->SetPosition({ 1.0f, 0.0f, 0.0f });
	tObj2_2->AddComponent<CapsuleCollider>();
	tObj2_2->GetComponent<CapsuleCollider>()->FreezeAxisX(true);
	tObj2_2->GetComponent<CapsuleCollider>()->FreezeAxisY(true);
	tObj2_2->GetComponent<CapsuleCollider>()->FreezeAxisZ(true);
	//tObj2_2->GetComponent<CapsuleCollider>()->SetPoisitonOffset({ 1.0f, 0.0f, 0.0f });
	//tObj2_2->GetComponent<CapsuleCollider>()->SetRotationOffset ({ 0.0f, 90.0f, 0.0f, 0.0f });
	tObj2_2->AddComponent<StaticMeshRenderer>();
	tObj2_2->GetComponent<StaticMeshRenderer>()->SetMeshFilePath("../Resources/3DModels/BasicMesh/Capsule/Capsule.fbx");
	tObj2_2->GetComponent<StaticMeshRenderer>()->SetActive(true);
	tObj2_2->AddComponent<MovingTest>();
	tObj2_2->AddComponent<MovingTest>()->SetActive(true);
	//tObj2_2->AddComponent<MoveForwardBack>();
	//tObj2_2->GetComponent<MoveForwardBack>()->SetActive(false);


	Pg::Data::GameObject* tObj3 = tCurrentScene->AddObject("Capsule2");
	tObj3->GetComponent<Transform>()->SetPosition({ -1.0f, -3.0f, 5.0f });
	//tObj3->GetComponent<Transform>()->SetLocalRotationEuler(0.0f, 100.0f, 0.0f);
	tObj3->AddComponent<CapsuleCollider>();
	tObj3->GetComponent<CapsuleCollider>()->SetCapsuleInfo(2.0f, 2.0f);
	tObj3->AddComponent<StaticMeshRenderer>();
	tObj3->GetComponent<StaticMeshRenderer>()->SetActive(true);
	tObj3->GetComponent<StaticMeshRenderer>()->SetMeshFilePath("../Resources/3DModels/BasicMesh/Capsule/Capsule.fbx");
	//tObj3->AddComponent<MoveForwardBack>();
	//tObj3->GetComponent<MoveForwardBack>()->SetActive(false);

	Pg::Data::GameObject* tObj4 = tCurrentScene->AddObject("Cube6");
	tObj4->GetComponent<Transform>()->SetPosition({ -3.0f, -1.0f, -2.0f });
	tObj4->GetComponent<Transform>()->SetLocalRotationEuler(0.5f, 1.5f, -2.0f);
	//tObj4->AddComponent<BoxCollider>();
	tObj4->AddComponent<StaticMeshRenderer>();
	tObj4->GetComponent<StaticMeshRenderer>()->SetMeshFilePath("../Resources/3DModels/StaticMesh/SimpleCube/simplecube.fbx");
	tObj4->GetComponent<StaticMeshRenderer>()->SetActive(true);
	tObj4->AddComponent<MoveForwardBack>();
	tObj4->GetComponent<MoveForwardBack>()->SetActive(true);

	Pg::Data::GameObject* tObj5 = tCurrentScene->AddObject("Cube7");
	tObj5->GetComponent<Transform>()->SetPosition({ 5.0f, 0.0f, 0.0f });
	tObj5->GetComponent<Transform>()->SetLocalRotationEuler(1.5f, 2.0f, 4.0f);
	//tObj5->AddComponent<BoxCollider>();
	//tObj5->GetComponent<BoxCollider>()->SetPoisitonOffset({ 1.0f,0.0f,0.0f });
	tObj5->AddComponent<StaticMeshRenderer>();
	tObj5->GetComponent<StaticMeshRenderer>()->SetMeshFilePath("../Resources/3DModels/StaticMesh/twcylinder/twcylinder.fbx");
	tObj5->GetComponent<StaticMeshRenderer>()->SetActive(true);
	//tObj5->AddComponent<MoveForwardBack>();
	//tObj5->GetComponent<MoveForwardBack>()->SetActive(true);

	Pg::Data::GameObject* tObj6 = tCurrentScene->AddObject("Cube8");
	tObj6->GetComponent<Transform>()->SetPosition({ -5.0f, 0.0f, 0.0f });
	tObj6->GetComponent<Transform>()->SetLocalScale( 2.0f, 2.0f, 2.0f );
	tObj6->GetComponent<Transform>()->SetLocalRotationEuler(0.0f, -1.57f, 0.0f);
	//tObj6->AddComponent<BoxCollider>();
	tObj6->AddComponent<StaticMeshRenderer>();
	tObj6->GetComponent<StaticMeshRenderer>()->SetMeshFilePath("../Resources/3DModels/StaticMesh/SimpleCube/simplecube.fbx");
	tObj6->GetComponent<StaticMeshRenderer>()->SetActive(true);

	Pg::Data::GameObject* tObj7 = tCurrentScene->AddObject("Plane");
	tObj7->GetComponent<Transform>()->SetPosition({ 0.0f, 0.2f, 0.0f });
	tObj7->GetComponent<Transform>()->SetLocalScale(1.0f, 1.0f, 1.0f);
	tObj7->GetComponent<Transform>()->SetLocalRotationEuler(0.0f, 0.0f, 0.0f);
	tObj7->AddComponent<StaticMeshRenderer>();
	tObj7->GetComponent<StaticMeshRenderer>()->SetMeshFilePath("../Resources/3DModels/BasicMesh/Plane/plane.fbx");
	tObj7->GetComponent<StaticMeshRenderer>()->SetActive(true);

	///새로 추가한 라이트와 UI 컴포넌트 테스트를 위한 코드 
	
	// Directional
	GameObject* tLight = tCurrentScene->AddObject("LightTest1");
	tLight->GetComponent<Transform>()->SetPosition({ 0.f, 0.f, 0.f });
	tLight->AddComponent<DirectionalLight>();
	tLight->GetComponent<DirectionalLight>()->SetDirection({ -1.0f, -1.0f, 1.0f });
	tLight->GetComponent<DirectionalLight>()->SetAmbient({ 0.0f, 0.0f, 0.0f, 1.0f });
	tLight->GetComponent<DirectionalLight>()->SetDiffuse({ 0.6f, 0.6f, 0.6f, 1.0f });
	tLight->GetComponent<DirectionalLight>()->SetSpecular({ 0.2f, 0.2f, 0.2f, 1.0f });
	tLight->GetComponent<DirectionalLight>()->SetIntensity( 0.2f );
	tLight->GetComponent<DirectionalLight>()->SetActive(true);

	// Point
	GameObject* tLight2 = tCurrentScene->AddObject("LightTest2");
	tLight2->GetComponent<Transform>()->SetPosition({ 0.f, 2.0f, 0.f });
	tLight2->AddComponent<PointLight>();
	tLight2->GetComponent<PointLight>()->SetAmbient({ 0.0f, 0.0f, 0.0f, 1.0f });
	tLight2->GetComponent<PointLight>()->SetDiffuse({ 0.6f, 0.6f, 0.6f, 1.0f });
	tLight2->GetComponent<PointLight>()->SetSpecular({ 0.7f, 0.7f, 0.7f, 1.0f });
	tLight2->GetComponent<PointLight>()->SetAttenuation({ 1.0f, 1.0f, 0.0f });
	tLight2->GetComponent<PointLight>()->SetIntensity(10.0f);
	tLight2->GetComponent<PointLight>()->SetActive(true);
	 
	//GameObject* tLight2_1 = tCurrentScene->AddObject("LightTest2_1");
	//tLight2_1->GetComponent<Transform>()->SetPosition({ -3.f, 0.f, 0.f });
	//tLight2_1->AddComponent<PointLight>();
	//tLight2_1->GetComponent<PointLight>()->SetAmbient({ 0.01f, 0.01f, 0.01f, 1.0f });
	//tLight2_1->GetComponent<PointLight>()->SetDiffuse({ 0.1f, 0.1f, 0.6f, 1.0f });
	//tLight2_1->GetComponent<PointLight>()->SetSpecular({ 0.1f, 0.1f, 0.5f, 1.0f });
	//tLight2_1->GetComponent<PointLight>()->SetAttenuation({ 1.0f, 1.0f, 1.0f });
	//tLight2_1->GetComponent<PointLight>()->SetIntensity(0.7f);
	//tLight2_1->GetComponent<PointLight>()->SetActive(true);

	//// Spot
	//GameObject* tLight3 = tCurrentScene->AddObject("LightTest3");
	//tLight3->GetComponent<Transform>()->SetPosition({ 5.0f, 5.0f, 5.0f });
	//tLight3->AddComponent<SpotLight>();
	//tLight3->GetComponent<SpotLight>()->SetAmbient({ 0.01f, 0.01f, 0.01f, 1.0f });
	//tLight3->GetComponent<SpotLight>()->SetDiffuse({ 0.1f, 0.6f , 0.1f, 1.0f });
	//tLight3->GetComponent<SpotLight>()->SetSpecular({ 0.1f, 0.6f, 0.1f, 1.0f });
	//tLight3->GetComponent<SpotLight>()->SetAttenuation({ 0.0f, 1.0f, 1.0f });
	//tLight3->GetComponent<SpotLight>()->SetIntensity(3.0f);
	//tLight3->GetComponent<SpotLight>()->SetDirection({ -2.0f, -2.0f, -2.0f });
	//tLight3->GetComponent<SpotLight>()->SetActive(true);
	//
	GameObject* tLight4 = tCurrentScene->AddObject("LightTest4");
	tLight4->GetComponent<Transform>()->SetPosition({ 0.0f, 5.0f, 0.0f });
	tLight4->AddComponent<SpotLight>();
	tLight4->GetComponent<SpotLight>()->SetAmbient({ 0.01f, 0.01f, 0.01f, 1.0f });
	tLight4->GetComponent<SpotLight>()->SetDiffuse({ 0.8f, 0.8f, 0.8f, 1.0f });
	tLight4->GetComponent<SpotLight>()->SetSpecular({ 0.7f, 0.7f, 0.7f, 1.0f });
	tLight4->GetComponent<SpotLight>()->SetAttenuation({ 1.0f, 1.0f, 0.0f });
	tLight4->GetComponent<SpotLight>()->SetIntensity(10.0f);
	tLight4->GetComponent<SpotLight>()->SetDirection({ 0.0f, -1.0f, 0.0f });
	tLight4->GetComponent<SpotLight>()->SetActive(true);

	GameObject* tObj8 = tCurrentScene->AddObject("BtnTest");
	tObj8->GetComponent<Transform>()->SetPosition({ 10.f, 140.f, 0.f });
	tObj8->GetComponent<Transform>()->SetIs3D(false);
	tObj8->AddComponent<Button>();
	tObj8->GetComponent<ImageRenderer>()->SetImagePath("../Resources/Textures/wook.jpg");

	GameObject* tObj9 = tCurrentScene->AddObject("TextTest");
	tObj9->GetComponent<Transform>()->SetPosition({ 10.0f, 100.f, 0.f });
	tObj9->GetComponent<Transform>()->SetIs3D(false);
	tObj9->AddComponent<TextRenderer>();
	tObj9->GetComponent<TextRenderer>()->SetFont("../Resources/Fonts/NotoSansKR_16.spritefont");
	tObj9->GetComponent<TextRenderer>()->SetString("Paragon Engine");

	Pg::Data::GameObject* tText_1 = tCurrentScene->AddObject("TextTest");
	tText_1->GetComponent<Transform>()->SetPosition({ 10.0f, 250.f, 0.f });
	tText_1->GetComponent<Transform>()->SetIs3D(false);
	tText_1->AddComponent<TextRenderer>();
	tText_1->GetComponent<TextRenderer>()->SetFont("../Resources/Fonts/NotoSansKR_13.spritefont");
	tText_1->GetComponent<TextRenderer>()->SetString("Let's Go!!");

	GameObject* tObj10 = tCurrentScene->AddObject("TextTest");
	tObj10->GetComponent<Transform>()->SetPosition({ 10.0f, 10.f, 0.f });
	tObj10->GetComponent<Transform>()->SetIs3D(false);
	tObj10->AddComponent<TextRenderer>();
	tObj10->GetComponent<TextRenderer>()->SetFont("../Resources/Fonts/NotoSansKR_13.spritefont");
	tObj10->GetComponent<TextRenderer>()->SetString("[9] Previous Render Target\n[0] Next Render Target");

	//FPS 정보 출력.
	Pg::Data::GameObject* tText_2 = tCurrentScene->AddObject("FPSTextTest");
	tText_2->GetComponent<Transform>()->SetPosition({ 10.0f, 350.f, 0.f });
	tText_2->GetComponent<Transform>()->SetIs3D(false);
	tText_2->AddComponent<TextRenderer>();
	tText_2->GetComponent<TextRenderer>()->SetFont("../Resources/Fonts/NotoSansKR_13.spritefont");
	tText_2->GetComponent<TextRenderer>()->SetString("NULL");
	tText_2->AddComponent<PrintRuntimeData>();



	//이렇게 하면 메인 카메라 바뀜!!
	tCurrentScene->SetMainCamera(tObj1->GetComponent<Camera>());

	assert(true);
}

/*tObj2->GetComponent<Pg::Data::BoxCollider>()->FreezeAxisX(true);
	tObj2->GetComponent<Pg::Data::BoxCollider>()->FreezeAxisY(true);
	tObj2->GetComponent<Pg::Data::BoxCollider>()->FreezeAxisZ(true);*/

Pg::Data::Scene* Pg::Engine::TestScene::GetCurrentScene()
{
	return tCurrentScene;
}
