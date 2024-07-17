#include "ArtifactBoxAnim.h"
#include "ArtifactBox.h"

#include "../ParagonData/StaticBoxCollider.h"
#include "../ParagonData/SkinnedMeshRenderer.h"

#include <singleton-cpp/singleton.h>

namespace Pg::DataScript
{
	ArtifactBoxAnim::ArtifactBoxAnim(Pg::Data::GameObject* obj)
		:ScriptInterface(obj)
	{
	}

	void ArtifactBoxAnim::Awake()
	{
		auto child = _object->_transform.GetChildren().at(0)->_object;
		_artiBox = child->GetComponent<Pg::DataScript::ArtifactBox>();

		_renderer = _object->GetComponent<Pg::Data::SkinnedMeshRenderer>();
		_renderer->SetAnimation("OB_00002.pganim", false);
		_renderer->PauseAnim();

		_renderer->SetActive(true);
		auto col = _object->GetComponent<Pg::Data::StaticBoxCollider>();
		if (col != nullptr) col->SetActive(true);
		else
		{
			auto child = _object->_transform.GetChildren().at(1)->_object;
			if(child != nullptr) child->GetComponent<Pg::Data::StaticBoxCollider>()->SetActive(true);
		}
		_object->SetActive(true);
	}

	void ArtifactBoxAnim::Update()
	{
		// 애니매이션 완료 후 
		if (_animEnd)
		{
			// active 끄기
			_renderer->SetActive(false);

			auto col = _object->GetComponent<Pg::Data::StaticBoxCollider>();
			if (col != nullptr) col->SetActive(false);
			else
			{
				auto child = _object->_transform.GetChildren().at(1)->_object;
				if (child != nullptr) child->GetComponent<Pg::Data::StaticBoxCollider>()->SetActive(false);
			}
			_renderer->_object->SetActive(false);
			_animEnd = false;
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