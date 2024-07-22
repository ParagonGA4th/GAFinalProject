#include "BridgeHelper.h"

#include "../ParagonData/StaticBoxCollider.h"

namespace Pg::DataScript
{

	BridgeHelper::BridgeHelper(Pg::Data::GameObject* obj)
		:ScriptInterface(obj)
	{
	}

	void BridgeHelper::BeforePhysicsAwake()
	{
	}

	void BridgeHelper::Awake()
	{
	}

	void BridgeHelper::Update()
	{
	}

	void BridgeHelper::OnTriggerEnter(Pg::Data::Collider** _colArr, unsigned int count)
	{
		for (int i = 0; i < count; i++)
		{
			Pg::Data::Collider* col = _colArr[i];

			if (col->_object->GetTag() == "TAG_Player")
			{
				_isTrig = true;
			}
		}
	}
}