#include "ArtifactBox.h"

#include "../ParagonData/Collider.h"
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

	void ArtifactBox::BeforePhysicsAwake()
	{
	}

	void ArtifactBox::Awake()
	{
		std::string objName = _object->GetName();
		std::string num = objName.substr(objName.size() - 1, 1);
		auto childObj = _object->_transform.GetChild("ArtifactBox_" + num)->_object;
		
		_renderer = childObj->GetComponent<Pg::Data::SkinnedMeshRenderer>();
		//childObj->SetActive(true);
		//childObj->GetComponent<Pg::Data::StaticBoxCollider>()->SetActive(true);

		//_renderer->SetActive(true);

		_renderer->SetAnimation("OB_00002", false);
		_renderer->PauseAnim();
	}

	void ArtifactBox::Update()
	{
		// 애니매이션 완료 후 
		if (_animEnd)
		{
			// active 끄기
			_renderer->_object->SetActive(false);
			_renderer->_object->GetComponent<Pg::Data::StaticBoxCollider>()->SetActive(false);
			_renderer->SetActive(false);
			_animEnd = false;
		}

		if (_onTriggerStay)
		{
			 //상호작용 키
			if (_pgInput->GetKeyDown(Pg::API::Input::eKeyCode::KeyF))
			{
				// 애니매이션 재생
				_renderer->PlayAnim();
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
				_onTriggerStay = false;
			}
		}
	}

	void ArtifactBox::OnAnimationEnd(const std::string& justEndedAnimation)
	{
		_animEnd = true;
	}
}