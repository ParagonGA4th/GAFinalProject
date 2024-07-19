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
		//순서가 맞지 않을 수 있기에, 수정.
		Pg::Data::GameObject* tArtifactChild = nullptr;
		Pg::Data::GameObject* tOtherChild = nullptr;

		//진짜 갖고 있는 애 찾기.
		auto& tChildrenList = _object->_transform.GetChildren();
		unsigned int tListSize = tChildrenList.size();
		for (int i = 0; i < tListSize; i++)
		{
			auto child = tChildrenList.at(i)->_object;
			Pg::DataScript::ArtifactBox* tArtiBox = child->GetComponent<Pg::DataScript::ArtifactBox>();
			if (tArtiBox != nullptr)
			{
				tArtifactChild = child;
				_artiBox = tArtiBox;
			}
			else
			{
				tOtherChild = child;
			}
		}
		
		assert(_artiBox != nullptr);

		_renderer = _object->GetComponent<Pg::Data::SkinnedMeshRenderer>();
		_renderer->SetAnimation("OB_00002.pganim", false);
		_renderer->PauseAnim();

		_renderer->SetActive(true);
		auto col = _object->GetComponent<Pg::Data::StaticBoxCollider>();
		if (col != nullptr) col->SetActive(true);
		else
		{
			//auto child = _object->_transform.GetChildren().at(1)->_object;
			auto child = tOtherChild;
			auto ccol = child->GetComponent<Pg::Data::StaticBoxCollider>();
			if (ccol != nullptr) ccol->SetActive(true);
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
				auto ccol = child->GetComponent<Pg::Data::StaticBoxCollider>();
				if (ccol != nullptr) ccol->SetActive(true);
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