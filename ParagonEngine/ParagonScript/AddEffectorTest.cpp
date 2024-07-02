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
		
	}

	void AddEffectorTest::Update()
	{
		if (_pgInput->GetKeyDown(API::Input::KeyNum7))
		{
			tObj = new Pg::Data::VisualEffectRenderObject();
			tObj->_position = { 500,500,0 };
			//_pgGraphics->RegisterEffectObject("Effect_FireArrow", tObj);
			_pgGraphics->RegisterEffectObject("Effect_Credits", tObj);
		}
		else if (_pgInput->GetKeyDown(API::Input::KeyNum8))
		{
			if (tObj != nullptr)
			{
				_pgGraphics->RemoveEffectObject(tObj);
				delete tObj;
			}
		}
	}

}