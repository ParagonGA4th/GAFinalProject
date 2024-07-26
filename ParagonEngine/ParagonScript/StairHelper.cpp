#include "StairHelper.h"
#include "../ParagonData/Collider.h"
#include "../ParagonAPI/PgGraphics.h"

#include <singleton-cpp/singleton.h>

namespace Pg::DataScript
{
	StairHelper::StairHelper(Pg::Data::GameObject* obj)
		:ScriptInterface(obj)
	{
		_pgGraphics = &singleton<Pg::API::Graphics::PgGraphics>();
	}

	StairHelper::~StairHelper()
	{
		if (_instructionRO != nullptr)
		{
			_pgGraphics->RemoveEffectObject(_instructionRO);

			delete _instructionRO;
			_instructionRO = nullptr;
		}
	}

	void StairHelper::Awake()
	{
		if (_instructionRO == nullptr)
		{
			_instructionRO = new Pg::Data::VisualEffectRenderObject();
			_pgGraphics->RegisterEffectObject("Effect_BB_HowToJump", _instructionRO);
			_instructionRO->SetActive(false);
			_instructionRO->_position = _object->_transform._position + Pg::Math::PGFLOAT3(0, 1, 0);
			_instructionRO->_scale = { 7,1,1 };
		}
	}

	void StairHelper::EffectHelper(bool isActive)
	{
		_instructionRO->SetActive(isActive);
	}

	void StairHelper::OnTriggerEnter(Pg::Data::Collider** _colArr, unsigned int count)
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
	
	void StairHelper::OnTriggerExit(Pg::Data::Collider** _colArr, unsigned int count)
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
}