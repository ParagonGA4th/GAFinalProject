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
		//Image Renderer 들어가기.
		_imageRenderer = _object->GetComponent<Pg::Data::ImageRenderer>();
		assert(_imageRenderer != nullptr);
		//UpToDown으로.
		_imageRenderer->SetFillRatioDirection(Pg::Data::eFillRatioDirection::UP_TO_DOWN);
		_imageRenderer->_sortingLayer = 2;
	}

	void GUI_CoolDownWhiteFill::ReceiveDependentPointers(void* p1, void* p2, void* p3, double v1, double v2, double v3)
	{
		//비율로 계산해서 출력할 수 있게 하기 위해서.
		_percentageSource = (const float*)p1;

		//이제는 Bool값으로 가능한지 + Mana까지 해서 필터링해야 한다.
		_isStartEligible = (const bool*)p2;
		_isCheckedBoolPointer = (const bool*)p3;

		//
		_maxVal = v1;
		_requiredSkillManaAmt = v2;
	}

	void GUI_CoolDownWhiteFill::Update()
	{
		//처음에는 안되는 것을 가정.
		float tRatio = 100.f;

		bool tIsStartEligible = *_isStartEligible;
		bool tBool = *_isCheckedBoolPointer;
		//발동은 된다는 얘기.
		if (tIsStartEligible)
		{
			//마나도 충족.
			if (tBool)
			{
				tRatio = 0.f;
			}
			else
			{
				//불가능.
				tRatio = 100.f;
			}
		}
		else
		{
			float tImpVal = std::clamp<float>(*_percentageSource, 0.f, _maxVal);
			tRatio = (tImpVal / _maxVal) * 100.f; // 0-100 사이.
			tRatio = 100.0f - tRatio;
		}

		_imageRenderer->SetFillRatio(tRatio);
	}


}
