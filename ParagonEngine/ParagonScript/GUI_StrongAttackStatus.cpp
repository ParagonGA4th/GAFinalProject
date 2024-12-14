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

		//Delay ���� ���ؼ�.
		_hwnd = GetActiveWindow();
		assert(_hwnd != nullptr);
	}

	void GUI_StrongAttackStatus::GrabManagedObjects()
	{
		//Image Renderer Frame ���� ������Ʈ�� �Ҵ�Ǿ���� �Ѵ�.
		_imageRendererFrame = _object->GetComponent<Pg::Data::ImageRenderer>();
		assert(_imageRendererFrame != nullptr);
		_imageRendererFrame->SetFillRatioDirection(Pg::Data::eFillRatioDirection::UP_TO_DOWN);

		//UpToDown����.
		Pg::Data::GameObject* tBarObj = _object->GetScene()->FindObjectWithName("GUI_StrongAttackCooldown_Bar");
		assert(tBarObj != nullptr);

		_imageRendererBar = tBarObj->GetComponent<Pg::Data::ImageRenderer>();
		assert(_imageRendererBar != nullptr);
		_imageRendererBar->_sortingLayer = 1;
		_imageRendererBar->SetActive(false);

		//��������� Bar �Ҵ�.
		_imageRendererBar->SetFillRatioDirection(Pg::Data::eFillRatioDirection::UP_TO_DOWN);
		_imageRendererBar->_sortingLayer = 2;
		_imageRendererBar->SetActive(false);
	}

	void GUI_StrongAttackStatus::ReceiveDependentPointers(void* p1, void* p2, void* p3, double v1, double v2, double v3)
	{
		//������ ����ؼ� ����� �� �ְ� �ϱ� ���ؼ�.
		_percentageSource = (const float*)p1;

		//������ Bool������ �������� + Mana���� �ؼ� ���͸��ؾ� �Ѵ�.
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
		//ó������ �ȵǴ� ���� ����.
		float tRatio = 100.f;

		bool tIsStartEligible = *_isStartEligible;
		bool tBool = *_isCheckedBoolPointer;

		//�ߵ��� �ȴٴ� ���.
		if (tIsStartEligible)
		{
			//PG_WARN("ELIGIBLE");
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
			//PG_WARN("HERE");
			float tImpVal = std::clamp<float>(*_percentageSource, 0.f, _maxVal);
			tRatio = (tImpVal / _maxVal) * 100.f; // 0-100 ����.
			tRatio = 100.0f - tRatio;
		}

		_imageRendererFrame->SetFillRatio(tRatio);
	}

	void GUI_StrongAttackStatus::UpdatePosition()
	{
		Pg::Math::PGFLOAT3 tMousePos = {};

		//�� ����� ������, ���콺 ��ġ�� �����ؼ��� "�ڵ������" ������ ���. �̸� �ذ��ϱ� ����, WinAPI Ȱ��
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

		//�˸��� ������ Offset ����.
		tMousePos.x += RIGHT_OFFSET_AMT;

		_imageRendererBar->_object->_transform._position = tMousePos;
		_imageRendererFrame->_object->_transform._position = tMousePos;
	}
}
