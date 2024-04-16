#include "PlayerBehavior.h"

#include "../ParagonData/Camera.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/Scene.h"
#include "../ParagonData/DynamicCollider.h"

namespace Pg::DataScript
{
	PlayerBehavior::PlayerBehavior(Pg::Data::GameObject* obj) : ScriptInterface(obj)
	{

	}

	void PlayerBehavior::Awake()
	{
		mainCam = _object->GetScene()->GetMainCamera();
		selfCol = _object->GetComponent<Pg::Data::DynamicCollider>();
		//se
	}

	void PlayerBehavior::Start()
	{

	}

	void PlayerBehavior::Update()
	{

	}

}