#include "ArtifactBox.h"

#include "../ParagonData/Collider.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/ImageRenderer.h"
#include "../ParagonData/Scene.h"
#include "../ParagonData/StaticBoxCollider.h"
#include "../ParagonData/SkinnedMeshRenderer.h"
#include "../ParagonAPI/PgInput.h"

#include <singleton-cpp/singleton.h>

namespace Pg::DataScript
{
	ArtifactBox::ArtifactBox(Pg::Data::GameObject* obj)
		:ScriptInterface(obj)
	{
		_pgInput = &singleton<Pg::API::Input::PgInput>();
	}

	void ArtifactBox::GrabManagedObjects()
	{
		_interactionUI = _object->GetScene()->FindObjectWithName("InteractionKeyUI");
		_interaction = _interactionUI->GetComponent<Pg::Data::ImageRenderer>();
	}

	void ArtifactBox::BeforePhysicsAwake()
	{
	}

	void ArtifactBox::Awake()
	{
	}

	void ArtifactBox::Update()
	{
		if (_onTriggerStay)
		{
			 //상호작용 키
			if (_pgInput->GetKeyDown(Pg::API::Input::eKeyCode::KeyF))
			{
				_isOpen = true;
			}
		}
	}

	void ArtifactBox::OnTriggerEnter(Pg::Data::Collider** _colArr, unsigned int count)
	{
		for (int i = 0; i < count; i++)
		{
			Pg::Data::Collider* col = _colArr[i];

			if (col->_object->GetTag() == "TAG_Sensor")
			{
				_interactionUI->SetActive(true);
				_interaction->SetActive(true);
				_onTriggerStay = true;
			}
		}
	}

	void ArtifactBox::OnTriggerExit(Pg::Data::Collider** _colArr, unsigned int count)
	{
		for (int i = 0; i < count; i++)
		{
			Pg::Data::Collider* col = _colArr[i];

			if (col->_object->GetTag() == "TAG_Sensor")
			{
				_interactionUI->SetActive(false);
				_interaction->SetActive(false);
				_onTriggerStay = false;
			}
		}
	}
}