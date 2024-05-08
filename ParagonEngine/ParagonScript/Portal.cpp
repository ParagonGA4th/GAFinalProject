#include "Portal.h"

#include "../ParagonData/StaticBoxCollider.h"
#include "../ParagonAPI/PgScene.h"

Pg::DataScript::Portal::Portal(Pg::Data::GameObject* obj)
	:ScriptInterface(obj)
{

}

void Pg::DataScript::Portal::Awake()
{
	_collider = _object->GetComponent<Pg::Data::StaticBoxCollider>();
	assert(_collider != nullptr);

	_sceneHelper = new Pg::API::PgScene();
	_sceneHelper->Initialize();
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

void Pg::DataScript::Portal::OnTriggerEnter(Pg::Data::Collider* col)
{
	if (col->_object->GetTag() == "TAG_Player")
	{
		_sceneHelper->SetCurrentScene(_nextScene);
	}
}
