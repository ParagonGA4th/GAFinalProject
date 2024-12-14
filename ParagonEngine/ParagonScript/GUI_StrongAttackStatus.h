#pragma once
#include "ScriptInterface.h"
#include "ISingleGUI.h"

#include "../ParagonData/ImageRenderer.h"
#include <functional>
#include <windows.h>

namespace Pg::API
{
	namespace Input { class PgInput; }
	class PgScene;
}

namespace Pg::DataScript
{
	class GUI_StrongAttackStatus : public ScriptInterface<GUI_StrongAttackStatus>, public ISingleGUI
	{
		DEFINE_PARAGON_SCRIPT(GUI_StrongAttackStatus);

	public:
		inline static const float RIGHT_OFFSET_AMT = 50.f;

	public:
		GUI_StrongAttackStatus(Pg::Data::GameObject* obj);
		virtual void GrabManagedObjects();
		virtual void ReceiveDependentPointers(void* p1, void* p2, void* p3, double v1, double v2, double v3) override;
		virtual void Start() override;
		virtual void Update() override;

	private:
		void UpdateFillAmt();
		void UpdatePosition();

	private:
		Pg::Data::ImageRenderer* _imageRendererFrame{ nullptr };
		Pg::Data::ImageRenderer* _imageRendererBar{ nullptr };

		const float* _percentageSource{ nullptr };
		const bool* _isStartEligible{ nullptr };
		const bool* _isCheckedBoolPointer{ nullptr };

		float _maxVal;
		//unsigned int _requiredSkillStaminaAmt{ 3 };
		Pg::API::Input::PgInput* _pgInput{ nullptr };
		Pg::API::PgScene* _pgScene{ nullptr };

		HWND _hwnd;

	};
}


