#include "GUI_StrongAttackStatus.h"
#include "../ParagonUtil/Log.h"
#include "../ParagonData/Scene.h"
#include "../ParagonData/GameConstantData.h"
#include <cassert>
#include <algorithm>
#include <cmath>
#include <numbers>
#include <singleton-cpp/singleton.h>
#include "../ParagonAPI/PgInput.h"
#include "../ParagonAPI/PgScene.h"

namespace Pg::DataScript
{
	GUI_StrongAttackStatus::GUI_StrongAttackStatus(Pg::Data::GameObject* obj) : ScriptInterface(obj)
	{
		_pgInput = &singleton<Pg::API::Input::PgInput>();
		_pgScene = &singleton<Pg::API::PgScene>();

		//Delay 막기 위해서.
		_hwnd = GetActiveWindow();
		assert(_hwnd != nullptr);
	}

	void GUI_StrongAttackStatus::GrabManagedObjects()
	{
		//Image Renderer Frame 기준 오브젝트에 할당되었어야 한다.
		_imageRendererFrame = _object->GetComponent<Pg::Data::ImageRenderer>();
		assert(_imageRendererFrame != nullptr);
		_imageRendererFrame->SetFillRatioDirection(Pg::Data::eFillRatioDirection::UP_TO_DOWN);

		//UpToDown으로.
		Pg::Data::GameObject* tBarObj = _object->GetScene()->FindObjectWithName("GUI_StrongAttackCooldown_Bar");
		assert(tBarObj != nullptr);

		_imageRendererBar = tBarObj->GetComponent<Pg::Data::ImageRenderer>();
		assert(_imageRendererBar != nullptr);
		_imageRendererBar->_sortingLayer = 1;
		_imageRendererBar->SetActive(false);

		//명시적으로 Bar 할당.
		_imageRendererBar->SetFillRatioDirection(Pg::Data::eFillRatioDirection::UP_TO_DOWN);
		_imageRendererBar->_sortingLayer = 2;
		_imageRendererBar->SetActive(false);
	}

	void GUI_StrongAttackStatus::ReceiveDependentPointers(void* p1, void* p2, void* p3, double v1, double v2, double v3)
	{
		//비율로 계산해서 출력할 수 있게 하기 위해서.
		_percentageSource = (const float*)p1;

		//이제는 Bool값으로 가능한지 + Mana까지 해서 필터링해야 한다.
		_isStartEligible = (const bool*)p2;
		_isCheckedBoolPointer = (const bool*)p3;

		//
		_maxVal = v1;
	}

	void GUI_StrongAttackStatus::Start()
	{
		_imageRendererBar->SetActive(false);
		_imageRendererFrame->SetActive(false);
	}

	void GUI_StrongAttackStatus::Update()
	{
		if (_pgInput->GetKeyDown(Pg::API::Input::eKeyCode::MouseRight))
		{
			_imageRendererBar->SetActive(true);
			_imageRendererFrame->SetActive(true);
		}
		if (_pgInput->GetKeyUp(Pg::API::Input::eKeyCode::MouseRight))
		{
			_imageRendererBar->SetActive(false);
			_imageRendererFrame->SetActive(false);
		}

		UpdatePosition();
		UpdateFillAmt();
	}

	void GUI_StrongAttackStatus::UpdateFillAmt()
	{
		//처음에는 안되는 것을 가정.
		float tRatio = 100.f;

		bool tIsStartEligible = *_isStartEligible;
		bool tBool = *_isCheckedBoolPointer;

		//발동은 된다는 얘기.
		if (tIsStartEligible)
		{
			//PG_WARN("ELIGIBLE");
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
			//PG_WARN("HERE");
			float tImpVal = std::clamp<float>(*_percentageSource, 0.f, _maxVal);
			tRatio = (tImpVal / _maxVal) * 100.f; // 0-100 사이.
			tRatio = 100.0f - tRatio;
		}

		_imageRendererFrame->SetFillRatio(tRatio);
	}

	void GUI_StrongAttackStatus::UpdatePosition()
	{
		Pg::Math::PGFLOAT3 tMousePos = {};

		//위 방식이 맞으나, 마우스 위치에 한정해서는 "뒤따라오는" 느낌이 잦다. 이를 해결하기 위해, WinAPI 활용
		//auto editMode = _pgScene->GetEditorMode();
		//if (!(editMode == Data::Enums::eEditorMode::_NONE ||
		//	editMode == Data::Enums::eEditorMode::_EDIT))
		//{
		//	//Play Mode
		//	POINT cursorPos;
		//	GetCursorPos(&cursorPos);
		//	ScreenToClient(_hwnd, &cursorPos);
		//	tMousePos = { (float)cursorPos.x, (float)cursorPos.y, 0 };
		//}
		//else
		//{
			tMousePos = { _pgInput->GetMouseX(), _pgInput->GetMouseY(),0 };

			tMousePos = { tMousePos.x * Pg::Data::GameConstantData::WIDTH,
						tMousePos.y * Pg::Data::GameConstantData::HEIGHT,0 };
		//}

		//알맞은 우측의 Offset 적용.
		tMousePos.x += RIGHT_OFFSET_AMT;

		_imageRendererBar->_object->_transform._position = tMousePos;
		_imageRendererFrame->_object->_transform._position = tMousePos;
	}
}
