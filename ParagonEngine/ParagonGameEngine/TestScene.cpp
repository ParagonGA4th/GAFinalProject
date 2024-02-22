#include "TestScene.h"
#include "../ParagonData/Camera.h"
#include "../ParagonData/ImageRenderer.h"
#include "../ParagonData/Button.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/Transform.h"
#include "../ParagonData/BoxCollider.h"
#include "../ParagonData/StaticBoxCollider.h"
#include "../ParagonData/SphereCollider.h"
#include "../ParagonData/PlaneCollider.h"
#include "../ParagonData/CapsuleCollider.h"

//#include "../ParagonData/BaseRenderer.h" //Render ���� ���� ����.
//#include "../ParagonData/RendererBase2D.h" //Render ���� ���� ����.
//#include "../ParagonData/RendererBase3D.h" //Render ���� ���� ����.
#include "../ParagonData/ImageRenderer.h" //Render ���� ���� ����.
#include "../ParagonData/TextRenderer.h" //Render ���� ���� ����.
#include "../ParagonData/StaticMeshRenderer.h" //Render ���� ���� ����.



// Script<->Component Ȯ���ϱ� ����.
#include "../ParagonData/MoveForwardBack.h"
#include "PrintRuntimeData.h"
#include "EditorCameraScript.h"
#include "MovingTest.h"
#include "RayCastTest.h"
#include "EventTest.h"

#include "../ParagonData/SpotLight.h"
#include "../ParagonData/PointLight.h"

Pg::Engine::TestScene::TestScene()
{
	tCurrentScene = new Pg::Data::Scene("TestCurrentSceneWorkspace");
}

