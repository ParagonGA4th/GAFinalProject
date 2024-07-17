#pragma once
#include "ScriptInterface.h"
#include "ISingleGUI.h"

#include "../ParagonData/ImageRenderer.h"

namespace Pg::DataScript
{
	class GUI_CoolDownWhiteFill : public ScriptInterface<GUI_CoolDownWhiteFill>, public ISingleGUI
	{
		DEFINE_PARAGON_SCRIPT(GUI_CoolDownWhiteFill);

	public:
		GUI_CoolDownWhiteFill(Pg::Data::GameObject* obj);
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


