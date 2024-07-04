#include "AddEffectorTest.h"
#include "../ParagonAPI/PgGraphics.h"
#include "../ParagonAPI/PgInput.h"

#include <singleton-cpp/singleton.h>
#include <windows.h>

namespace Pg::DataScript
{
	AddEffectorTest::AddEffectorTest(Pg::Data::GameObject* obj) : ScriptInterface(obj)
	{
		
	}

	AddEffectorTest::~AddEffectorTest()
	{

	}

	void AddEffectorTest::Awake()
	{
		_pgGraphics = &singleton<Pg::API::Graphics::PgGraphics>();
		_pgInput = &singleton<Pg::API::Input::PgInput>();
	}

	void AddEffectorTest::Start()
	{
		_ptrRenderMat = _pgGraphics->GetRenderMaterialWithName("RoadLavaConeTestMat.pgmat");
		assert(_ptrRenderMat != nullptr);
		
	}

	void AddEffectorTest::Update()
	{
		InUpdate_MaterialChange();
	}

	void AddEffectorTest::InUpdate_EffectTest()
	{
		if (_pgInput->GetKeyDown(API::Input::KeyNum7))
		{
			if (tObj == nullptr)
			{
				tObj = new Pg::Data::VisualEffectRenderObject();
				tObj->_position = { 0,0,0 };
				_pgGraphics->RegisterEffectObject("Effect_FireArrow", tObj);
				//_pgGraphics->RegisterEffectObject("Effect_Credits", tObj);
				//_pgGraphics->RegisterEffectObject("Effect_3D_FireArrow", tObj);

			}
		}
		else if (_pgInput->GetKeyDown(API::Input::KeyNum8))
		{
			if (tObj != nullptr)
			{
				_pgGraphics->RemoveEffectObject(tObj);
				delete tObj;

				tObj = nullptr;
			}
		}
	}

	void AddEffectorTest::InUpdate_MaterialChange()
	{
		if (_pgInput->GetKeyDown(API::Input::KeyNum7))
		{
			_pgGraphics->Material_SetBoolPS(_ptrRenderMat, "isOne", true);
		}
		else if (_pgInput->GetKeyUp(API::Input::KeyNum7))
		{
			_pgGraphics->Material_SetBoolPS(_ptrRenderMat, "isOne", false);
		}

		if (_pgInput->GetKeyDown(API::Input::KeyNum8))
		{
			_pgGraphics->Material_SetBoolVS(_ptrRenderMat, "toggleNoise", true);
		}
		else if (_pgInput->GetKeyUp(API::Input::KeyNum8))
		{
			_pgGraphics->Material_SetBoolVS(_ptrRenderMat, "toggleNoise", false);
		}
	}

	void AddEffectorTest::InUpdate_Fade()
	{
		if (_pgInput->GetKeyDown(API::Input::KeyNum7))
		{
			_pgGraphics->ScreenSpace_FadeOut();
		}
		else if (_pgInput->GetKeyUp(API::Input::KeyNum7))
		{
			_pgGraphics->ScreenSpace_FadeIn();
		}
	}

}