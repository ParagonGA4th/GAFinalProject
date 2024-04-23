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
}

void Pg::DataScript::Portal::Start()
{

}

void Pg::DataScript::Portal::Update()
{

}

void Pg::DataScript::Portal::OnTriggerEnter(Pg::Data::Collider* col)
{
	if (col->_object->GetTag() == "TAG_Player")
	{
		//if (_object->GetScene()->GetSceneName() == "stage1")
		//{
		//	_sceneHelper->SetCurrentScene("stage1");
		//}
	}
}
