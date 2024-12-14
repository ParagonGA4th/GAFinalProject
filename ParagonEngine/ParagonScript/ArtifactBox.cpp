#include "ArtifactBox.h"

#include "../ParagonData/Collider.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/ImageRenderer.h"
#include "../ParagonData/Scene.h"
#include "../ParagonData/StaticBoxCollider.h"
#include "../ParagonData/SkinnedMeshRenderer.h"
#include "../ParagonAPI/PgInput.h"
#include "../ParagonAPI/PgGraphics.h"

#include <singleton-cpp/singleton.h>

namespace Pg::DataScript
{
	ArtifactBox::ArtifactBox(Pg::Data::GameObject* obj)
		:ScriptInterface(obj)
	{
		_pgInput = &singleton<Pg::API::Input::PgInput>();
		_pgGraphics = &singleton<Pg::API::Graphics::PgGraphics>();
	}

	ArtifactBox::~ArtifactBox()
	{
		if (_instructionRO != nullptr)
		{
			_pgGraphics->RemoveEffectObject(_instructionRO);

			delete _instructionRO;
			_instructionRO = nullptr;
		}
	}

	void ArtifactBox::GrabManagedObjects()
	{
		//_interactionUI = _object->GetScene()->FindObjectWithName("InteractionKeyUI");
		//_interaction = _interactionUI->GetComponent<Pg::Data::ImageRenderer>();
	}

	void ArtifactBox::BeforePhysicsAwake()
	{
	}

	void ArtifactBox::Awake()
	{
		//Test.
		if (_instructionRO == nullptr)
		{
			_instructionRO = new Pg::Data::VisualEffectRenderObject();
			_pgGraphics->RegisterEffectObject("Effect_BB_HowToInteract", _instructionRO);
			_instructionRO->SetActive(false);
			_instructionRO->_position = _object->_transform._position + Pg::Math::PGFLOAT3(0, 1, 0);
			_instructionRO->_scale = { 3,1,1 };
		}
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
				if (_object->GetScene()->GetSceneName() == "Stage1") _instructionRO->SetActive(true);

				//_interactionUI->SetActive(true);
				//_interaction->SetActive(true);
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
				//_interactionUI->SetActive(false);
				//_interaction->SetActive(false);
				_instructionRO->SetActive(false);
				_onTriggerStay = false;
			}
		}
	}



}