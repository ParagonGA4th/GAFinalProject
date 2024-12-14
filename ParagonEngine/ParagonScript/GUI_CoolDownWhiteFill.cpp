#include "GUI_CoolDownWhiteFill.h"
#include "../ParagonUtil/Log.h"
#include <cassert>
#include <algorithm>
#include <cmath>
#include <numbers>

namespace Pg::DataScript
{
	GUI_CoolDownWhiteFill::GUI_CoolDownWhiteFill(Pg::Data::GameObject* obj) : ScriptInterface(obj)
	{

	}

	void GUI_CoolDownWhiteFill::GrabManagedObjects()
	{
		//Image Renderer ����.
		_imageRenderer = _object->GetComponent<Pg::Data::ImageRenderer>();
		assert(_imageRenderer != nullptr);
		//UpToDown����.
		_imageRenderer->SetFillRatioDirection(Pg::Data::eFillRatioDirection::UP_TO_DOWN);
		_imageRenderer->_sortingLayer = 2;
	}

	void GUI_CoolDownWhiteFill::ReceiveDependentPointers(void* p1, void* p2, void* p3, double v1, double v2, double v3)
	{
		//������ ����ؼ� ����� �� �ְ� �ϱ� ���ؼ�.
		_percentageSource = (const float*)p1;

		//������ Bool������ �������� + Mana���� �ؼ� ���͸��ؾ� �Ѵ�.
		_isStartEligible = (const bool*)p2;
		_isCheckedBoolPointer = (const bool*)p3;

		//
		_maxVal = v1;
		_requiredSkillManaAmt = v2;
	}

	void GUI_CoolDownWhiteFill::Update()
	{
		//ó������ �ȵǴ� ���� ����.
		float tRatio = 100.f;

		bool tIsStartEligible = *_isStartEligible;
		bool tBool = *_isCheckedBoolPointer;
		//�ߵ��� �ȴٴ� ���.
		if (tIsStartEligible)
		{
			//������ ����.
			if (tBool)
			{
				tRatio = 0.f;
			}
			else
			{
				//�Ұ���.
				tRatio = 100.f;
			}
		}
		else
		{
			float tImpVal = std::clamp<float>(*_percentageSource, 0.f, _maxVal);
			tRatio = (tImpVal / _maxVal) * 100.f; // 0-100 ����.
			tRatio = 100.0f - tRatio;
		}

		_imageRenderer->SetFillRatio(tRatio);
	}


}
