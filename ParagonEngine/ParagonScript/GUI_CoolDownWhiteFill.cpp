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
		_maxVal = v1;
	}

	void GUI_CoolDownWhiteFill::Update()
	{
		float tImpVal = std::clamp<float>(*_percentageSource, 0.f, _maxVal);
		
		//테스트용.
		//{
		//	static float time = 0.1f;
		//	time += 0.01f;
		//
		//	const float frequency = 0.5;
		//	const float phase = 0;
		//
		//	// Calculate the sine value (-1 to 1)
		//	float sineValue = std::sin(2 * std::numbers::pi * frequency * time + phase);
		//
		//	// Map the sine value to the range 0-100
		//	tImpVal = (sineValue + 1) * 50;
		//}

		float tRatio = (tImpVal / _maxVal) * 100.f; // 0-100 사이.
		tRatio = 100.0f - tRatio;
		PG_WARN(tRatio);


		_imageRenderer->SetFillRatio(tRatio);
	}


}