void Pg::Engine::TestScene::Initialize()
{
	using namespace Pg::Data;

	//���� ���⼭ MainCamera �ֱ� ����.

	Pg::Data::GameObject* tObj2 = tCurrentScene->AddObject("RoadLavaConeTest");
	tObj2->GetComponent<Transform>()->_position = { 0.0f, 10.0f, 0.0f };
	tObj2->AddComponent<StaticMeshRenderer>();
	tObj2->GetComponent<StaticMeshRenderer>()->SetMeshFilePath("../Resources/3DModels/BasicMesh/Cube/Cube.fbx");
	tObj2->GetComponent<StaticMeshRenderer>()->SetMaterialFilePath("../ShaderResources/Materials/RoadLavaConeTestMat.pgmat");
	tObj2->GetComponent<StaticMeshRenderer>()->SetActive(true);

	GameObject* tObj2_1 = tCurrentScene->AddObject("Sphere1");
	tObj2_1->GetComponent<Transform>()->_position = { 3.0f, 10.0f, 0.0f };
	tObj2_1->AddComponent<SphereCollider>();
	tObj2_1->GetComponent<SphereCollider>()->SetRadius(2.0f);
	//tObj2_1->RemoveComponent<SphereCollider>();
	tObj2_1->AddComponent<StaticMeshRenderer>();
	tObj2_1->GetComponent<StaticMeshRenderer>()->SetMeshFilePath("../Resources/3DModels/StaticMesh/DefaultGeometry/DefaultGeometry.fbx");
	//tObj2_1->GetComponent<StaticMeshRenderer>()->SetMeshFilePath("../Resources/3DModels/UnrealTest/test.fbx");
	tObj2_1->GetComponent<StaticMeshRenderer>()->SetActive(true);


	GameObject* tObj2_2 = tCurrentScene->AddObject("Capsule1");
	tObj2_2->GetComponent<Transform>()->_position = { 1.0f, 10.0f, 0.0f };
	tObj2_2->AddComponent<CapsuleCollider>();
	tObj2_2->GetComponent<CapsuleCollider>()->FreezeAxisX(true);
	tObj2_2->GetComponent<CapsuleCollider>()->FreezeAxisY(true);
	tObj2_2->GetComponent<CapsuleCollider>()->FreezeAxisZ(true);
	//tObj2_2->GetComponent<CapsuleCollider>()->SetTrigger(true);
	//tObj2_2->GetComponent<CapsuleCollider>()->SetPoisitonOffset({ 1.0f, 0.0f, 0.0f });
	//tObj2_2->GetComponent<CapsuleCollider>()->SetRotationOffset ({ 0.0f, 90.0f, 0.0f, 0.0f });
	tObj2_2->AddComponent<StaticMeshRenderer>();
	tObj2_2->GetComponent<StaticMeshRenderer>()->SetMeshFilePath("../Resources/3DModels/BasicMesh/Capsule/Capsule.fbx");
	tObj2_2->GetComponent<StaticMeshRenderer>()->SetActive(true);
	tObj2_2->AddComponent<MovingTest>();
	tObj2_2->AddComponent<MovingTest>()->SetActive(true);
	tObj2_2->AddComponent<RayCastTest>();
	//tObj2_2->AddComponent<RayCastTest>()->SetActive(true);
	//tObj2_2->AddComponent<MoveForwardBack>();
	//tObj2_2->GetComponent<MoveForwardBack>()->SetActive(false);
	//
	//
	Pg::Data::GameObject* tObj3 = tCurrentScene->AddObject("Capsule2");
	tObj3->GetComponent<Transform>()->_position = { -1.0f, 5.0f, 5.0f };
	tObj3->GetComponent<Transform>()->_scale = { 3.0f, 3.0f, 3.0f };
	//tObj3->GetComponent<Transform>()->SetLocalRotationEuler(0.0f, 100.0f, 0.0f);
	//tObj3->AddComponent<CapsuleCollider>();
	//tObj3->GetComponent<CapsuleCollider>()->SetCapsuleInfo(3.0f, 3.0f);
	tObj3->AddComponent<StaticMeshRenderer>();
	tObj3->GetComponent<StaticMeshRenderer>()->SetActive(true);
	tObj3->GetComponent<StaticMeshRenderer>()->SetMeshFilePath("../Resources/3DModels/BasicMesh/Capsule/Capsule.fbx");
	//tObj3->AddComponent<MoveForwardBack>();
	//tObj3->GetComponent<MoveForwardBack>()->SetActive(false);

	Pg::Data::GameObject* tObj4 = tCurrentScene->AddObject("Cube6");
	tObj4->GetComponent<Transform>()->_position = { -2.0f, 1.0f, 0.0f };
	tObj4->GetComponent<Transform>()->_scale = { 1.0f, 1.0f, 1.0f };
	//tObj4->GetComponent<Transform>()->SetLocalRotationEuler(0.5f, 1.5f, -2.0f);
	tObj4->AddComponent<StaticBoxCollider>();
	tObj4->GetComponent<StaticBoxCollider>()->SetTrigger(true);
	//tObj4->GetComponent<StaticBoxCollider>()->_scale(2.0f, 2.0f, 2.0f);
	tObj4->AddComponent<StaticMeshRenderer>();
	tObj4->GetComponent<StaticMeshRenderer>()->SetMeshFilePath("../Resources/3DModels/StaticMesh/SimpleCube/simplecube.fbx");
	tObj4->GetComponent<StaticMeshRenderer>()->SetActive(true);
	//tObj4->RemoveComponent<StaticMeshRenderer>();
	//tObj4->AddComponent<MoveForwardBack>();
	//tObj4->GetComponent<MoveForwardBack>()->SetActive(true);

	Pg::Data::GameObject* tObj5 = tCurrentScene->AddObject("Cube7");
	tObj5->GetComponent<Transform>()->_position = { 5.0f, 0.0f, 0.0f };
	tObj5->GetComponent<Transform>()->_rotation = tObj5->GetComponent<Transform>()->EulerToQuaternion(1.5f, 2.0f, 4.0f);
	//tObj5->AddComponent<BoxCollider>();
	//tObj5->GetComponent<BoxCollider>()->SetPoisitonOffset({ 1.0f,0.0f,0.0f });
	tObj5->AddComponent<StaticMeshRenderer>();
	tObj5->GetComponent<StaticMeshRenderer>()->SetMeshFilePath("../Resources/3DModels/StaticMesh/twcylinder/twcylinder.fbx");
	tObj5->GetComponent<StaticMeshRenderer>()->SetActive(true);
	//tObj5->AddComponent<MoveForwardBack>();
	//tObj5->GetComponent<MoveForwardBack>()->SetActive(true);

	Pg::Data::GameObject* tObj6 = tCurrentScene->AddObject("Cube8");
	tObj6->GetComponent<Transform>()->_position = { -5.0f, 0.0f, 0.0f };
	tObj6->GetComponent<Transform>()->_scale = { 2.0f, 2.0f, 2.0f };
	tObj6->GetComponent<Transform>()->_rotation = tObj6->GetComponent<Transform>()->EulerToQuaternion(0.0f, -1.57f, 0.0f);
	//tObj6->AddComponent<BoxCollider>();
	tObj6->AddComponent<StaticMeshRenderer>();
	tObj6->GetComponent<StaticMeshRenderer>()->SetMeshFilePath("../Resources/3DModels/StaticMesh/SimpleCube/simplecube.fbx");
	tObj6->GetComponent<StaticMeshRenderer>()->SetActive(true);

	Pg::Data::GameObject* tObj7 = tCurrentScene->AddObject("Plane");
	tObj7->GetComponent<Transform>()->_position = { 0.0f, 0.0f, 0.0f };
	tObj7->GetComponent<Transform>()->_scale = { 1.0f, 1.0f, 1.0f };
	tObj7->GetComponent<Transform>()->_rotation = tObj7->GetComponent<Transform>()->EulerToQuaternion(0.0f, 0.0f, 0.0f);
	tObj7->AddComponent<PlaneCollider>();
	//tObj7->GetComponent<PlaneCollider>();
	tObj7->AddComponent<StaticMeshRenderer>();
	tObj7->GetComponent<StaticMeshRenderer>()->SetMeshFilePath("../Resources/3DModels/BasicMesh/Plane/plane.fbx");
	tObj7->GetComponent<StaticMeshRenderer>()->SetActive(true);

	///���� �߰��� ����Ʈ�� UI ������Ʈ �׽�Ʈ�� ���� �ڵ� 
	/// 
	Pg::Data::GameObject* tObj8 = tCurrentScene->AddObject("2DButton");
	tObj8->GetComponent<Transform>()->_position = { 100.0f, 200.0f, 0.0f };
	tObj8->AddComponent<Button>();
	tObj8->GetComponent<Button>()->SetImagePath("../Resources/Textures/Sprites/StartCrunch.png");
	tObj8->GetComponent<Button>()->SetImageSize(200.0f, 200.0f);
	tObj8->AddComponent<EventTest>();
	//tObj8->AddComponent<ImageRenderer>();
	//tObj8->GetComponent<ImageRenderer>()->SetImagePath("../Resources/Textures/Sprites/StartCrunch.png");
	//tObj8->GetComponent<ImageRenderer>()->SetSize(200.0f, 300.0f);
	//tObj8->GetComponent<ImageRenderer>()->SetActive(true);


	Pg::Data::GameObject* tObj20 = tCurrentScene->AddObject("TestingPlane");
	tObj20->GetComponent<Transform>()->_position = { 30.0f, 0.0f, 0.0f };
	tObj20->GetComponent<Transform>()->_scale = { 1.0f, 1.0f, 1.0f };
	tObj20->GetComponent<Transform>()->_rotation = tObj20->GetComponent<Transform>()->EulerToQuaternion(0.0f, 0.0f, 0.0f);
	tObj20->AddComponent<PlaneCollider>();
	tObj20->AddComponent<StaticMeshRenderer>();
	tObj20->GetComponent<StaticMeshRenderer>()->SetMeshFilePath("../Resources/3DModels/BasicMesh/Plane/plane.fbx");
	tObj20->GetComponent<StaticMeshRenderer>()->SetMaterialFilePath("../ShaderResources/Materials/PlainDiffuseMaterial.pgmat");
	tObj20->GetComponent<StaticMeshRenderer>()->SetActive(true);

	GameObject* tObj21 = tCurrentScene->AddObject("TestingShadow");
	tObj21->GetComponent<Transform>()->_position = { 30.0f, 1.0f, 0.0f };
	tObj21->AddComponent<StaticMeshRenderer>();
	tObj21->GetComponent<StaticMeshRenderer>()->SetMeshFilePath("../Resources/3DModels/BasicMesh/Cube/Cube.fbx");
	tObj21->GetComponent<StaticMeshRenderer>()->SetMaterialFilePath("../ShaderResources/Materials/RoadLavaConeTestMat.pgmat");
	tObj21->GetComponent<StaticMeshRenderer>()->SetActive(true);

	GameObject* tObj22 = tCurrentScene->AddObject("TestingShadow2");
	tObj22->GetComponent<Transform>()->_position = { 40.0f, 10.0f, 0.0f };
	tObj22->GetComponent<Transform>()->_rotation = tObj22->GetComponent<Transform>()->EulerToQuaternion(1.5708f, -1.5708f, 0.f);
	tObj22->AddComponent<StaticMeshRenderer>();
	tObj22->GetComponent<StaticMeshRenderer>()->SetMeshFilePath("../Resources/3DModels/BasicMesh/Plane/plane.fbx");
	tObj22->GetComponent<StaticMeshRenderer>()->SetMaterialFilePath("../ShaderResources/Materials/ExperimentalMaterial.pgmat");
	tObj22->GetComponent<StaticMeshRenderer>()->SetActive(true);

	assert(true);
}

/*tObj2->GetComponent<Pg::Data::BoxCollider>()->FreezeAxisX(true);
	tObj2->GetComponent<Pg::Data::BoxCollider>()->FreezeAxisY(true);
	tObj2->GetComponent<Pg::Data::BoxCollider>()->FreezeAxisZ(true);*/

Pg::Data::Scene* Pg::Engine::TestScene::GetCurrentScene()
{
	return tCurrentScene;
}

void Pg::Engine::TestScene::SetScenesData(Pg::Data::Scene* scene)
{
	if (scene != nullptr)
	{
		for (auto i : tCurrentScene->GetObjectList())
		{
			for (auto j : scene->GetObjectList())
			{
				if (i->GetName() == "Cube6" && j->GetName() == "New Object")
				{
					i->_transform = j->_transform;
				}
				else continue;
			}
		}
	}
}