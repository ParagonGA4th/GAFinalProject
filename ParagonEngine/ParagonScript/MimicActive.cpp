#include "MimicActive.h"
#include "MimicBehaviour.h"

#include "../ParagonData/Collider.h"
#include "../ParagonData/StaticBoxCollider.h"
#include "../ParagonData/SkinnedMeshRenderer.h"
#include "../ParagonData/BoxCollider.h"
#include "../ParagonData/AudioSource.h"
#include "../ParagonData/CapsuleCollider.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/Scene.h"
#include "../ParagonAPI/PgInput.h"

#include <singleton-cpp/singleton.h>

namespace Pg::DataScript
{
	MimicActive::MimicActive(Pg::Data::GameObject* obj) :
		ScriptInterface(obj)
	{
		_pgInput = &singleton<Pg::API::Input::PgInput>();
	}

	void MimicActive::GrabManagedObjects()
	{
		Pg::Data::GameObject* _boxOpen = _object->GetScene()->FindObjectWithName("BoxOpenSound");
		_boxOpenSound = _boxOpen->GetComponent<Pg::Data::AudioSource>();
	}

	void MimicActive::BeforePhysicsAwake()
	{
		_collider = _object->GetComponent<Pg::Data::StaticBoxCollider>();
		_renderer = _object->GetComponent<Pg::Data::SkinnedMeshRenderer>();
		_renderer->SetRendererOffset(_rendererOffset);

		_mimic = _object->GetScene()->FindObjectWithName(_mimicName);
		assert(_mimic != nullptr);
		_mimicBehaviour = _mimic->GetComponent<MimicBehaviour>();
	}

	void MimicActive::Awake()
	{
		_renderer->SetAnimation("OB_00002.pganim", false);
		_renderer->PauseAnim();
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
		if (_onTriggerStay)
		{
			//상호작용 키
			if (_pgInput->GetKeyDown(Pg::API::Input::eKeyCode::KeyF))
			{
				//플레이어가 상호작용하면 미믹으로 바껴라
				//미믹 행동 시작.
				_mimicBehaviour->SetActive(true);
				_mimicBehaviour->_meshRenderer->SetActive(true);
				_mimicBehaviour->_collider->SetActive(true);
				_boxOpenSound->Play();

				//박스 삭제.
				_collider->SetActive(false);
				_renderer->SetActive(false);
				_object->SetActive(false);
			}
		}
	}

	void MimicActive::OnTriggerEnter(Pg::Data::Collider** _colArr, unsigned int count)
	{
		for (int i = 0; i < count; i++)
		{
			Pg::Data::Collider* col = _colArr[i];

			if (col->_object->GetTag() == "TAG_Player")
			{
				_onTriggerStay = true;
			}
		}
	}

	void MimicActive::OnTriggerExit(Pg::Data::Collider** _colArr, unsigned int count)
	{
		for (int i = 0; i < count; i++)
		{
			Pg::Data::Collider* col = _colArr[i];

			if (col->_object->GetTag() == "TAG_Player")
			{
				_onTriggerStay = false;
			}
		}
	}

}