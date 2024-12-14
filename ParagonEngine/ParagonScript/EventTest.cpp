#include "EventTest.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/Button.h"
#include "../ParagonUtil/Log.h"

#include "../ParagonAPI/PgInput.h"
#include "../ParagonAPI/KeyCodeType.h"
#include <singleton-cpp/singleton.h>
#include <cassert>

namespace Pg::DataScript
{
	EventTest::EventTest(Pg::Data::GameObject* owner) : ScriptInterface(owner)
	{

	}

	void EventTest::Start()
	{
		tButton = _object->GetComponent<Pg::Data::Button>();

		tButton->SetOnClickDownEvent([]
			{
				//assert(false, "���� ���α׷��� ��ŷ ���ߴ�. ����������"); 
				PG_TRACE("Button Clicked!!");
			});
	}

	void EventTest::Update()
	{

	}
}
