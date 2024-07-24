#include "Portal.h"

#include "../ParagonData/BoxCollider.h"
#include "../ParagonAPI/PgScene.h"
#include "TotalGameManager.h"

#include <singleton-cpp/singleton.h>

Pg::DataScript::Portal::Portal(Pg::Data::GameObject* obj)
	:ScriptInterface(obj)
{

}

void Pg::DataScript::Portal::Awake()
{
	_collider = _object->GetComponent<Pg::Data::BoxCollider>();
	assert(_collider != nullptr);

	_sceneHelper = &singleton<Pg::API::PgScene>();
	//_sceneHelper->Initialize();
}

void Pg::DataScript::Portal::Start()
{

}

void Pg::DataScript::Portal::Update()
{

}

void Pg::DataScript::Portal::SetNextScene(std::string nextScene)
{
	_nextScene = nextScene;
}

void Pg::DataScript::Portal::OnTriggerEnter(Pg::Data::Collider** _colArr, unsigned int count)
{
	for (int i = 0; i < count; i++)
	{
		Pg::Data::Collider* col = _colArr[i];

		if (col->_object->GetName() == "PlayerSensor")
		{
			if (_sceneHelper->GetCurrentScene()->GetSceneName() == "Stage1")
			{
				_sceneHelper->SetCurrentScene("Stage2");
			}
			//else if (_sceneHelper->GetCurrentScene()->GetSceneName() == "Stage2")
			//{
			//	_sceneHelper->SetCurrentScene("BossStage");
			//}
			//_sceneHelper->SetCurrentScene(_nextScene);
		}
	}
}
