#include "TestScene.h"
#include "../ParagonCore/Scene.h"
#include "../ParagonCore/GameObject.h"
#include "../ParagonCore/Transform.h"

void TestScene::Initialize()
{
	using namespace Pg::Core;

	Scene* tCurrentScene = new Scene("TestCurrentScene");
	GameObject* tObj1 = new GameObject("Cube1");
	tObj1->GetComponent<Transform>()->SetPosition({ 10.0f, 0.0f, 5.0f });
	GameObject* tObj2 = new GameObject("Cube2");
	tObj2->GetComponent<Transform>()->SetPosition({ -10.0f, 0.0f, 5.0f });
	tCurrentScene->AddObject(tObj1);
	tCurrentScene->AddObject(tObj2);
}
