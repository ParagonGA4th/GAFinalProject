#include "MimicActive.h"
#include "../ParagonData/Collider.h"
#include "../ParagonData/StaticBoxCollider.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/Scene.h"
#include "../ParagonData/Scene.h"

namespace Pg::DataScript
{
	MimicActive::MimicActive(Pg::Data::GameObject* obj) :
		ScriptInterface(obj)
	{

	}

	void MimicActive::BeforePhysicsAwake()
	{
		_collider = _object->GetComponent<Pg::Data::StaticBoxCollider>();
	}

	void MimicActive::Awake()
	{
		_mimic = _object->GetScene()->FindObjectWithName(_mimicName);
	}

	void MimicActive::OnDeserialize(SerializeVector& sv)
	{
		Pg::Data::SerializerHelper::OnDeserializerHelper(this, sv);
	}

	void MimicActive::OnSerialize(SerializeVector& sv)
	{
		Pg::Data::SerializerHelper::OnSerializerHelper(this, sv);
	}

	void MimicActive::Update()
	{

	}

	void MimicActive::OnTriggerEnter(Pg::Data::Collider** _colArr, unsigned int count)
	{
		for (int i = 0; i < count; i++)
		{
			Pg::Data::Collider* col = _colArr[i];

			//플레이어가 상호작용하면 미믹으로 바껴라
			if (col->_object->GetTag() == "TAG_Player")
			{
				//_mimic->SetActive(true);
				_collider->SetActive(false);
				_object->SetActive(false);

			}
		}
	}
}