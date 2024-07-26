#include "ArtifactBoxAnim.h"
#include "ArtifactBox.h"
#include "CombatSystem.h"
#include "EventList_PlayerRelated.h"

#include "../ParagonData/Scene.h"
#include "../ParagonData/StaticBoxCollider.h"
#include "../ParagonData/BoxCollider.h"
#include "../ParagonData/ImageRenderer.h"
#include "../ParagonData/AudioSource.h"
#include "../ParagonData/SkinnedMeshRenderer.h"
#include "../ParagonData/VisualEffectRenderObject.h"

#include <singleton-cpp/singleton.h>

namespace Pg::DataScript
{
	ArtifactBoxAnim::ArtifactBoxAnim(Pg::Data::GameObject* obj)
		:ScriptInterface(obj)
	{
	}

	void ArtifactBoxAnim::GrabManagedObjects()
	{
		_combatSystem = CombatSystem::GetInstance(nullptr);
		_renderer = _object->GetComponent<Pg::Data::SkinnedMeshRenderer>();

		for (auto child : _object->_transform.GetChildren())
		{
			_artiBox = child->_object->GetComponent<Pg::DataScript::ArtifactBox>();
			_artiBoxCol = child->_object->GetComponent<Pg::Data::BoxCollider>();
			assert(_artiBox != nullptr);
			_artiBoxCol->SetActive(true);
		}

		Pg::Data::GameObject* _interactionKey = _object->GetScene()->FindObjectWithName("InteractionKeyUI");
		_artiUI = _interactionKey->GetComponent<Pg::Data::ImageRenderer>();

		Pg::Data::GameObject* _boxOpen = _object->GetScene()->FindObjectWithName("BoxOpenSound");
		_boxOpenSound = _boxOpen->GetComponent<Pg::Data::AudioSource>();
	}

	void ArtifactBoxAnim::Awake()
	{
		_renderer->SetAnimation("OB_00002.pganim", false);
		_renderer->PauseAnim();

		_renderer->SetActive(true);
		auto col = _object->GetComponent<Pg::Data::StaticBoxCollider>();
		assert(col != nullptr);

		_artiBoxCol->SetActive(true);
		col->SetActive(true);
		_object->SetActive(true);
	}

	void ArtifactBoxAnim::Update()
	{
		// 애니매이션 완료 후 
		if (_animEnd)
		{
			// active 끄기
			_renderer->SetActive(false);
			_artiBoxCol->SetActive(false);
			_artiUI->SetActive(false);

			_object->GetComponent<Pg::Data::StaticBoxCollider>()->SetActive(false);
			_artiBox->_instructionRO->SetActive(false);
			_animEnd = false;

			if (!_isHp) _combatSystem->Post(Event_PlayerGetArtifact(), NULL, NULL);
			else _combatSystem->ChangePlayerHealth(30.f);
		}

		if (_artiBox->_isOpen)
		{
			_renderer->PlayAnim();
			_boxOpenSound->Play();
			_artiBox->_isOpen = false;
		}
	}

	void ArtifactBoxAnim::OnAnimationEnd(const std::string& justEndedAnimation)
	{
		_animEnd = true;
	}
}