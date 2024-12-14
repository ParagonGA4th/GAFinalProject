#pragma once
#include "ScriptInterface.h"
#include "ISingleGUI.h"

#include "../ParagonData/ImageRenderer.h"

namespace Pg::DataScript
{
	class GUI_PercentageFillBar : public ScriptInterface<GUI_PercentageFillBar>, public ISingleGUI
	{
		DEFINE_PARAGON_SCRIPT(GUI_PercentageFillBar);

	public:
		GUI_PercentageFillBar(Pg::Data::GameObject* obj);	
	public:
		virtual void GrabManagedObjects();
		virtual void ReceiveDependentPointers(void* p1, void* p2, void* p3, double v1, double v2, double v3) override;
		virtual void Update();

	private:
		const float* _percentageSource{ nullptr };
		float _maxVal;
		Pg::Data::ImageRenderer* _imageRenderer{ nullptr };
	};
}


