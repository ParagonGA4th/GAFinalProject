#include "GUI_PercentageFillBar.h"
#include <cassert>
#include <algorithm>
#include <cmath>
#include <numbers>

namespace Pg::DataScript
{
	GUI_PercentageFillBar::GUI_PercentageFillBar(Pg::Data::GameObject* obj) : ScriptInterface(obj)
	{

	}

	void GUI_PercentageFillBar::GrabManagedObjects()
	{
		//Image Renderer 들어가기.
		_imageRenderer = _object->GetComponent<Pg::Data::ImageRenderer>();
		assert(_imageRenderer != nullptr);
	}

	void GUI_PercentageFillBar::ReceiveDependentPointers(void* p1, void* p2, void* p3, double v1, double v2, double v3)
	{
		//비율로 계산해서 출력할 수 있게 하기 위해서.
		_percentageSource = (const float*)p1;
		_maxVal = v1; 
	}

	void GUI_PercentageFillBar::Update()
	{
		float tImpVal = std::clamp<float>(*_percentageSource, 0.f, _maxVal);
		float tRatio = (tImpVal / _maxVal) * 100.f; // 0-100 사이.
		_imageRenderer->SetFillRatio(tRatio);
	}
	

}
