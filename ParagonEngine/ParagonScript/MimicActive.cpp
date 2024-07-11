#include "MimicActive.h"
#include "MimicBehaviour.h"
#include "../ParagonData/Collider.h"
#include "../ParagonData/StaticBoxCollider.h"
#include "../ParagonData/SkinnedMeshRenderer.h"
#include "../ParagonData/BoxCollider.h"
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
		_renderer = _object->GetComponent<Pg::Data::SkinnedMeshRenderer>();

		_mimic = _object->GetScene()->FindObjectWithName(_mimicName);
		assert(_mimic != nullptr);
		_mimicBehaviour = _mimic->GetComponent<MimicBehaviour>();
	}

	void MimicActive::Awake()
	{

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
				//미믹 행동 시작.
				_mimicBehaviour->SetActive(true);
				_mimicBehaviour->_meshRenderer->SetActive(true);
				_mimicBehaviour->_collider->SetActive(true);

				//박스 삭제.
				_collider->SetActive(false);
				_renderer->SetActive(false);
				_object->SetActive(false);
			}
		}
	}
}