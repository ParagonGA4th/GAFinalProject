#include "ArtifactBoxAnim.h"
#include "ArtifactBox.h"
#include "CombatSystem.h"
#include "EventList_PlayerRelated.h"

#include "../ParagonData/StaticBoxCollider.h"
#include "../ParagonData/SkinnedMeshRenderer.h"

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
	}

	void ArtifactBoxAnim::Awake()
	{
		for (auto child : _object->_transform.GetChildren())
		{
			_artiBox = child->_object->GetComponent<Pg::DataScript::ArtifactBox>();
			assert(_artiBox != nullptr);
		}

		_renderer = _object->GetComponent<Pg::Data::SkinnedMeshRenderer>();
		_renderer->SetAnimation("OB_00002.pganim", false);
		_renderer->PauseAnim();

		_renderer->SetActive(true);
		auto col = _object->GetComponent<Pg::Data::StaticBoxCollider>();
		assert(col != nullptr);

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

			_object->GetComponent<Pg::Data::StaticBoxCollider>()->SetActive(false);
			_renderer->_object->SetActive(false);
			_animEnd = false;

			_combatSystem->Post(Event_PlayerGetArtifact(), NULL, NULL);
		}

		if (_artiBox->_isOpen)
		{
			_renderer->PlayAnim();
			_artiBox->_isOpen = false;
		}
	}

	void ArtifactBoxAnim::OnAnimationEnd(const std::string& justEndedAnimation)
	{
		_animEnd = true;
	}
}